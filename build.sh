#!/bin/bash
cd ..
git clone https://github.com/richelbilderbeek/RibiLibraries
cd RibiClasses
qmake RibiClassesTest.pro
make
./RibiClassesTest
#gcov-5 main_test.cpp
#gcov-5 my_functions.cpp
# Create gcov files
for filename in `find . -name '*.cpp'`; do gcov-5 $filename; done
for filename in `find . -name '*.h'`; do gcov-5 $filename; done
# Display gcov files
for filename in `find . -name '*.cpp.gcov'`; do cat $filename; done
for filename in `find . -name '*.cpp.gcov'`; do cat $filename; done

