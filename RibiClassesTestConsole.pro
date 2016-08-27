#INCLUDEPATH += C:/Boost/include
#DEPENDPATH += C:/Boost/include

#INCLUDEPATH += C:/Boost
#LIBS += C:/Boost/stage/lib/libboost_filesystem-mgw49-mt-1_59.a
#LIBS += C:/Boost/stage/lib/libboost_system-mgw49-mt-d-1_59.a
#LIBS += C:/Boost/stage/lib/libboost_graph-mgw49-mt-d-1_59.a

include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/GeneralConsoleTest.pri)
include(../RibiLibraries/Apfloat.pri)
include(../RibiLibraries/BoostAll.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

include(../RibiClasses/CppAbcFile/CppAbcFile.pri)
include(../RibiClasses/CppAbcFile/CppAbcFileTest.pri)

include(../RibiClasses/CppBiology/CppBiology.pri)
include(../RibiClasses/CppBiology/CppBiologyTest.pri)

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

include(../RibiClasses/CppMusic/CppMusic.pri)

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
include(../RibiClasses/CppStopwatch/CppStopwatch.pri)
include(../RibiClasses/CppStopwatch/CppStopwatchTest.pri)
include(../RibiClasses/CppXml/CppXml.pri)
include(../RibiClasses/CppXml/CppXmlTest.pri)

SOURCES += main_test_console.cpp

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

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable
