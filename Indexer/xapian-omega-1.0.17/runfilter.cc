/* runfilter.cc: run an external filter and capture its output in a std::string.
 *
 * Copyright (C) 2003,2006,2007,2009 Olly Betts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <config.h>

#include <iostream>
#include <string>

#include <sys/types.h>
#include "safeerrno.h"
#include "safefcntl.h"
#include <stdio.h>
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#include "safeunistd.h"

#if defined HAVE_FORK && defined HAVE_SOCKETPAIR && defined HAVE_SETRLIMIT
# include <signal.h>
#endif

#include "freemem.h"
#include "runfilter.h"

#ifdef __WIN32__
# ifndef WIFEXITED
#  define WIFEXITED(status) (status != -1)
# endif
# ifndef WEXITSTATUS
#  define WEXITSTATUS(status) (status)
# endif
#endif

#ifdef _MSC_VER
# define popen _popen
# define pclose _pclose
#endif

using namespace std;

string
stdout_to_string(const string &cmd)
{
    string out;
#if defined HAVE_FORK && defined HAVE_SOCKETPAIR && defined HAVE_SETRLIMIT
    // We want to be able to get the exit status of the child process.
    signal(SIGCHLD, SIG_DFL);

    int fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, fds) < 0)
	throw ReadError();

    pid_t child = fork();
    if (child == 0) {
	// We're the child process.

	// Close the parent's side of the socket pair.
	close(fds[0]);

	// Connect stdout to our side of the socket pair.
	dup2(fds[1], 1);

	// Impose some pretty generous resource limits to prevent run-away
	// filter programs from causing problems.

	// Limit CPU time to 300 seconds (5 minutes).
	struct rlimit cpu_limit = { 300, RLIM_INFINITY } ;
	setrlimit(RLIMIT_CPU, &cpu_limit);

#if defined RLIMIT_AS || defined RLIMIT_VMEM || defined RLIMIT_DATA
	// Limit process data to 7/8 of free physical memory.
	long mem = get_free_physical_memory();
	if (mem > 0) {
	    mem = (mem / 8) * 7;
	    struct rlimit ram_limit = { mem, RLIM_INFINITY } ;
#ifdef RLIMIT_AS
	    setrlimit(RLIMIT_AS, &ram_limit);
#elif defined RLIMIT_VMEM
	    setrlimit(RLIMIT_VMEM, &ram_limit);
#else
	    // Only limits the data segment rather than the total address
	    // space, but that's better than nothing.
	    setrlimit(RLIMIT_DATA, &ram_limit);
#endif
	}
#endif

	execl("/bin/sh", "/bin/sh", "-c", cmd.c_str(), (void*)NULL);
	_exit(-1);
    }

    // We're the parent process.

    // Close the child's side of the socket pair.
    close(fds[1]);
    if (child == -1) {
	// fork() failed.
	close(fds[0]);
	throw ReadError();
    }

    int fd = fds[0];

    while (true) {
	char buf[4096];
	ssize_t res = read(fd, buf, sizeof(buf));
	if (res == 0) break;
	if (res == -1) {
	    if (errno == EINTR) {
		// read() interrupted by a signal, so retry.
		continue;
	    }
	    close(fd);
	    int status;
	    (void)waitpid(child, &status, 0);
	    throw ReadError();
	}
	out.append(buf, res);
    }

    close(fd);
    int status;
    if (waitpid(child, &status, 0) == -1) {
	throw ReadError();
    }
#else
    FILE * fh = popen(cmd.c_str(), "r");
    if (fh == NULL) throw ReadError();
    while (!feof(fh)) {
	char buf[4096];
	size_t len = fread(buf, 1, 4096, fh);
	if (ferror(fh)) {
	    (void)pclose(fh);
	    throw ReadError();
	}
	out.append(buf, len);
    }
    int status = pclose(fh);
#endif

    if (status != 0) {
	if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
	    throw NoSuchFilter();
	}
#ifdef SIGXCPU
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGXCPU) {
	    cerr << "Filter process consumed too much CPU time" << endl;
	}
#endif
	throw ReadError();
    }
    return out;
}
