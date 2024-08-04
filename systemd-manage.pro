QT       += core gui dbus
QT       += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/

SOURCES += \
    src/aboutwindow.cpp \
    src/boottimes.cpp \
    src/configfile.cpp \
    src/configfilewindow.cpp \
    src/customcanttchartscene.cpp \
    src/customcanttchartview.cpp \
    src/customcantttimebaritem.cpp \
    src/customlistwidgetitem.cpp \
    src/customsortfilterproxymodel.cpp \
    src/customsvggraphicsview.cpp \
    src/highlighter.cpp \
    src/journalwindow.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/mystyleditemdelegate.cpp \
    src/qprocessinfo.cpp \
    src/qprocessinfowindow.cpp \
    src/settingswindow.cpp \
    src/systemdjournalinterface.cpp \
    src/systemdmanagerinterface.cpp \
    src/unitinformation.cpp \
    src/unitwindow.cpp \
    src/usersessionwindow.cpp \
    src/systemdgeneric.cpp \
    src/unitmodel.cpp \
    src/usersessioninformation.cpp \
    src/configfileinformation.cpp \
    src/analyzewindow.cpp

HEADERS += \
    src/aboutwindow.h \
    src/boottimes.h \
    src/configfile.h \
    src/configfilewindow.h \
    src/customcanttchartscene.h \
    src/customcanttchartview.h \
    src/customcantttimebaritem.h \
    src/customlistwidgetitem.h \
    src/customsortfilterproxymodel.h \
    src/customsvggraphicsview.h \
    src/globalvar.h \
    src/highlighter.h \
    src/journalwindow.h \
    src/mainwidget.h \
    src/mystyleditemdelegate.h \
    src/qprocessinfo.h \
    src/qprocessinfowindow.h \
    src/settingswindow.h \
    src/systemdjournalinterface.h \
    src/systemdmanagerinterface.h \
    src/unitinformation.h \
    src/unitwindow.h \
    src/usersessionwindow.h \
    src/unitmodel.h \
    src/systemdgeneric.h \
    src/configfileinformation.h \
    src/usersessioninformation.h \
    src/analyzewindow.h \
    src/version.h

# Default rules for deployment.
target.path = /usr/bin
INSTALLS += target

RESOURCES += \
    res/resources.qrc

TRANSLATIONS += \
    res/translators/app_zh_CN.ts \
    res/translators/app_en_US.ts \
    res/translators/app_de_DE.ts \
    res/translators/app_ru_RU.ts \
    res/translators/app_fr_FR.ts \
    res/translators/app_ja_JP.ts \
    res/translators/app_zh_TW.ts

LIBS += -lsystemd

DISTFILES +=

TARGET=systemd-manage

VERSION = 1.0

RC_ICONS = "res/icons/systemd-manage.ico"
QMAKE_TARGET_COMPANY = "hanjinpeng <hanjinpeng127@gmail.com>"
QMAKE_TARGET_PRODUCT = "systemd-manage"
QMAKE_TARGET_DESCRIPTION = "Qt Creator based on Qt 5.15"
QMAKE_TARGET_COPYRIGHT = "hanjinpeng <hanjinpeng127@gmail.com>"
