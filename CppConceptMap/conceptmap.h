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

#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "install_vertex_custom_type.h"
#include "install_vertex_is_selected.h"
#include "install_edge_custom_type.h"
#include "install_edge_is_selected.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "conceptmapgraphtypes.h"
namespace ribi {
namespace cmap {

//Concept map definition is in 'conceptmapgraphtypes.h'

int CountCenterNodes(const ConceptMap& c) noexcept;
ConceptMap CreateDirectNeighbourConceptMap(const VertexDescriptor vd, const ConceptMap& c);
std::vector<ConceptMap> CreateDirectNeighbourConceptMaps(const ConceptMap& c);
void DecodeConceptMap(ConceptMap& g) noexcept;
void DecodeEdge(Edge& g) noexcept;
void DecodeNode(Node& g) noexcept;
ConceptMap DotToConceptMap(const std::string& s);
VertexDescriptor FindCenterNode(const ConceptMap& c);
Node GetCenterNode(const ConceptMap& c);
Edge GetEdge(const EdgeDescriptor vd, const ConceptMap& c) noexcept;
std::vector<Edge> GetEdges(const ConceptMap& c) noexcept;
Edge GetFirstEdge(const ConceptMap& c);
Node GetFirstNode(const ConceptMap& c);
Node GetFrom(const Edge& edge, const ConceptMap& c) noexcept;
Node GetFrom(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetNode(const VertexDescriptor vd, const ConceptMap& c) noexcept;
Node GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept;
Node GetTo(const Edge& edge, const ConceptMap& c) noexcept;
std::vector<Node> GetNodes(const ConceptMap& c) noexcept; /// Because 'get_my_custom_vertexes(c)' does not work yet
std::vector<Edge> GetSortedEdges(const ConceptMap& c) noexcept;
std::vector<Node> GetSortedNodes(const ConceptMap& c) noexcept;
bool HasCenterNode(const ConceptMap& c) noexcept;
ConceptMap LoadFromFile(const std::string& dot_filename);
ConceptMap RemoveFirstNode(ConceptMap g);
void SaveToFile(const ConceptMap& g, const std::string& dot_filename);
void SaveToImage(const ConceptMap& g, const std::string& png_filename);
void SaveSummaryToImage(const ConceptMap& g, const std::string& png_filename);
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
