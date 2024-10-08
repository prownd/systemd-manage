[![build](https://github.com/prownd/systemd-manage/actions/workflows/main.yml/badge.svg)](https://github.com/prownd/systemd-manage/actions/workflows/main.yml)
![GitHub License](https://img.shields.io/github/license/prownd/systemd-manage)
![Static Badge](https://img.shields.io/badge/qt-%3E%3D5-green)
![GitHub Release](https://img.shields.io/github/v/release/prownd/systemd-manage)

# systemd-manage工具
systemd-manage是一个基于systemd服务管理的图形化工具，使用qt图形库进行开发。  
提供服务管理，用户会话，配置文件修改，日志查询，性能分析，进程管理等功能。  
图形化，简单方便的进行系统服务的管理。

# 编译构建安装
使用qmake进行编译和部署
```bash
qmake
make
make install
```

也可以编译成发行版格式的二进制软件包，比如编译生成rpm包, 或者deb包
- 编译生成rpm包
```bash
cd dist
./build.sh rpm
```

- 编译生成deb包
```bash
cd dist
./build.sh deb
```

- 编译生成tar包
```bash
cd dist
./build.sh tar
```

# systemd-manage使用
### 系统服务管理:
可以进行系统单元和用户单元的管理，筛选unit类型等
![service manage](images/zh/service-manage-view-1.png "systemd service unit manage")

### 系统服务详情:
显示服务单元内容，日志，依赖关系等
![service detail](images/zh/service-detail-view-1.png "service unit detail")

### 用户会话管理：
管理系统登陆的会话和用户
![user session](images/zh/user-session-view-1.png "user session info")

### 用户会话详情：
显示会话的状态信息，比如：ID，名字，路径，状态，TTY等信息
![user session detail](images/zh/user-session-detail-1.png "show user session detail")

### 配置文件：
对systemd的配置文件进行查看管理，修改参数项值
![config file](images/zh/config-file-view-1.png "config file info")
### 配置文件详情：
显示systemd的配置文件信息，参数字段，参数值等
![config file detail](images/en/config-file-detail-1.png "config file detail")

### journal日志：
查看系统的日志，可以选择时间段，日志级别
![journal log](images/zh/journal-log-view-1.png "show journal log")

### systemd性能分析：
显示系统启动阶段时间消耗，还有更详细的火焰图，查看具体的每个服务的启动时间
![analysize statistics](images/zh/analysize-statistics-view-1.png "systemd performance analysize statistics")

### 进程管理：
显示当前运行的进程，可以过滤筛选进程名，用户
![process manage](images/zh/process-manage-view-1.png "process management")

### 进程详细信息:
显示进程详细信息，例如:进程名，id，状态，内存等
![process detail](images/zh/process-detail-view-1.png "process detail information")

### Udev设备：
显示系统所有的Udev设备项信息
![udev device view](images/en/udev-device-view-1.png "udev device view")

### Udev设备详情:
显示Udev设备信息，例如:进程名，id，状态，内存等
![udev device detail](images/en/udev-device-detail-1.png "udev device detail information")


### 首选项设置：
系统首选项的设置，设置显示语言
![perference setting](images/zh/setting-view-1.png "program perference setting")

### 关于程序和版本：
显示关于程序版本，许可信息等
![about application version](images/zh/about-view-1.png "abuot version info")


# 许可协议
systemd-manage is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

systemd-manage is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/) for more details.


# 反馈和联系
如果你在使用中，发现bug问题，或者有更好的建议意见。  
还请提出issue问题，或者提交pr来进行修改优化。  
也可以发送邮件与我联系交流。  
欢迎反馈贡献，谢谢!

联系邮箱：  [hanjinpeng127@gmail.com](mailto:hanjinpeng127@gmail.com)












