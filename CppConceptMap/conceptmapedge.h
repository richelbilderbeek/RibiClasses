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
#ifndef CONCEPTMAPEDGE_H
#define CONCEPTMAPEDGE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapfwd.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop


namespace ribi {
namespace cmap {

struct EdgeFactory;

///An Edge is the GUI-independent part of the edges used in QtConceptMap.
///An Edge goes from one Node to another, which must a different Node,
/// at the center of the Edge is a Node
struct Edge
{
  ///@param node a Node
  ///@param has_head_arrow does the edge have an arrow at its head/target
  ///@param has_tail_arrow does the edge have an arrow at its tail/source
  explicit Edge(
    const Node& node = Node(),
    const bool has_head_arrow = false,
    const bool has_tail_arrow = false
  );

  ~Edge() noexcept;

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept { m_node.Decode(); }

  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }

  bool HasHeadArrow() const noexcept { return m_has_head_arrow; }
  bool HasTailArrow() const noexcept { return m_has_tail_arrow; }

  int GetId() const noexcept { return m_id; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///These arrows are needed: the graph is an undirected graph with/without arrows at either/both sides
  ///This cannot be done by using a directed
  void SetHeadArrow(const bool has_head_arrow) noexcept { m_has_head_arrow = has_head_arrow; }
  void SetTailArrow(const bool has_tail_arrow) noexcept { m_has_tail_arrow = has_tail_arrow; }

  ///Set the center Node
  void SetNode(const Node& node) noexcept;

  std::string ToStr() const noexcept;


  private:
  bool m_has_head_arrow;
  bool m_has_tail_arrow;

  int m_id; //Unique ID, until the Node is copied

  ///The Node on the Edge
  Node m_node;

  //ID to assign
  static int sm_ids;
};

std::string ToXml(const Edge& edge) noexcept;
Edge XmlToEdge(const std::string& s);
std::ostream& operator<<(std::ostream& os, const Edge& edge) noexcept;
std::istream& operator>>(std::istream& is, Edge& edge);

bool operator==(const Edge& lhs, const Edge& rhs);
bool operator!=(const Edge& lhs, const Edge& rhs);
bool operator<(const Edge& lhs, const Edge& rhs);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGE_H
