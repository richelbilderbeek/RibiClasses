include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/GeneralConsoleTest.pri)
include(../RibiLibraries/BoostAll.pri)
include(../RibiLibraries/Apfloat.pri)
include(../RibiLibraries/Rinside.pri)

include(../plane/plane.pri)

include(../RibiClasses/CppBeast/CppBeast.pri)
include(../RibiClasses/CppBeast/CppBeastTest.pri)

include(../RibiClasses/CppFastaFile/CppFastaFile.pri)
include(../RibiClasses/CppFastaFile/CppFastaFileTest.pri)

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

include(../RibiClasses/CppRibiRinside/CppRibiRinside.pri)

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

# Boost Graph Tutorial
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)

#
SOURCES += main_test_console.cpp

# C++14
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# High warning levels
# -Wshadow goes bad with apfloat
QMAKE_CXXFLAGS += -Wall -Wextra -Wnon-virtual-dtor -pedantic -Werror

# Debug and release mode
CONFIG += debug_and_release
CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}

CONFIG(debug, debug|release) {

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # UBSAN
  QMAKE_CXXFLAGS += -fsanitize=undefined
  QMAKE_LFLAGS += -fsanitize=undefined
  LIBS += -lubsan
}

# Boost.Test
LIBS += -lboost_unit_test_framework

# Boost.Graph
LIBS += \
  -lboost_date_time \
  -lboost_graph \
  -lboost_regex

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable

# Fixes
#/usr/include/boost/math/constants/constants.hpp:277: error: unable to find numeric literal operator 'operator""Q'
#   BOOST_DEFINE_MATH_CONSTANT(half, 5.000000000000000000000000000000000000e-01, "5.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e-01")
#   ^
QMAKE_CXXFLAGS += -fext-numeric-literals

message(Host name: $$QMAKE_HOST.name)
contains(QMAKE_HOST.name,pc-157-103) {
  message("Host is university computer in the canteen")
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
}
