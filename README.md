[![build](https://github.com/prownd/systemd-manage/actions/workflows/main.yml/badge.svg)](https://github.com/prownd/systemd-manage/actions/workflows/main.yml)
![GitHub License](https://img.shields.io/github/license/prownd/systemd-manage)
![Static Badge](https://img.shields.io/badge/qt-%3E%3D5-green)
![GitHub Release](https://img.shields.io/github/v/release/prownd/systemd-manage)

# Systemd manage tool
systemd-manage is a graphical systemd service management tool, developed using the Qt graphics library.  
It can be providing functions such as service management, user sessions, configuration file modification, log queries, performance analysis and process management, etc.  
Graphic interface operation, simple and convenient service management.

# Build Deploy and Install
Use qmake for compile build, configuration, and installation.

```bash
qmake
make
make install
```

In addition, it can also be compiled into distribution software packages, such as rpm or deb package format, tar package.

- build rpm package
```bash
cd dist
./build.sh rpm
```

- build deb package
```bash
cd dist
./build.sh deb
```

- build tar package
```bash
cd dist
./build.sh tar
```

# Instructions for using systemd management
### System Service Management:
Manage system units and user units, filter unit types, etc.
![service manage](./images/en/service-manage-view-1.png "systemd service unit manage")

### System Service Details:
Display service unit content, logs, dependencies, etc.
![service detail](images/en/service-detail-view-1.png "service unit detail")

### User session management:
Management system login sessions and users.
![user session](images/en/user-session-view-1.png "user session info")

### User session details:
Display session status information, such as ID, name, path, status, TTY, etc.
![user session detail](images/en/user-session-detail-1.png "show user session detail")

### Configuration file:
View and manage the configuration files of systemd, modify parameter values.
![config file](images/en/config-file-view-1.png "config file info")

### Configuration file detail:
display  detail information for configuration files of systemd.
![config file detail](images/en/config-file-detail-1.png "config file detail")

### Journal log:
View system logs, select time period and log level.
![journal log](images/en/journal-log-view-1.png "show journal log")

### System performance analysis:
Display the time consumption during the system startup phase, as well as a more detailed flame diagram to view the specific startup time of each service.
![analysize statistics](images/en/analysize-statistics-view-1.png "systemd performance analysize statistics")

### Process management:
Display the currently running process, filter process names, and users.
![process manage](images/en/process-manage-view-1.png "process management")

### Process Details Information:
Display process detail information, eg: name, id, state, memory, etc.
![process detail](images/en/process-detail-view-1.png "process detail information")

### Udev Device View:
Display the all udev device event.
![udev device view](images/en/udev-device-view-1.png "udev device view")

### Udev Device Details information:
Display udev device detail information, eg: path,  subsystem, type,node,action etc.
![udev device detail](images/en/udev-device-detail-1.png "udev device detail information")

### Preferred settings:
Setting system preferences, setting display language.
![perference setting](images/en/setting-view-1.png "program perference setting")

### Application version information:
Display information about program version, license, etc.
![about application version](images/en/about-view-1.png "abuot version info")

# license agreement
systemd-manage is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

systemd-manage is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at [ http://www.gnu.org/licenses/ ]( http://www.gnu.org/licenses/ ) for more details.

# Feedback and Contact
If you encounter bug issues or have better suggestions while using it.  
Please report issue questions or submit PR for modification and optimization.  
You can also send me an email to contact and communicate with me  
Welcome feedback and contributions, thank you!

Contact email:  [hanjinpeng127@gmail.com](mailto:hanjinpeng127@gmail.com)
