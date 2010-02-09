# RedHat-style .spec file for Xapian
# xapian-omega.spec.  Generated from xapian-omega.spec.in by configure.
%define contentdir /var/lib
%define logdir /var/log

Summary: A CGI search frontend and indexers built on Xapian
Name: xapian-omega
Version: 1.0.17
Release: 1
License: GPL
Vendor: xapian.org
Group: Applications/Internet
URL: http://xapian.org/
# FIXME: once we reach 1.0 "=" should become ">="
Requires: xapian-core-libs = %{version}, webserver
BuildRequires: xapian-core-devel = %{version}
BuildRequires: autoconf automake libtool
Source0: http://oligarchy.co.uk/xapian/%{version}/%{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Omega is a CGI application which uses the Xapian Information Retrieval
library to index and search collections of documents.

%prep
%setup -q

%build
# FC6 (at least) has a patched libtool which knows not to set rpath for
# /usr/lib64, which upstream libtool fails to do currently.  We can drop
# this "autoreconf --force" and the "BuildRequires:" for the autotools
# once upstream libtool is fixed.  Note: this overwrites INSTALL, but
# that doesn't matter here as we don't package it.
autoreconf --force
%configure
make

%install
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}
mkdir -p %{buildroot}
%makeinstall
# CGI application
mkdir -p %{buildroot}/var/www/cgi-bin/
mv %{buildroot}%{_libdir}/xapian-omega/bin/omega %{buildroot}/var/www/cgi-bin
# Create /var directories
mkdir -p %{buildroot}%{contentdir}/omega/data
mkdir -p %{buildroot}%{contentdir}/omega/cdb
mkdir -p %{buildroot}%{logdir}/omega
# Default templates
mkdir -p %{buildroot}%{contentdir}/omega/templates
cp -r templates/* %{buildroot}%{contentdir}/omega/templates/
# Images
mkdir -p %{buildroot}/var/www/icons/omega
cp -r images/* %{buildroot}/var/www/icons/omega/
# Configuration file
mkdir -p %{buildroot}/etc
install -D -m644 omega.conf %{buildroot}/etc/omega.conf
# Move the scripts to the right place
mv %{buildroot}/usr/share/omega %{buildroot}%{_datadir}/%{name}
# Move the docs to the right place
mv %{buildroot}/usr/share/doc/xapian-omega %{buildroot}/usr/share/doc/%{name}-%{version}
cp AUTHORS ChangeLog COPYING NEWS README TODO %{buildroot}/usr/share/doc/%{name}-%{version}

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(-, root, root)
%{_bindir}/dbi2omega
%{_bindir}/omindex
%{_bindir}/scriptindex
%{_bindir}/htdig2omega
%{_bindir}/mbox2omega
%{contentdir}/omega
%{logdir}/omega
/var/www/cgi-bin/omega
/var/www/icons/omega
%{_datadir}/%{name}
%config(noreplace) /etc/omega.conf
%doc %{_datadir}/doc/%{name}-%{version}
# man pages may be gzipped, hence the trailing wildcard.
%{_mandir}/man1/omindex.1*
%{_mandir}/man1/scriptindex.1*
