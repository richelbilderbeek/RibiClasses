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
  using Graphs = std::vector<ConceptMap>;

  ConceptMapFactory() noexcept;

  ///Get all the other tests as one vector
  Graphs GetAllTests() const noexcept;
  Graph GetTest(const int index) const noexcept;
  int GetNumberOfAllTests() const noexcept { return 7; }
  Graphs GetNastyTests() const noexcept;

  Graph GetEmptyGraph() const noexcept { return Get0(); }

  ///Get the documented heteromorphous test concept maps
  Graph Get0() const noexcept; //Empty graph
  Graph Get1() const noexcept;
  Graph Get2() const noexcept;
  Graph Get3() const noexcept;
  Graph Get4() const noexcept;
  Graph Get5() const noexcept;
  Graph Get6() const noexcept;
  Graph GetNasty0() const noexcept;

  private:

};

void AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  Graph& g
) noexcept;

VertexDescriptor AddVertex(const Node& node, Graph& g) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAPFACTORY_H
