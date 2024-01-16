QT       += core gui
QT +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosewidget.cpp \
    jbf293kparser.cpp \
    loginwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mqttclient.cpp \
    yiaiparser.cpp

HEADERS += \
    choosewidget.h \
    jbf293kparser.h \
    loginwidget.h \
    mainwindow.h \
    mqttclient.h \
    yiaiparser.h

FORMS += \
    choosewidget.ui \
    loginwidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 添加字体文件
OTHER_FILES += fonts/iconfont.ttf

RESOURCES += \
    pic/pic.qrc


QT += serialport mqtt
