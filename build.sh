#!/bin/bash
cd ..
git clone https://github.com/richelbilderbeek/RibiLibraries
cd RibiClasses
qmake RibiClassesTest.pro
make
./RibiClassesTest