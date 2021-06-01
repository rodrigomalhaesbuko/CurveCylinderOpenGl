QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCCTest.pro
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    polygonalmoulder.h \
    renderwidget.h

SOURCES += \
    polygonalmoulder.cpp \
    mainwindow.cpp \
    main.cpp \
    renderwidget.cpp

INCLUDEPATH += $$PWD/eigen/
DEPENDPATH  += $$PWD/eigen/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QtResources.qrc
