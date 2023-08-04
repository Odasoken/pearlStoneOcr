
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17




SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h


ICON = logo.icns




FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target







macx: LIBS += -L$$PWD/leptonica/ -llept.5

INCLUDEPATH += $$PWD/leptonica
DEPENDPATH += $$PWD/leptonica

macx: LIBS += -L$$PWD/tesseract/ -ltesseract.5

INCLUDEPATH += $$PWD/tesseract
DEPENDPATH += $$PWD/tesseract

DISTFILES += \
    tessdata/chi_sim.traineddata

RESOURCES += \
    ress.qrc
