# -------------------------------------------------
# Project created by QtCreator 2011-12-18T00:13:30
# -------------------------------------------------
QT += opengl \
    xml
TARGET = FreeDO
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    OutSound.cpp \
    joyconfig.cpp \
    inputedit.cpp \
    inputconf.cpp \
    glwidget.cpp \
    emuthread.cpp \
    Joy/EventDispatcher.cpp \
    core/Madam.cpp \
    core/Iso.cpp \
    core/DiagPort.cpp \
    core/Clio.cpp \
    core/bitop.cpp \
    core/arm.cpp \
    core/_3do_sys.cpp \
    core/XBUS.cpp \
    core/vdlp.cpp \
    core/SPORT.cpp \
    core/quarz.cpp \
    core/DSP.cpp
HEADERS += mainwindow.h \
    OutSound.h \
    joyconfig.h \
    inputedit.h \
    inputconf.h \
    glwidget.h \
    emuthread.h \
    Joy/Gamepad.h \
    Joy/EventDispatcher.h \
    cd/discdata.h \
    cd/bigend_types.h \
    core/Madam.h \
    core/IsoXBUS.h \
    core/freedocore.h \
    core/DiagPort.h \
    core/Clio.h \
    core/bitop.h \
    core/arm.h \
    core/XBUS.h \
    core/vdlp.h \
    core/types.h \
    core/SPORT.h \
    core/quarz.h \
    core/DSP.h
FORMS += mainwindow.ui \
    joyconfig.ui \
    inputedit.ui \
    inputconf.ui
RESOURCES += fiches.qrc


win32{
    contains(QMAKE_HOST.arch, x86_64) {
        LIBS += ../FreeDO/al/libs/win64/OpenAL32.lib
    }else{
        LIBS += ../FreeDO/al/libs/win32/OpenAL32.lib
    }
}

win32 {

    LIBS += -lwinmm

    SOURCES += joy/Gamepad_windows.cpp
}

linux {
    LIBS += -lopenal -ldl -lGLU

    SOURCES += joy/Gamepad_linux.cpp
}

