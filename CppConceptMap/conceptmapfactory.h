//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

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

#include <vector>
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

struct Node;

class ConceptMapFactory
{
public:
  using ConceptMaps = std::vector<ConceptMap>;

  ConceptMapFactory() noexcept;

  ///Get all the other tests as one vector
  ConceptMaps GetAllTests() const noexcept;
  ConceptMaps GetNastyTests() const noexcept;
  ConceptMap GetEmptyGraph() const noexcept { return Get0(); }

  ///Get the documented heteromorphous test concept maps
  ConceptMap Get0() const noexcept; //Empty graph
  ConceptMap Get1() const noexcept;
  ConceptMap Get2() const noexcept;
  ConceptMap Get3() const noexcept;
  ConceptMap Get4() const noexcept;
  ConceptMap Get5() const noexcept;
  ConceptMap Get6() const noexcept;
  ConceptMap Get7() const noexcept;

  /// Complex rated concept map
  ConceptMap Get8() const noexcept;

  /// Complex rated concept map with many examples
  ConceptMap Get9() const noexcept;

  /// Complex rated concept map with many long concept names and examples
  ConceptMap Get10() const noexcept;

  /// Complex rated concept map with all nodes connected
  ConceptMap Get11() const noexcept;
  ConceptMap GetNasty0() const noexcept;

  private:

};

void AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  ConceptMap& g
) noexcept;

VertexDescriptor AddVertex(const Node& node, ConceptMap& g) noexcept;

ConceptMap RepositionNodes(ConceptMap& g);


} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
