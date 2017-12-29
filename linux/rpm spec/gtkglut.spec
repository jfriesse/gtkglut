# Authority: Jan Friesse

%{?dist: %{expand: %%define %dist 1}}

Summary: GLUT replacement based on GTK+ and GtkGLExt
Name: gtkglut
Version: 0.3.7
Release: 1
License: MIT
Group: System Environment/Libraries

Source: %{name}-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires: gtk2-devel 
# libtool *sigh*
BuildRequires: gcc-c++
BuildRequires: libXt-devel, mesa-libGLU-devel
BuildRequires: gtkglext-devel

%description
GtkGLUT is an GLUT replacement based on GTK+ and GtkGLExt.

%package devel
Summary: Header files, libraries and development documentation for %{name}
Group: Development/Libraries
Requires: %{name} = %{version}, gtk2-devel

%description devel
This package contains the header files, static libraries and development
documentation for %{name}. If you like to develop programs using %{name},
you will need to install %{name}-devel.

%prep
%setup


%build
%{__make} PREFIX=%{_prefix} default docs


%install
%{__rm} -rf %{buildroot}
%{__make} PREFIX=%{buildroot}/%{_prefix} install


%clean
%{__rm} -rf %{buildroot}


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


%files
%defattr(-, root, root, 0755)
%doc COPYING INSTALL README README.win32
%{_libdir}/*.so.*
%{_libdir}/*.so

%files devel
%defattr(-, root, root, 0755)
%doc doc/html/
%{_includedir}/gtkglut-0.3/
%{_libdir}/*.a
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


%changelog
* Sat Apr 19 2008 Jan Friesse <jfriesse@gmail.com> 0.3.7-1
- First version
