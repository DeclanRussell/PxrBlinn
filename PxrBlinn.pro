#-------------------------------------------------
#
# Project created by Declan Russell 2016-03-22T13:47:09
#
#-------------------------------------------------

QT       -= core gui

TARGET = PxrBlinn
TEMPLATE = lib

# Somethings are changed in the new renderman 21 so define this to make compatible
DEFINES += RENDERMAN21

OBJECTS_DIR = "obj"
MOC_DIR = "moc"

INCLUDEPATH += $$(RMANTREE)include  "$$PWD"/include

QMAKE_LIBDIR += $$(RMANTREE)\lib

LIBS += -llibprman
defined(RENDERMAN21)
{
    LIBS += -llibrix
}

win32:DEFINES += WIN32
DEFINES += _USE_MATH_DEFINES

HEADERS += include/*.h

SOURCES += src/PxrBlinn.cpp

OTHER_FILES += *.args

win32:{
    QMAKE_CXXFLAGS += -nologo -MT
    INCLUDEPATH+=C:/boost
}
