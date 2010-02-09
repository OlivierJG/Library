/* md5wrap.cc: wrapper functions to allow easy use of MD5 from C++.
 *
 * Copyright (C) 2006 Olly Betts
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

#ifdef HAVE_POSIX_FADVISE
# ifdef __linux__
#  define _POSIX_C_SOURCE 200112L // for posix_fadvise from fcntl.h
#  define _BSD_SOURCE 1 // Need this to get lstat() as well
# endif
#endif

#include <string>

#include "safefcntl.h"
#include "safeerrno.h"
#include "safeunistd.h"

#ifndef O_STREAMING
# ifdef __linux__
// This is the value used by rml's O_STREAMING patch for 2.4.
#  define O_STREAMING	04000000
# else
// Define as 0 otherwise, so we don't need ifdefs in the code.
#  define O_STREAMING	0
# endif
#endif

#include "md5.h"
#include "md5wrap.h"

using namespace std;

bool
md5_file(const string &file_name, string &md5)
{
    mode_t mode = O_RDONLY;
    mode |= O_STREAMING;

    int fd = open(file_name.c_str(), mode);
    if (fd < 0) return false;

#ifdef HAVE_POSIX_FADVISE
    posix_fadvise(fd, 0, 0, POSIX_FADV_NOREUSE); // or POSIX_FADV_SEQUENTIAL
#endif

    MD5Context md5_ctx;
    MD5Init(&md5_ctx);

    unsigned char blk[4096];

    while (true) {
	int c = read(fd, blk, sizeof(blk));
	if (c == 0) break;
	if (c < 0) {
	    if (errno == EINTR) continue;
	    break;
	}
	MD5Update(&md5_ctx, blk, c);
    }

#ifdef HAVE_POSIX_FADVISE
    posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);
#endif

    close(fd);

    MD5Final(blk, &md5_ctx);
    md5.assign(reinterpret_cast<const char *>(blk), 16);

    return true;
}

void
md5_string(const string &str, string &md5)
{
    unsigned char blk[16];
    MD5Context md5_ctx;

    MD5Init(&md5_ctx);
    MD5Update(&md5_ctx, reinterpret_cast<const unsigned char *>(str.data()),
	      str.size());
    MD5Final(blk, &md5_ctx);
    md5.assign(reinterpret_cast<const char *>(blk), 16);
}
