#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T20:11:31
#
#-------------------------------------------------

QT       += core gui serialport testlib
CONFIG   += c++11

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QT.testlib.CONFIG -= console

INCLUDEPATH += C:/Qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

TARGET = Ciclop
TEMPLATE = app


SOURCES += main.cpp\
        Ciclop.cpp \
    ../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/UI/MDG_23_by_Geo_Stress_Test_Simple.cpp \
    ../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/MDG_23_by_Geo.cpp \
    ../ANGEL/Equipment/Monochromator/Monochromator/monochromator.cpp \
    ../ANGEL/Equipment/StepMotorController/SMSD_4_2/UI/SMSD_4_2_UI.cpp \
    ../ANGEL/Equipment/StepMotorController/SMSD_4_2/SMSD_4_2.cpp \
    ../ANGEL/Functions/AllFunctions.cpp

HEADERS  += Ciclop.h \
    ../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/UI/MDG_23_by_Geo_Stress_Test_Simple.h \
    ../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/MDG_23_by_Geo.h \
    ../ANGEL/Equipment/Monochromator/Monochromator/monochromator.h \
    ../ANGEL/Equipment/Monochromator/AllMonochromators.h \
    ../ANGEL/Equipment/StepMotorController/SMSD_4_2/UI/SMSD_4_2_UI.h \
    ../ANGEL/Equipment/StepMotorController/SMSD_4_2/SMSD_4_2.h \
    ../ANGEL/Equipment/StepMotorController/AllStepMotorControllers.h \
    ../ANGEL/Equipment/AllEquipment.h \
    ../ANGEL/Functions/AllFunctions.h \
    ../ANGEL/Angel.h

FORMS    += Ciclop.ui \
    ../ANGEL/Equipment/StepMotorController/SMSD_4_2/UI/SMSD_4_2_UI.ui \
    ../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/UI/MDG_23_by_Geo_Stress_Test_Simple.ui
