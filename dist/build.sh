#!/usr/bin/bash
#########################################################################
# File Name: build.sh
# Author: hanjinpeng
# mail: hanjinpeng127@gmail.com
# Created Time: 2024年08月06日 星期二 00时12分46秒
#########################################################################

PKG_NAME=systemd-manage
PKG_VERSION=1.0
BUILD_DIR="$( cd "$( dirname "$0"  )" && pwd  )"

function build_rpm_package()
{
    #clean rpmbuild dir and regenerate rpmdev tree
    [ -d $BUILD_DIR/rpmbuild ] && rm -fr $BUILD_DIR/rpmbuild
    mkdir -p $BUILD_DIR/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
    mkdir -p $BUILD_DIR/rpms-dir

    #tar source file, generate xz archive 
    #tar -Jcvf $BUILD_DIR/rpmbuild/SOURCES/$PKG_NAME-$PKG_VERSION.tar.xz  $BUILD_DIR/../../$PKG_NAME
    tar -Jcv --exclude='.git' -f $BUILD_DIR/rpmbuild/SOURCES/$PKG_NAME-$PKG_VERSION.tar.xz  $BUILD_DIR/../../$PKG_NAME

    #begin build
    rpmbuild --define "_topdir $BUILD_DIR/rpmbuild" -ba  $BUILD_DIR/rpm/$PKG_NAME.spec

    #find package rpm file , and copy to rpms-dir
    rpms=`find $BUILD_DIR/rpmbuild/RPMS/ -name $PKG_NAME-\*` || exit 1
    for rpm in ${rpms[@]}; do
        /bin/cp -f $rpm $BUILD_DIR/rpms-dir/
    done

    #find srpm package , and copy to rpms-dir
    srpms=`find $BUILD_DIR/rpmbuild/SRPMS/ -name $PKG_NAME-\*` || exit 1
    for srpm in ${srpms[@]}; do
        /bin/cp -f $srpm $BUILD_DIR/rpms-dir/
    done

    # clean rpmbuild dir
    rm -fr $BUILD_DIR/rpmbuild
}

build_rpm_package
