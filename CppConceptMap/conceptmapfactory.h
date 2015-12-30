//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPCONCEPTMAPFACTORY_H
#define CONCEPTMAPCONCEPTMAPFACTORY_H

//#include <array>
//#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include "conceptmap.h"
//#include "conceptmapnode.h"
//#include "conceptmapfwd.h"
//#include "conceptmapgraphtypes.h"
#include "conceptmapgraphfactory.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

#ifndef DO_NOT_USE_BOOST_GRAPH
using ConceptMapFactory = GraphFactory;
#else
///Factory class to create ConceptMaps
///ConceptMapFactory is the only class using the ConceptMap constructor
struct ConceptMapFactory
{
  using ConceptMaps = std::vector<ConceptMap>;

  ConceptMapFactory() noexcept;

  ConceptMap Create(const Graph& graph = Graph()) const noexcept;

  ///Obtain the main constituents of a
  ///ConceptMap from an XML std::string
  Graph FromXmlAsTuple(const std::string& s) const;

  ///Obtain a ConceptMap from an XML std::string
  ConceptMap FromXml(const std::string& s) const;

  ///Get all the other tests as one vector
  ConceptMaps GetTests() const noexcept;
  ConceptMap GetTest(const int index) const noexcept;
  int GetNumberOfTests() const noexcept { return 12; }
  ConceptMap GetEmptyConceptMap() const noexcept { return GetTest(0); }

  private:

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};
#endif

} //~namespace cmap

} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
