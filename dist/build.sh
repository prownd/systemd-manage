#!/usr/bin/bash
#########################################################################
# File Name: build.sh
# Author: hanjinpeng
# mail: hanjinpeng127@gmail.com
# Created Time: 2024-08-06 00:12:46
#########################################################################

PKG_NAME="systemd-manage"
PKG_VERSION="1.2"
PKG_VERSION_RELEASE="${PKG_VERSION}-1"
BUILD_DIR="$( cd "$( dirname "$0" )" && pwd  )"
TOP_DIR="$( cd "$( dirname "$0" )" && cd .. && pwd  )"
OS_ARCH="`arch`"

BUILD_TAR_NAME="${PKG_NAME}-${PKG_VERSION_RELEASE}-${OS_ARCH}"
BUILD_TAR_DIR="${BUILD_TAR_NAME}"

function  do_help()
{
     echo -e "\033[31m usage:\n $0  rpm|deb|tar \033[0m"
     echo -e "\033[31m example:\n $0   rpm \033[0m"
     exit 1
}

function build_rpm_package()
{
    #clean rpmbuild dir and regenerate rpmdev tree
    [ -d ${BUILD_DIR}/rpmbuild ] && rm -fr ${BUILD_DIR}/rpmbuild
    mkdir -p ${BUILD_DIR}/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
    mkdir -p ${BUILD_DIR}/rpms-dir

    #tar source file, generate xz archive 
    #tar -Jcvf ${BUILD_DIR}/rpmbuild/SOURCES/${PKG_NAME}-$PKG_VERSION.tar.xz  ${BUILD_DIR}/../../${PKG_NAME}
    tar -Jcv --exclude='.git' -f ${BUILD_DIR}/rpmbuild/SOURCES/${PKG_NAME}-${PKG_VERSION}.tar.xz  ${BUILD_DIR}/../../${PKG_NAME}

    #begin build
    rpmbuild --define "_topdir ${BUILD_DIR}/rpmbuild" -ba  ${BUILD_DIR}/rpm/${PKG_NAME}.spec

    #find package rpm file , and copy to rpms-dir
    rpms=`find ${BUILD_DIR}/rpmbuild/RPMS/ -name ${PKG_NAME}-\*` || exit 1
    for rpm in ${rpms[@]}; do
        /bin/cp -f $rpm ${BUILD_DIR}/rpms-dir/
    done

    #find srpm package , and copy to rpms-dir
    srpms=`find ${BUILD_DIR}/rpmbuild/SRPMS/ -name ${PKG_NAME}-\*` || exit 1
    for srpm in ${srpms[@]}; do
        /bin/cp -f $srpm ${BUILD_DIR}/rpms-dir/
    done

    # clean rpmbuild dir
    rm -fr ${BUILD_DIR}/rpmbuild
}


function build_deb_package()
{
    [ -d ${BUILD_DIR}/tmpbuilddir ] && rm -fr ${BUILD_DIR}/tmpbuilddir
    [ -d ${BUILD_DIR}/debbuild ] && rm -fr ${BUILD_DIR}/debbuild
    mkdir -p ${BUILD_DIR}/debbuild/{DEBIAN,}
    mkdir -p ${BUILD_DIR}/tmpbuilddir
    mkdir -p ${BUILD_DIR}/debs-dir

    pushd  ${BUILD_DIR}/tmpbuilddir 
    qmake $TOP_DIR/${PKG_NAME}.pro
    make -j `nproc`
    popd
 
    # copy control  and script
    install -m755 -p -D ${BUILD_DIR}/deb/* ${BUILD_DIR}/debbuild/DEBIAN/

    # copy binnary file
    install -d ${BUILD_DIR}/debbuild/usr/bin/
    install -m755 -p -D ${BUILD_DIR}/tmpbuilddir/${PKG_NAME}  ${BUILD_DIR}/debbuild/usr/bin/

    # copy .desktop file
    install -d ${BUILD_DIR}/debbuild/usr/share/applications/
    install -m644 -p -D $TOP_DIR/data/${PKG_NAME}.desktop  ${BUILD_DIR}/debbuild/usr/share/applications/${PKG_NAME}.desktop

    # copy icons
    install -d ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/icons/
    install -m644 -p -D $TOP_DIR/res/icons/* ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/icons/

    # copy translate file
    install -d ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/translations/
    install -m644 -p -D $TOP_DIR/res/translators/*.qm  ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/translations/

    # license file
    install -d ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/license/
    install -m644 -p -D ${TOP_DIR}/LICENSE  ${BUILD_DIR}/debbuild/usr/share/${PKG_NAME}/license/
    
    #build deb
    dpkg  -b ${BUILD_DIR}/debbuild ${BUILD_DIR}/debs-dir/${PKG_NAME}_${PKG_VERSION_RELEASE}_amd64.deb

    #clean tmp build dir
    [ -d ${BUILD_DIR}/debbuild ]  && /bin/rm -fr  ${BUILD_DIR}/debbuild 
    [ -d ${BUILD_DIR}/tmpbuilddir ] && /bin/rm -fr  ${BUILD_DIR}/tmpbuilddir 
}

function build_tar_package()
{
    [ -d ${BUILD_DIR}/tmpbuilddir ] && rm -fr ${BUILD_DIR}/tmpbuilddir
    [ -d ${BUILD_DIR}/tarbuild ] && rm -fr ${BUILD_DIR}/tarbuild
    mkdir -p ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}

    mkdir -p ${BUILD_DIR}/tmpbuilddir
    mkdir -p ${BUILD_DIR}/tar-dir

    pushd  ${BUILD_DIR}/tmpbuilddir 
    qmake-qt5 $TOP_DIR/${PKG_NAME}.pro
    make -j `nproc`
    popd
 
    # copy binnary file
    install -d  ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}
    install -m755 -p -D ${BUILD_DIR}/tmpbuilddir/${PKG_NAME}  ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/

    # copy .desktop file
    install -d ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/applications
    install -m644 -p -D $TOP_DIR/data/${PKG_NAME}.desktop  ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/applications/${PKG_NAME}.desktop

    # copy icons
    install -d ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/icons
    install -m644 -p -D $TOP_DIR/res/icons/* ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/icons/

    # license file
    install -d ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/license
    install -m644 -p -D ${TOP_DIR}/LICENSE ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/data/license

    # install.sh file
    install -d  ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}
    install -m755 -p -D ${TOP_DIR}/misc/install.sh  ${BUILD_DIR}/tarbuild/${BUILD_TAR_DIR}/
    
    #build tar
    cd  ${BUILD_DIR}/tarbuild/
    tar -Jcvf ${BUILD_DIR}/tar-dir/${BUILD_TAR_NAME}-unknown-linux-gnu.tar.xz  ${BUILD_TAR_DIR}

    #clean tmp build dir
    [ -d ${BUILD_DIR}/tarbuild ]  && /bin/rm -fr  ${BUILD_DIR}/tarbuild 
    [ -d ${BUILD_DIR}/tmpbuilddir ] && /bin/rm -fr  ${BUILD_DIR}/tmpbuilddir 
}

if [ $# != 1 ]; then
    do_help
fi

if [ x"$1"  = x"rpm" ];then
    #build rpm
    build_rpm_package
elif [ x"$1"  = x"deb" ];then
    #build deb
    build_deb_package
elif [ x"$1"  = x"tar" ];then
    #build tar
    build_tar_package
else
    do_help
    exit 1
fi
