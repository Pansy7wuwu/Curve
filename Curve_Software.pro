QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = language
TRANSLATIONS = language_cn.ts\
                language_en.ts\
                language_tra_cn.ts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data_reader.cpp \
    main.cpp \
    curvefont.cpp \
    qcustomplot.cpp

HEADERS += \
    curvefont.h \
    data_reader.h \
    qcustomplot.h

FORMS += \
    curvefont.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    RES.qrc

DISTFILES += \
    language_cn.qm \
    language_en.qm
