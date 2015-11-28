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

#include <array>
#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {

namespace cmap {

///Factory class to create ConceptMaps
///ConceptMapFactory is the only class using the ConceptMap constructor
struct ConceptMapFactory
{
  using Nodes = std::vector<Node>;
  using Edges = std::vector<Edge>;
  using ConceptMaps = std::vector<ConceptMap>;

  ConceptMapFactory() noexcept;

  ConceptMap Create();

  ConceptMap Create(
    Nodes& nodes,
    Edges& edges
  ) const noexcept;

  ///Obtain the main constituents of a
  ///ConceptMap from an XML std::string
  std::tuple<
    std::vector<Edge>,
    std::vector<Node>
  > FromXmlAsTuple(const std::string& s) const;

  ///Obtain a ConceptMap from an XML std::string
  ConceptMap FromXml(const std::string& s) const;

  ///Get all the other tests as one vector
  ConceptMaps GetAllTests() const noexcept;
  int GetNumberOfAllTests() const noexcept { return 39; }

  ConceptMap GetEmptyConceptMap() const noexcept { return GetHeteromorphousTestConceptMap0(); }

  ///Get the documented heteromorphous test concept maps
  ConceptMaps GetHeteromorphousTestConceptMaps() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap(const int index) const noexcept;

  ///Get the documented complex homomorphous test concept maps
  ConceptMaps GetComplexHomomorphousTestConceptMaps() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap(const int index) const noexcept;
  int GetNumberOfComplexHomomorphousTestConceptMaps() const noexcept { return 12; }

  ///Get the documented simple homomorphous test concept maps
   ConceptMaps GetSimpleHomomorphousTestConceptMaps() const noexcept;


  ConceptMap GetComplexHomomorphousTestConceptMap1() const noexcept; //Move to private after bug is fixed

  private:
  ConceptMap GetComplexHomomorphousTestConceptMap0() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap2() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap3() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap4() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap5() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap6() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap7() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap8() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap9() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap10() const noexcept;
  ConceptMap GetComplexHomomorphousTestConceptMap11() const noexcept;

  ConceptMap GetHeteromorphousTestConceptMap0() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap1() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap2() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap3() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap4() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap5() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap6() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap7() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap8() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap9() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap10() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap11() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap12() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap13() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap14() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap15() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap16() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap17() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap18() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap19() const noexcept;
  ConceptMap GetHeteromorphousTestConceptMap20() const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};

} //~namespace cmap

} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
