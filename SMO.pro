QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Desktop/Polytech/SMO/src/Application.cpp \
    ../Desktop/Polytech/SMO/src/Buffer.cpp \
    ../Desktop/Polytech/SMO/src/FunctionalModule.cpp \
    ../Desktop/Polytech/SMO/src/Handler.cpp \
    ../Desktop/Polytech/SMO/src/RingAdd.cpp \
    ../Desktop/Polytech/SMO/src/RingSelection.cpp \
    ../Desktop/Polytech/SMO/src/Source.cpp \
    ../Desktop/Polytech/SMO/src/TimeBehaviour.cpp \
    load.cpp \
    main.cpp \
    mainwindow.cpp \
    runwindow.cpp \
    stepwindow.cpp \


HEADERS += \
    ../Desktop/Polytech/SMO/headers/AdditionStrategy.hpp \
    ../Desktop/Polytech/SMO/headers/Application.hpp \
    ../Desktop/Polytech/SMO/headers/Buffer.hpp \
    ../Desktop/Polytech/SMO/headers/FunctionalModule.hpp \
    ../Desktop/Polytech/SMO/headers/Handler.hpp \
    ../Desktop/Polytech/SMO/headers/RingSelection.hpp \
    ../Desktop/Polytech/SMO/headers/SelectionStrategy.hpp \
    ../Desktop/Polytech/SMO/headers/Source.hpp \
    ../Desktop/Polytech/SMO/headers/TimeBehaviour.hpp \
    ../Desktop/Polytech/SMO/headers/RingAdd.h \
    mainwindow.h \
    runwindow.h \
    stepwindow.h \
    step.hpp \


FORMS += \
    mainwindow.ui \
    runwindow.ui \
    stepwindow.ui \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
