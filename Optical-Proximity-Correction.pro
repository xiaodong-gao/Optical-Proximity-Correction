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
    INCLUDEPATH += D:/codes/3dParty/MVS/Development/Includes
    LIBS += D:/codes/3dParty/MVS/Development/Libraries/win64/MvCameraControl.lib
    CONFIG(debug, debug|release) {
       INCLUDEPATH += D:/tools/opencv-4.10.0-windows/build/include
       LIBS += D:/tools/opencv-4.10.0-windows/build/x64/vc16/lib/opencv_world4100d.lib
       INCLUDEPATH += D:/codes/3dParty/libharu-2.4.4/Debug-x64/include
       LIBS += D:/codes/3dParty/libharu-2.4.4/Debug-x64/lib/hpdf.lib
    }
    else{
        INCLUDEPATH += D:/tools/opencv-4.10.0-windows/build/include
        LIBS += D:/tools/opencv-4.10.0-windows/build/x64/vc16/lib/opencv_world4100.lib
        INCLUDEPATH += D:/codes/3dParty/libharu-2.4.4/Release-x64/include
        LIBS += D:/codes/3dParty/libharu-2.4.4/Release-x64/lib/hpdf.lib
    }
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataStructure/MCameraBase.cpp \
    DataStructure/MCameraFactory.cpp \
    DataStructure/MCameraHik.cpp \
    DataStructure/MCameraHikFactory.cpp \
    DataStructure/MImageProcess.cpp \
    DataStructure/MSystemInfo.cpp \
    DataStructure/ThreadManager.cpp \
    DataStructure/MExportPDF.cpp \
    Dialog/ConfigurationWidget.cpp \
    Dialog/ImageProcessWidget.cpp \
    Dialog/ReportWidget.cpp \
    OpticalToolDlg.cpp \
    ROILabel.cpp \
    ZoomableLabel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DataStructure/MCameraBase.h \
    DataStructure/MCameraFactory.h \
    DataStructure/MCameraHik.h \
    DataStructure/MCameraHikFactory.h \
    DataStructure/MCommonBase.h \
    DataStructure/MImageProcess.h \
    DataStructure/MSystemInfo.h \
    DataStructure/ThreadManager.h \
    DataStructure/MExportPDF.h \
    Dialog/ConfigurationWidget.h \
    Dialog/ImageProcessWidget.h \
    Dialog/ReportWidget.h \
    OpticalToolDlg.h \
    ROILabel.h \
    ZoomableLabel.h \
    mainwindow.h

FORMS += \
    Dialog/configurationwidget.ui \
    Dialog/imageprocesswidget.ui \
    Dialog/reportwidget.ui \
    opticaltooldlg.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
