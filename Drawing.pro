QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawingwidget.cpp \
    editablelabel.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    tool.cpp

HEADERS += \
    drawingwidget.h \
    editablelabel.h \
    logindialog.h \
    mainwindow.h \
    myline.h \
    registerdialog.h \
    tool.h

FORMS += \
    logindialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    Drawing_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = drawicon.ico
