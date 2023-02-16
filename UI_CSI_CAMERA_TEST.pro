#-------------------------------------------------
#
# Project created by QtCreator 2022-05-07T14:53:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI_CSI_CAMERA_TEST
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/include/opencv4 \ #添加头文件路径
                /usr/local/include
LIBS += -L/usr/lib/aarch64-linux-gnu -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio  \  #添加需要链接的库
        -L/usr/local/lib/lib* \
        -L/usr/local/lib
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    camera_gpio.cpp \
    mythread.cpp

HEADERS += \
        mainwindow.h \
    camera_gpio.h \
    mythread.h

FORMS += \
        mainwindow.ui
CONFIG += CONSOLE
QMAKE_LFLAGS += -no-pie

#gpio
LIBS += /usr/local/lib/lib*  \
        -L/usr/local/lib
INCLUDEPATH += /usr/local/include


