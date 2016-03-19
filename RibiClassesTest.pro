include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
include(../RibiLibraries/GeneralConsole.pri)

include(CppAbout/CppAboutTest.pri)
include(CppFileIo/CppFileIoTest.pri)
include(CppHelp/CppHelpTest.pri)
include(CppMenuDialog/CppMenuDialogTest.pri)

include(../RibiClasses/CppContainer/CppContainer.pri)
include(../RibiClasses/CppContainer/CppContainerTest.pri)

include(../RibiClasses/CppCounter/CppCounter.pri)
include(../RibiClasses/CppCounter/CppCounterTest.pri)
#include(../RibiClasses/CppConceptMap/CppConceptMap.pri)
#include(../RibiClasses/CppGeometry/CppGeometry.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_toTest.pri)
#include(../RibiClasses/CppPlane/CppPlane.pri)
#include(../RibiClasses/CppRibiRegex/CppRibiRegex.pri)
include(../RibiClasses/CppXml/CppXml.pri)
include(../RibiClasses/CppXml/CppXmlTest.pri)


SOURCES += main_test.cpp

LIBS += -lboost_unit_test_framework
