QT += core
QT -= gui

CONFIG += c++11

TARGET = switchDetect3_1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    square.cpp \
    switchdetect.cpp \
    switchstatedetect.cpp

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc -lopencv_objdetect -lopencv_features2d -lopencv_xfeatures2d -lopencv_imgcodecs -lopencv_flann -lopencv_calib3d

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    square.h \
    switchdetect.h \
    switchstatedetect.h
