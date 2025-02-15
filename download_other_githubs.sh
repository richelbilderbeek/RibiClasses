#!/bin/bash	
cd ..

if [ ! -d boost_graph_cookbook_1 ]; then
  git clone https://github.com/richelbilderbeek/boost_graph_cookbook_1
fi

if [ ! -d plane ]; then
  git clone https://github.com/richelbilderbeek/plane
fi

if [ ! -d RibiClasses ]; then
  git clone https://github.com/richelbilderbeek/RibiClasses
fi

if [ ! -d RibiLibraries ]; then
  git clone https://github.com/richelbilderbeek/RibiLibraries
fi


