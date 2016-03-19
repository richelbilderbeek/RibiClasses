include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
include(../RibiLibraries/GeneralConsole.pri)

include(CppAbout/CppAboutTest.pri)
include(CppFileIo/CppFileIoTest.pri)
include(CppHelp/CppHelpTest.pri)

SOURCES += main_test.cpp

LIBS += -lboost_unit_test_framework
