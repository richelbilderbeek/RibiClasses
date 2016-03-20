#!/bin/bash
cd ..
git clone https://github.com/richelbilderbeek/RibiLibraries
cd RibiClasses
qmake RibiClassesTest.pro
make
./RibiClassesTest
gcov-5 main_test.cpp
#gcov-5 my_functions.cpp
# Create gcov files
#for filename in `ls *.cpp`; do gcov $filename; done
#for filename in `ls *.h`; do gcov $filename; done

# Display gcov files
#for filename in `ls *.h.gcov`; do cat $filename; done
#for filename in `ls *.cpp.gcov`; do cat $filename; done

