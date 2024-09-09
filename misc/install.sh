#!/bin/bash

#########################################################################
# File Name: install.sh
# Author: hanjinpeng
# mail: hanjinpeng127@163.com
# Created Time: Tue 10 Sep 2024 01:02:55 AM CST
#########################################################################

PKG_NAME="systemd-manage"
PKG_VERSION="1.2"
PKG_VERSION_RELEASE="${PKG_VERSION}-1"
TOP_DIR="$( cd "$( dirname "$0" )" && pwd  )"
OS_ARCH="`arch`"
PKG_TAR_NAME="${PKG_NAME}-${PKG_VERSION_RELEASE}-${OS_ARCH}"

function  do_help()
{
     echo -e "\033[31m usage:\n $0 \033[0m"
     exit 1
}

function do_install()
{
    # install binnary file
    if [  -e "${TOP_DIR}/${PKG_NAME}"  ];then
        install -d  /usr/bin/
        install -m755 -p -D ${TOP_DIR}/${PKG_NAME}  /usr/bin/
    else
        echo "No $PKG_NAME file"
        exit 2
	fi

    # install .desktop file
    if [  -e "${TOP_DIR}/data/applications/${PKG_NAME}.desktop" ];then
    	install -d  /usr/share/applications/
    	install -m644 -p -D $TOP_DIR/data/applications/${PKG_NAME}.desktop  /usr/share/applications/${PKG_NAME}.desktop
    else
        echo "No ${PKG_NAME}.desktop file"
        exit 2
	fi

    # install icons
    if [  -d  "${TOP_DIR}/data/icons/" ];then
    	install -d /usr/share/${PKG_NAME}/icons/
    	install -m644 -p -D $TOP_DIR/data/icons/*  /usr/share/${PKG_NAME}/icons/
    else
        echo "No icons file"
        exit 2
	fi

    # installlicense file
    if [  -e  "${TOP_DIR}/data/license/LICENSE" ];then
    	install -d  /usr/share/${PKG_NAME}/license/
    	install -m644 -p -D ${TOP_DIR}/data/license/LICENSE  /usr/share/${PKG_NAME}/license/
    else
        echo "No  LICENSE file"
        exit 2
	fi
}

if [ x"$1"  = x"help" ];then
    do_help
    exit 1
fi

do_install
