#!/bin/sh

# you should run this script on fedora
# you need to have the following packages:
# rpmdevtools, rpm-build, gawk, nautilus-devel, automake,
# autoconf, gnome-common, libtool, gcc

if [ $(basename $(pwd)) != 'nautilus-skype' ]; then
    echo "This script must be run from the nautilus-skype folder"
    exit -1
fi

BUILD=1
while [ $# != 0 ]; do
    flag="$1"
    case "$flag" in
        -n)
	    BUILD=0
            ;;
    esac
    shift
done

# creating an RPM is easier than creating a debian package, surprisingly
# I call bullcrap on this statement.  They are exactly the same.
set -e

# get version
if which gawk; then
    CURVER=$(gawk '/^AC_INIT/{sub("AC_INIT\\(\\[nautilus-skype\\],", ""); sub("\\)", ""); print $0}' configure.in)
else
    CURVER=$(awk '/^AC_INIT/{sub("AC_INIT\(\[nautilus-skype\],", ""); sub("\)", ""); print $0}' configure.in)    
fi

# backup old rpmmacros file
if [ -e $HOME/.rpmmacros ] && [ ! -e $HOME/.rpmmacros.old ]; then
    mv $HOME/.rpmmacros $HOME/.rpmmacros.old
fi

cat <<EOF > $HOME/.rpmmacros
%_topdir      $(pwd)/rpmbuild
%_tmppath              $(pwd)/rpmbuild
%_smp_mflags  -j3
%_signature gpg
%_gpg_name 3565780E
%_gpgbin /usr/bin/gpg
EOF

# clean old package build
rm -rf nautilus-skype{-,_}*
rm -rf rpmbuild

# build directory tree
mkdir rpmbuild
for I in BUILD RPMS SOURCES SPECS SRPMS; do
    mkdir rpmbuild/$I
done;

if [ ! -x configure ]; then
    ./autogen.sh
fi

if [ ! -e Makefile ]; then
    ./configure
fi

# generate package
make dist

cp nautilus-skype-$CURVER.tar.bz2 rpmbuild/SOURCES/

cat <<EOF > rpmbuild/SPECS/nautilus-skype.spec
%define glib_version 2.14.0
%define nautilus_version 2.16.0
%define libgnome_version 2.16.0
%define pygtk2_version 2.12
%define pygpgme_version 0.1

Name:		nautilus-skype
Version:	$CURVER
Release:	1%{?dist}
Summary:	Skype integration for Nautilus
Group:		User Interface/Desktops
License:	GPLv3
URL:		http://www.skype.com/
Source0:	http://dl.skype.com/u/17/%{name}-%{version}.tar.bz2
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id} -u -n)

Requires:	nautilus-extensions >= %{nautilus_version}
Requires:	glib2 >= %{glib_version}
Requires:	libgnome >= %{gnome_version}
Requires:	pygtk2 >= %{pygtk2_version}

%{?with_suggest_tags:Suggests: pygpgme}

BuildRequires:	nautilus-devel >= %{nautilus_version}
BuildRequires:	glib2-devel >= %{glib_version}
BuildRequires:	python-docutils
BuildRequires:  cairo-devel
BuildRequires:  gtk2-devel
BuildRequires:  atk-devel
BuildRequires:  pango-devel

%description
Nautilus Skype is an extension that integrates
the Skype web service with your GNOME Desktop.

Check us out at http://www.skype.com/

%prep
%setup -q


%build
export DISPLAY=$DISPLAY
%configure
make %{?_smp_mflags}

%install
rm -rf \$RPM_BUILD_ROOT
make install DESTDIR=\$RPM_BUILD_ROOT

if [ -d \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-2.0 ]; then
    rm \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-2.0/*.la
    rm \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-2.0/*.a
    mkdir -p \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-3.0
    ln -s ../extensions-2.0/libnautilus-skype.so \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-3.0/
elif [ -d \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-3.0 ]; then
    rm \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-3.0/*.la
    rm \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-3.0/*.a
    mkdir -p \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-2.0
    ln -s ../extensions-3.0/libnautilus-skype.so \$RPM_BUILD_ROOT%{_libdir}/nautilus/extensions-2.0/
fi

%post
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :
/bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

if [ \$1 -gt 1 ] ; then
  # Old versions of the rpm delete the files in postun.  So just in case let's make a backup copy.  The backup copy will be restored in posttrans.
  ln -f %{_libdir}/nautilus/extensions-3.0/libnautilus-skype.so{,.bak}
  ln -f %{_libdir}/nautilus/extensions-2.0/libnautilus-skype.so{,.bak}
fi

EOF

cat <<'EOF' >> rpmbuild/SPECS/nautilus-skype.spec
for I in /home/*/.skype-dist;
do
  # require a minimum version of 1.0.0
  SKYPE_VERSION="$I/VERSION"
  if test -e "$SKYPE_VERSION"; then
    VERSION=`cat "$SKYPE_VERSION"`

    case "$VERSION" in
      1.2.4[3-6]|0.*.*)
        # 1.2.43-1.2.46 had a bug that prevents auto-update.
        # stop skype
        pkill -xf $I/skype > /dev/null 2>&1
        sleep 0.5
        rm -rf "$I"
    esac
  fi
done

DEFAULTS_FILE="/etc/default/skype-repo"

if [ ! -e "$DEFAULTS_FILE" ]; then
    YUM_REPO_FILE="/etc/yum.repos.d/skype.repo"

    if ! rpm -q gpg-pubkey-5044912e-4b7489b1 > /dev/null 2>&1 ;  then
      rpm --import - <<KEYDATA
-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: GnuPG v1.4.9 (GNU/Linux)

mQENBEt0ibEBCACv4hZRPqwtpU6z8+BB5YZU1a3yjEvg2W68+a6hEwxtCa2U++4d
zQ+7EqaUq5ybQnwtbDdpFpsOi9x31J+PCpufPUfIG694/0rlEpmzl2GWzY8NqfdB
FGGm/SPSSwvKbeNcFMRLu5neo7W9kwvfMbGjHmvUbzBUVpCVKD0OEEf1q/Ii0Qce
kx9CMoLvWq7ZwNHEbNnij7ecnvwNlE2MxNsOSJj+hwZGK+tM19kuYGSKw4b5mR8I
yThlgiSLIfpSBh1n2KX+TDdk9GR+57TYvlRu6nTPu98P05IlrrCP+KF0hYZYOaMv
Qs9Rmc09tc/eoQlN0kkaBWw9Rv/dvLVc0aUXABEBAAG0MURyb3Bib3ggQXV0b21h
dGljIFNpZ25pbmcgS2V5IDxsaW51eEBkcm9wYm94LmNvbT6JATYEEwECACAFAkt0
ibECGwMGCwkIBwMCBBUCCAMEFgIDAQIeAQIXgAAKCRD8kYszUESRLi/zB/wMscEa
15rS+0mIpsORknD7kawKwyda+LHdtZc0hD/73QGFINR2P23UTol/R4nyAFEuYNsF
0C4IAD6y4pL49eZ72IktPrr4H27Q9eXhNZfJhD7BvQMBx75L0F5gSQwuC7GdYNlw
SlCD0AAhQbi70VBwzeIgITBkMQcJIhLvllYo/AKD7Gv9huy4RLaIoSeofp+2Q0zU
HNPl/7zymOqu+5Oxe1ltuJT/kd/8hU+N5WNxJTSaOK0sF1/wWFM6rWd6XQUP03Vy
NosAevX5tBo++iD1WY2/lFVUJkvAvge2WFk3c6tAwZT/tKxspFy4M/tNbDKeyvr6
85XKJw9ei6GcOGHD
=5rWG
-----END PGP PUBLIC KEY BLOCK-----
KEYDATA
    fi
    if [ -d "/etc/yum.repos.d" ]; then
      cat > "$YUM_REPO_FILE" << REPOCONTENT
[Skype]
name=Skype Repository
baseurl=http://linux.skype.com/fedora/\$releasever/
gpgkey=http://linux.skype.com/fedora/rpm-public-key.asc
REPOCONTENT
    fi
fi

PROCS=`pgrep -x nautilus`

for PROC in $PROCS; do
  # Extract the display variable so that we can show a box.  
  # Hope they have xauth to localhost. 
  export `cat /proc/$PROC/environ | tr "\0" "\n" | grep DISPLAY` 

  zenity --question --timeout=30 --title=Skype --text='The Nautilus File Browser has to be restarted. Any open file browser windows will be closed in the process. Do this now?' > /dev/null 2>&1
  if [ $? -eq 0 ] ; then
    echo "Killing nautilus"
    kill $PROC
  fi
done

if ! pgrep -x skype > /dev/null 2>&1 ;  then
  zenity --info --timeout=5 --text='Skype installation successfully completed! You can start Skype from your applications menu.' > /dev/null 2>&1
  if [ $? -ne 0 ]; then
      echo
      echo 'Skype installation successfully completed! You can start Skype from your applications menu.'
  fi
fi
EOF

cat <<EOF >> rpmbuild/SPECS/nautilus-skype.spec
%postun
# Warning. postun also runs on upgrades.
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :
if [ \$1 -eq 0 ] ; then
    /bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    /usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

# Old versions of the rpm delete these files in postun.  Fortunately we have saved a backup.  
if [ ! -e %{_libdir}/nautilus/extensions-3.0/libnautilus-skype.so ]; then
  if [ -e %{_libdir}/nautilus/extensions-3.0/libnautilus-skype.so.bak ]; then
    mv -f %{_libdir}/nautilus/extensions-3.0/libnautilus-skype.so{.bak,}
  fi
fi
if [ ! -e %{_libdir}/nautilus/extensions-2.0/libnautilus-skype.so ]; then
  if [ -e %{_libdir}/nautilus/extensions-2.0/libnautilus-skype.so.bak ]; then
    mv -f %{_libdir}/nautilus/extensions-2.0/libnautilus-skype.so{.bak,}
  fi
fi

rm -f %{_libdir}/nautilus/extensions-3.0/libnautilus-skype.so.bak
rm -f %{_libdir}/nautilus/extensions-2.0/libnautilus-skype.so.bak

%clean
rm -rf \$RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc
%{_libdir}/nautilus/extensions-2.0/*.so*
%{_libdir}/nautilus/extensions-3.0/*.so*
%{_datadir}/icons/hicolor/*
%{_datadir}/nautilus-skype/emblems/*
%{_bindir}/skype
%{_datadir}/applications/skype.desktop
%{_datadir}/man/man1/skype.1.gz

%changelog
* $(date +'%a %b %d %Y')  Rian Hunter <rian@skype.com> - $CURVER-1
- This package does not use a changelog
EOF

cd rpmbuild
if [ $BUILD -eq 1 ]; then
    rpmbuild -ba SPECS/nautilus-skype.spec

    # sign all rpms
    find . -name '*.rpm' | xargs rpm --addsign
else
    # Kind of silly but this is the easiest way to get this info the the build_slave.
    rpmbuild -bs SPECS/nautilus-skype.spec > ../buildme
fi

# restore old macros file
if [ -e $HOME/.rpmmacros.old ]; then
    mv $HOME/.rpmmacros.old $HOME/.rpmmacros
fi
