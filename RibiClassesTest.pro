
include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/GeneralConsoleTest.pri)
include(../RibiLibraries/Apfloat.pri)
#include(../RibiLibraries/BoostAll.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

include(../RibiClasses/CppContainer/CppContainer.pri)
include(../RibiClasses/CppContainer/CppContainerTest.pri)
include(../RibiClasses/CppCounter/CppCounter.pri)
include(../RibiClasses/CppCounter/CppCounterTest.pri)
include(../RibiClasses/CppConceptMap/CppConceptMap.pri)
include(../RibiClasses/CppConceptMap/CppConceptMapTest.pri)
include(../RibiClasses/CppDnaSequence/CppDnaSequence.pri)
include(../RibiClasses/CppDnaSequence/CppDnaSequenceTest.pri)
include(../RibiClasses/CppGeometry/CppGeometry.pri)
include(../RibiClasses/CppGeometry/CppGeometryTest.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_toTest.pri)
include(../RibiClasses/CppPlane/CppPlane.pri)
include(../RibiClasses/CppPlane/CppPlaneTest.pri)
include(../RibiClasses/CppRibiRandom/CppRibiRandom.pri)
include(../RibiClasses/CppRibiRandom/CppRibiRandomTest.pri)
include(../RibiClasses/CppRibiRegex/CppRibiRegex.pri)
include(../RibiClasses/CppRibiRegex/CppRibiRegexTest.pri)
include(../RibiClasses/CppRibiSystem/CppRibiSystem.pri)
include(../RibiClasses/CppRibiSystem/CppRibiSystemTest.pri)
include(../RibiClasses/CppRibiTime/CppRibiTime.pri)
include(../RibiClasses/CppRibiTime/CppRibiTimeTest.pri)
include(../RibiClasses/CppXml/CppXml.pri)
include(../RibiClasses/CppXml/CppXmlTest.pri)

SOURCES += main_test.cpp

# Boost.Test
LIBS += -lboost_unit_test_framework

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Graph
LIBS += \
  -lboost_date_time \
  -lboost_graph \
  -lboost_regex
