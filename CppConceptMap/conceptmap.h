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
#ifndef CONCEPTMAPCONCEPTMAP_H
#define CONCEPTMAPCONCEPTMAP_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "install_vertex_custom_type.h"
#include "install_vertex_is_selected.h"
#include "install_edge_custom_type.h"
#include "install_edge_is_selected.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"

namespace ribi {
namespace cmap {

using ConceptMap = boost::adjacency_list
<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  boost::property<
    boost::vertex_custom_type_t, Node,
    boost::property<
      boost::vertex_is_selected_t, bool
    >
  >,
  boost::property<
    boost::edge_custom_type_t, Edge,
    boost::property<
      boost::edge_is_selected_t, bool
    >
  >
>;
using VertexDescriptor = boost::graph_traits<ConceptMap>::vertex_descriptor;
static_assert(sizeof(VertexDescriptor) == sizeof(VertexDescriptor&),"On university computer");
using EdgeDescriptor = boost::graph_traits<ConceptMap>::edge_descriptor;

int CountCenterNodes(const ConceptMap& c) noexcept;
ConceptMap CreateDirectNeighbourConceptMap(const VertexDescriptor vd, const ConceptMap& c);
std::vector<ConceptMap> CreateDirectNeighbourConceptMaps(const ConceptMap& c);
ConceptMap DotToConceptMap(const std::string& s);
VertexDescriptor FindCenterNode(const ConceptMap& c) noexcept;
EdgeDescriptor FindEdge(const Edge& e, const ConceptMap& g) noexcept;
VertexDescriptor FindNode(const Node& e, const ConceptMap& g) noexcept;
Edge GetEdge(const EdgeDescriptor vd, const ConceptMap& c) noexcept;
std::vector<Edge> GetEdges(const ConceptMap& c) noexcept;
Edge GetFirstEdge(const ConceptMap& c);
Node GetFocalNode(const ConceptMap& c);
Node GetFrom(const Edge& edge, const ConceptMap& c) noexcept;
Node GetFrom(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetNode(const VertexDescriptor vd, const ConceptMap& c) noexcept;
Node GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetTo(const Edge& edge, const ConceptMap& c) noexcept;
std::vector<Node> GetNodes(const ConceptMap& c) noexcept;
std::vector<Edge> GetSortedEdges(const ConceptMap& c) noexcept;
std::vector<Node> GetSortedNodes(const ConceptMap& c) noexcept;
bool HasCenterNode(const ConceptMap& c) noexcept;
ConceptMap LoadFromFile(const std::string& dot_filename);
void SaveToFile(const ConceptMap& g, const std::string& dot_filename);
void SaveSummaryToFile(const ConceptMap& g, const std::string& dot_filename);
void SelectRandomNode(ConceptMap& conceptmap, std::mt19937& rng_engine) noexcept;
std::string ToXml(const ConceptMap& conceptmap) noexcept;
std::string ToDot(const ConceptMap& conceptmap) noexcept;
ConceptMap UnselectEdges(ConceptMap conceptmap) noexcept;
ConceptMap UnselectEverything(ConceptMap conceptmap) noexcept;
ConceptMap UnselectNodes(ConceptMap conceptmap) noexcept;
ConceptMap XmlToConceptMap(const std::string& s);
std::ostream& operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept;
std::istream& operator>>(std::istream& is, ConceptMap& conceptmap);
bool operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
bool operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAP_H
