include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
include(../RibiLibraries/GeneralConsole.pri)

include(CppFileIo/CppFileIoTest.pri)
include(CppAbout/CppAboutTest.pri)

SOURCES += main_test.cpp

LIBS += -lboost_unit_test_framework
