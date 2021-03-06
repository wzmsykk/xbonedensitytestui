QT += quick qml svg
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += main.cpp \
    initc.cpp \
    scan.cpp \
    settings.cpp \
    svgedit.cpp


RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
TRANSLATIONS = zh_cn.ts
DISTFILES += zh_cn.ts \
    zh_cn.ts \
    input_data \
    lcdroi \
    lcd \
    pages/NVRamSet2.qml

HEADERS += \
    initc.h \
    scan.h \
    settings.h \
    svgedit.h \
    lib/createbmp.h \
    lib/generatelcd.h


unix|win32: LIBS += -L$$PWD/lib/ -lcreatebmp32

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/libcreatebmp32.a
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/libcreatebmp32.a
