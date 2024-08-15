Name: systemd-manage           
Version: 1.0       
Release: 1%{?dist}
Summary: A graphical manage tools of systemd   

License: LGPLv2
URL:  https://github.com/prownd/systemd-manage           
Source0:     https://github.com/prownd/systemd-manage/releases/download/%{name}-%{version}.tar.xz

BuildRequires: gcc-c++
BuildRequires: systemd-devel >= 209
BuildRequires: qt5-qtbase-devel
BuildRequires: qt5-qtsvg-devel
BuildRequires: qt5-qtscript-devel
BuildRequires: qt5-qttools-devel
BuildRequires: qt5-qtcharts-devel

Requires: qt5-qtbase
Requires: systemd       

%description
Systemd manage is a graphical tool based on Systemd service management, developed using the Qt graphics library, providing functions such as service management, user sessions, configuration file modification, log queries, performance analysis, and process management. Simple and convenient to manage systems and services.


%prep
%autosetup -n %{name}
#%patch -p1 1


%build
export PATH=%{_qt5_bindir}:$PATH
mkdir %{name}-qmake-build
pushd %{name}-qmake-build
%{qmake_qt5} ..
%{make_build}
popd


%install
pushd %{name}-qmake-build
%{make_install} INSTALL_ROOT=%{buildroot}  prefix=/usr/
popd

# Add desktop files
# desktop-file-install \
#  --dir=%{buildroot}%{_datadir}/applications \
#  data/%{name}.desktop

# Add .desktop file
install -d %{buildroot}%{_datadir}/applications/
install -m644 -p -D data/%{name}.desktop  %{buildroot}%{_datadir}/applications/%{name}.desktop

# icons
install -d %{buildroot}%{_datadir}/%{name}/icons/
install -m644 -p -D res/icons/* %{buildroot}%{_datadir}/%{name}/icons/

# translate file
install -d %{buildroot}%{_datadir}/%{name}/translations/
install -m644 -p -D res/translators/*.qm  %{buildroot}%{_datadir}/%{name}/translations/

# license file
#install -d %{buildroot}%{_datadir}/%{name}/license/
#install -m644 -p -D LICENSE  %{buildroot}%{_datadir}/%{name}/license/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%license LICENSE
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}/icons/*
%{_datadir}/%{name}/translations/*

%changelog
* Sun Aug 04 2024 hanjinpeng <hanjinpeng127@gmail.com> - 1.0-1
- Initial systemd-manage package
