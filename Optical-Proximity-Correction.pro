QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Windows 特定配置
win32 {
    # 海康相机
    3DPARTY_DIR = ../3dParty
    INCLUDEPATH += $$3DPARTY_DIR/MVS/Development/Includes
    LIBS += -L$$3DPARTY_DIR/MVS/Development/Libraries/win64 -lMvCameraControl


    CONFIG(debug, debug|release) {
        INCLUDEPATH += $$3DPARTY_DIR/opencv-4.10.0-windows/Debug-x64/include
        INCLUDEPATH += $$3DPARTY_DIR/libharu-2.4.4/Debug-x64/include
        INCLUDEPATH += $$3DPARTY_DIR/json-3.11.3/Debug-x64/include
        LIBS += -L$$3DPARTY_DIR/opencv-4.10.0-windows/Debug-x64/x64/vc17/lib -lopencv_world4100d
        LIBS += -L$$3DPARTY_DIR/libharu-2.4.4/Debug-x64/lib -lhpdf
    }
    else{
        INCLUDEPATH += $$3DPARTY_DIR/opencv-4.10.0-windows/Debug-x64/include
        INCLUDEPATH += $$3DPARTY_DIR/libharu-2.4.4/Release-x64/include
        INCLUDEPATH += $$3DPARTY_DIR/json-3.11.3/Release-x64/include
        LIBS += -L$$3DPARTY_DIR/opencv-4.10.0-windows/Release-x64/x64/vc17/lib -lopencv_world4100
        LIBS += -L$$3DPARTY_DIR/libharu-2.4.4/Release-x64/lib -lhpdf
    }
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataStructure/Camera/MCameraBase.cpp \
    DataStructure/Camera/MCameraFactory.cpp \
    DataStructure/Camera/MCameraHik.cpp \
    DataStructure/Camera/MCameraHikFactory.cpp \
    DataStructure/MImageProcess.cpp \
    DataStructure/MSystemInfo.cpp \
    DataStructure/ThreadManager.cpp \
    DataStructure/MExportPDF.cpp \
    Dialog/ConfigurationWidget.cpp \
    Dialog/ImageProcessWidget.cpp \
    Dialog/ReportWidget.cpp \
    Dialog/mainwindow.cpp \
    ROILabel.cpp \
    ZoomableLabel.cpp \
    main.cpp

HEADERS += \
    DataStructure/Camera/MCameraBase.h \
    DataStructure/Camera/MCameraFactory.h \
    DataStructure/Camera/MCameraHik.h \
    DataStructure/Camera/MCameraHikFactory.h \
    DataStructure/MCommonBase.h \
    DataStructure/MImageProcess.h \
    DataStructure/MSystemInfo.h \
    DataStructure/ThreadManager.h \
    DataStructure/MExportPDF.h \
    Dialog/ConfigurationWidget.h \
    Dialog/ImageProcessWidget.h \
    Dialog/ReportWidget.h \
    Dialog/mainwindow.h \
    ROILabel.h \
    ZoomableLabel.h


FORMS += \
    Dialog/configurationwidget.ui \
    Dialog/imageprocesswidget.ui \
    Dialog/reportwidget.ui \
    Dialog/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
