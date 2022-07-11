QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += sdk_no_version_check
DEFINES += HV_SOURCE
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES -= UNICODE _UNICODE

INCLUDEPATH += \
    3rd/include \
    src \
    src/qt \
    src/ui \
    src/video \
    src/util \
    src/win32 \
    src/hv \
    src/hv/base \
    src/hv/cpputil \
    src/hv/utils \
    src/hv/http \
    src/hv/http/client \
    src/hv/ssl

SOURCES += \
    src/main.cpp \
    src/ui/centralwidget.cpp \
    src/ui/hmultiview.cpp \
    src/ui/hopenmediadlg.cpp \
    src/ui/htable.cpp \
    src/ui/hvideotitlebar.cpp \
    src/ui/hvideotoolbar.cpp \
    src/ui/hvideowidget.cpp \
    src/ui/hvideownd.cpp \
    src/ui/lsidewidget.cpp \
    src/ui/mainwindow.cpp \
    src/ui/rsidewidget.cpp \
    src/ui/sdl2wnd.cpp \
    src/util/hframe.cpp \
    src/video/haudio.cpp \
    src/video/hffplayer.cpp \
#    src/win32/hdevice.cpp

HEADERS += \
    src/appdef.h \
    src/avdef.h \
    src/confile.h \
    src/qt/qtfunctions.h \
    src/qt/qtheaders.h \
    src/ui/HVideoWndFactory.h \
    src/ui/QCustomEvent.h \
    src/ui/centralwidget.h \
    src/ui/hmultiview.h \
    src/ui/hopenmediadlg.h \
    src/ui/htable.h \
    src/ui/hvideotitlebar.h \
    src/ui/hvideotoolbar.h \
    src/ui/hvideowidget.h \
    src/ui/hvideownd.h \
    src/ui/lsidewidget.h \
    src/ui/mainwindow.h \
    src/ui/qtstyles.h \
    src/ui/rsidewidget.h \
    src/ui/sdl2wnd.h \
    src/util/ffmpeg_util.h \
    src/util/hframe.h \
    src/util/sdl_util.h \
    src/video/HVideoPlayer.h \
    src/video/HVideoPlayerFactory.h \
    src/video/haudio.h \
    src/video/hffplayer.h \
    src/video/hmedia.h \
#    src/win32/hdevice.h

TRANSLATIONS += rc/lang/app_zh_CN.ts rc/lang/app_zh_CN.qm rc/lang/qt_zh_CN.ts rc/lang/qt_zh_CN.qm

HEADERS += \
    src/hv/hv.h \
    src/hv/base/hplatform.h \
    src/hv/base/hdef.h \
    src/hv/base/hbase.h \
    src/hv/base/hversion.h \
    src/hv/base/htime.h \
    src/hv/base/herr.h \
    src/hv/base/hbuf.h \
    src/hv/base/hlog.h \
    src/hv/base/hscope.h \
    src/hv/cpputil/hstring.h \
    src/hv/base/hmutex.h \
    src/hv/base/hthread.h \
    src/hv/cpputil/singleton.h \
    src/hv/cpputil/iniparser.h \

SOURCES += \
    src/hv/base/hversion.c \
    src/hv/base/hbase.c \
    src/hv/base/herr.c \
    src/hv/base/htime.c \
    src/hv/base/hlog.c \
    src/hv/cpputil/hstring.cpp \
    src/hv/cpputil/iniparser.cpp \

RESOURCES += rc/skin.qrc rc/image.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lavcodec

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lavdevice

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lavfilter

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lavformat

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lavutil

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lpostproc

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lSDL2

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lswresample

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include

unix|win32: LIBS += -L$$PWD/3rd/lib/ -lswscale

INCLUDEPATH += $$PWD/3rd/include
DEPENDPATH += $$PWD/3rd/include
