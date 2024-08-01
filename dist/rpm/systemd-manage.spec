Name: systemd-manage           
Version: 1        
Release: 1%{?dist}
Summary: A graphical manage tools of systemd   

License: LGPLv2
URL:  https://github.com/prownd/systemd-manage           
Source0:     https://github.com/prownd/systemd-manage/releases/download/%{name}-%{version}.tar.xz

BuildRequires: gcc-c++
BuildRequires: systemd-devel
Requires: systemd       

%description
Systemd manage is a graphical tool based on Systemd service management, developed using the Qt graphics library, providing functions such as service management, user sessions, configuration file modification, log queries, performance analysis, and process management. Simple and convenient to manage systems and services.


%prep
%autosetup


%build
%configure
%make_build


%install
%make_install

%files
%license add-license-file-here
%doc add-docs-here

%changelog
* Thu Aug 01 2024 hanjinpeng <hanjinpeng127@gmail.com> - 1.0-1
- Initial spec for Package
