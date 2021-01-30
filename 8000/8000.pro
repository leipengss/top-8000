#-------------------------------------------------
#
# Project created by QtCreator 2018-11-07T17:25:08
#
#-------------------------------------------------
include("/opt/pvb/halcmd.pri")

QT       += core gui
QT       += xml
QT       += sql
QT       +=concurrent network
TRANSLATIONS += translator_CH.ts \
                translator_EN.ts \
                translator_FAN.ts\
                translator_Korea.ts\
                translator_Japan.ts\
                translator_Vietnam.ts



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nn
TEMPLATE = app

QMAKE_LFLAGS +=-Wl,-rpath=/usr/share/runf/toplib/
DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += main.cpp\
        mainwindow.cpp \
    UI/dlgmaterialmanage.cpp \
    driver/libdriver.cpp \
    UI/cdlgmessage.cpp \
    UI/dlgworkpiece.cpp \
    UI/dlgsystemcheck.cpp \
    UI/dlgsuperparasetting.cpp \
    UI/dlglogin.cpp \
    UI/dlglightzero.cpp \
    UI/dlgioparameter.cpp \
    UI/dlginputkey.cpp \
    UI/dlghome.cpp \
    UI/dlgcncparmeter.cpp \
    UI/dlgchangepassword.cpp \
    UI/dlgbgparameter.cpp \
    UI/cxmlparmetershow.cpp \
    UI/clogfileshow.cpp \
    UI/cinioutfile.cpp \
    UI/cinifileshow.cpp \
    filemanage/cxmlparameter.cpp \
    filemanage/cinputrangecheck.cpp \
    filemanage/cinirw.cpp \
    filemanage/cfileoperate.cpp \
    driver/cupdatedatathread.cpp \
    UI/dlgregister.cpp \
    AES/qaesencryption.cpp \
    driver/libregister.cpp \
    function/cbaoguangflow.cpp \
    function/cscreenprotect.cpp \
    UI/dlglanguage.cpp \
    UI/dlgkeyboard.cpp \
    UI/dlgabselect.cpp \
    UI/caxispositionshow.cpp \
    dlgparmeter.cpp \
    dlgmove.cpp

HEADERS  += mainwindow.h \
    UI/dlgmaterialmanage.h \
    driver/libdriver.h \
    UI/cdlgmessage.h \
    UI/dlgworkpiece.h \
    UI/dlgsystemcheck.h \
    UI/dlgsuperparasetting.h \
    UI/dlglogin.h \
    UI/dlglightzero.h \
    UI/dlgioparameter.h \
    UI/dlginputkey.h \
    UI/dlghome.h \
    UI/dlgcncparmeter.h \
    UI/dlgchangepassword.h \
    UI/dlgbgparameter.h \
    UI/cxmlparmetershow.h \
    UI/clogfileshow.h \
    UI/cinioutfile.h \
    UI/cinifileshow.h \
    filemanage/cxmlparameter.h \
    filemanage/cinputrangecheck.h \
    filemanage/cinirw.h \
    filemanage/cfileoperate.h \
    driver/global.h \
    driver/cupdatedatathread.h \
    driver/CncWidget.h \
    UI/dlgregister.h \
    AES/qaesencryption.h \
    driver/cncsh.hh \
    function/cbaoguangflow.h \
    function/cscreenprotect.h \
    UI/dlglanguage.h \
    UI/dlgkeyboard.h \
    UI/dlgabselect.h \
 #   modbus/libmodbus.h \
    modbus/inkRTU.h \
    modbus/ui_about.h \
    modbus/modbus.h \
    modbus/modbus-version.h \
    modbus/modbus-tcp.h \
    modbus/modbus-rtu.h \
    modbus/modbus-ascii.h \
    modbus/mainwindowRTU.h \
    modbus/lightRTU.h \
    modbus/mainwindowTCP.h \
    modbus/light.h \
    UI/caxispositionshow.h \
    dlgparmeter.h \
    dlgmove.h \

FORMS    += UI/mainwindow.ui \
            UI/workpiece.ui \
            UI/systemcheck.ui \
            UI/home.ui \
    UI/ioparameter.ui \
    UI/cinifileshow.ui \
    UI/dlginputkey.ui \
    UI/changepassword.ui \
    UI/dlglogin.ui \
    UI/bgparameter.ui \
    UI/dlgsuperparasetting.ui \
    UI/cinioutfile.ui \
    UI/dlgmaterialmanage.ui \
    UI/cdlgmessage.ui \
    UI/dlglightzero.ui \
    UI/dlgcncparmeter.ui \
    UI/cxmlparmetershow.ui \
    UI/clogfileshow.ui \
    UI/dlgregister.ui \
    UI/dlglanguage.ui \
    UI/dlgkeyboard.ui \
    UI/dlgabselect.ui \
    UI/caxispositionshow.ui \
    dlgparmeter.ui \
    dlgmove.ui
RESOURCES += \
    res.qrc \

#add lib
#LIBS += -L./libdriver/ -ldriver

INCLUDEPATH += ./libdriver
DEPENDPATH += ./libdriver
INCLUDEPATH += /opt/pvb/
LIBS += /usr/lib/libCncWidget.so  /usr/lib/libpvcnc.so
LIBS += /usr/lib/libmarkWidget.so

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lModbus
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lModbus
#else:symbian: LIBS += -lModbus
#else:unix: LIBS += -L$$PWD/ -lModbus

#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/






win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -llightSource8000_RTUTCP
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -llightSource8000_RTUTCP
else:symbian: LIBS += -llightSource8000_RTUTCP
else:unix: LIBS += -L$$PWD/ -llightSource8000_RTUTCP

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
