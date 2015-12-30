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
#ifndef CONCEPTMAPCONCEPTMAP_H
#define CONCEPTMAPCONCEPTMAP_H

#ifndef DO_NOT_USE_BOOST_GRAPH
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "conceptmapvertexcustomtype.h"
#include "conceptmapvertexisselected.h"
#include "conceptmapedgecustomtype.h"
#include "conceptmapedgeisselected.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
//#include "conceptmapgraphtypes.h"

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
using EdgeDescriptor = boost::graph_traits<ConceptMap>::edge_descriptor;

} //~namespace cmap
} //~namespace ribi
#else
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
//#include <boost/shared_ptr.hpp>
//#include <boost/signals2.hpp>
//#include <QPointF>
//#include <QUndoStack>
#include "conceptmapfwd.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmapgraphtypes.h"
#pragma GCC diagnostic pop


namespace ribi {
namespace cmap {

struct ConceptMapFactory;

///A concept map
///Its interface is based on nodes and edges
///Use ConceptMapWidget to work with commands
class ConceptMap
{
public:
  explicit ConceptMap(const Graph& graph = Graph()) noexcept;
  ~ConceptMap() noexcept {}

  using ConceptMaps = std::vector<ConceptMap>;
  using Edges = std::vector<Edge>;
  using Nodes = std::vector<Node>;
  using EdgesAndNodes = std::pair<Edges,Nodes>;
  using SubConceptMaps = ConceptMaps; //Just to let the code speak more for itself

  //Add an Edge, assumes that the nodes it points to are in the concept map
  void AddEdge(
    const VertexDescriptor& vd_from,
    const VertexDescriptor& vd_to,
    const Edge& edge
  ) noexcept;

  //Add a node, always works
  VertexDescriptor AddNode(const Node& node) noexcept;

  ///Add the nodes to the current (can be zero) selected nodes
  void AddSelected(const Edges& edges) noexcept;
  void AddSelected(const Nodes& nodes) noexcept;
  void AddSelected(const Edges& edges,const Nodes& nodes) noexcept;

  bool AreAllEdgeIdsUnique() const noexcept;
  bool AreAllNodeIdsUnique() const noexcept;

  int CountSelectedEdges() const noexcept;
  int CountSelectedNodes() const noexcept;

  void ClearAllSelectednesses() noexcept;

  ///Prepend the question as a first node, before adding the supplied nodes
  static Nodes CreateNodes(
    const std::string& question,
    const Nodes& nodes
  ) noexcept;

  ConceptMap CreateSub(const VertexDescriptor& vd) const noexcept;

  ///Create all sub-conceptmaps
  ///Note that CreateSubs()[0] is the concept map around the focal question
  SubConceptMaps CreateSubs() const noexcept;


  ///Delete an edge
  void DeleteEdge(const Edge& edge) noexcept;

  ///Delete a node and all the edges connected to it
  void DeleteNode(const Node& node) noexcept;

  ///Check if the ConceptMap is empty, that is: it has no nodes and (thus) no edges
  bool Empty() const noexcept;

  EdgeDescriptor FindEdge(const Edge& edge) const noexcept;
  EdgeDescriptor FindEdgeHaving(const Node& node) const noexcept;
  VertexDescriptor FindNode(const Node& node) const noexcept;

  VertexDescriptor FindCenterNode() const noexcept;
  VertexDescriptor FindCenterNode()       noexcept;

  ///Find the Edge that has the node as its center Node
  ///Returns nullptr if not present
  EdgeDescriptor GetEdgeHaving(const Node& node) const noexcept;

  Edges GetEdges() const noexcept;

  ///Find the Edges that start or end at the node
  Edges GetEdgesConnectedTo(const Node& node) const noexcept;

  ///Get the focal node (always at index zero), if any
  VertexDescriptor GetFocalNode() const noexcept;
  VertexDescriptor GetFocalNode()       noexcept;

  Graph GetGraph() const noexcept { return m_graph; }

  Nodes GetNodes() const noexcept;
  VertexDescriptor GetNodeWithIndex(const int index) noexcept;

  ///There can be multiple items selected
  //const EdgesAndNodes& GetSelected() const noexcept { return m_selected; }
  //      EdgesAndNodes& GetSelected()       noexcept { return m_selected; }

  //const Edges& GetSelectedEdges() const noexcept { return m_selected.first; }
  //      Edges& GetSelectedEdges()       noexcept { return m_selected.first; }

  Nodes GetSelectedNodes() const noexcept;

  //const QUndoStack& GetUndo() const noexcept { return m_undo; }
  //      QUndoStack& GetUndo()       noexcept { return m_undo; }

  int GetNumberOfEdges() const noexcept;
  int GetNumberOfNodes() const noexcept;

  bool GetVerbosity() const noexcept { return m_verbose; }

  ///Obtain the version
  static std::string GetVersion() noexcept;

  ///Obtain the version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  bool HasCenterNode() const noexcept;
  bool HasEdge(const Edge& edge) const noexcept;
  bool HasEdgeHaving(const Node& node) const noexcept;
  bool HasNode(const Node& node) const noexcept;
  bool HasNodeWithIndex(const int index) const noexcept;
  bool HasSelectedNodes() const noexcept;

  bool IsConnectedToCenterNode(const Edge& edge) const noexcept;
  bool IsSelected(const Edge& node) const noexcept;
  bool IsSelected(const Node& node) const noexcept;

  //void MouseMoveEvent(const double mouse_x) noexcept;

  void Redo() noexcept;

  ///Add the nodes to the current (can be zero) selected nodes
  void RemoveSelectedness(const Edges& edges) noexcept;
  void RemoveSelectedness(const Nodes& nodes) noexcept;
  void RemoveSelectedness(const Edges& edges,const Nodes& nodes) noexcept;

  ///Set the nodes to the only nodes selected
  void SetSelected(const Nodes& nodes) noexcept;
  void SetSelected(const Edges& edges) noexcept;
  void SetSelected(const Edges& edges,const Nodes& nodes) noexcept; //WORK
  void SetSelected(const EdgesAndNodes& edges_and_nodes) noexcept;

  void SetVerbosity(const bool verbose) noexcept { m_verbose = verbose; }

private:

  Graph m_graph;

  bool m_verbose;

  static ConceptMap CreateEmptyConceptMap() noexcept;

  ///Creates a new Edge in the concept map. The return value is
  ///that node. This is used by CommandCreateNode::Undo
  Edge CreateNewEdge() noexcept;

  ///Creates a new Node in the concept map. The return value is
  ///that node. This is used by CommandCreateNode::Undo
  Node CreateNewNode() noexcept;

  ///Used by CommandAddSelectedRandom and CommandSetSelectedRandom
  ///Of all the concept maps its nodes, except for the uses supplied as the
  ///argument, return 1 to all the nodes, except when there is no node
  ///left (as all are excluded) or the concept map does not have any nodes
  std::vector<Edge> GetRandomEdges(std::vector<Edge> edges_to_exclude = {}) noexcept;
  Edge GetRandomEdge(std::vector<Edge> edges_to_exclude = {});


  ///Used by CommandAddSelectedRandom and CommandSetSelectedRandom
  ///Of all the concept maps its nodes, except for the uses supplied as the
  ///argument, return 1 to all the nodes, except when there is no node
  ///left (as all are excluded) or the concept map does not have any nodes
  std::vector<Node> GetRandomNodes(Nodes nodes_to_exclude = {}) noexcept;
  Node GetRandomNode(const Nodes& nodes_to_exclude = {});

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  ///Unselect the node, assumes it is selected

  void Unselect(const EdgesAndNodes& edges_and_nodes) noexcept;
  void Unselect(const Edges& edges) noexcept;
  void Unselect(const Nodes& nodes) noexcept;

  ///Create a concept map from a cluster
  #ifdef TO_ADD_TO_PROJECTBRAINWEAVER
  ConceptMap(
    const std::string& question,
    const Cluster& cluster);
  #endif

  ///To make the compiler use the const version
  VertexDescriptor FindCenterNodeConst() const noexcept { return FindCenterNode(); }
  ///To make the compiler use the const version
  //ReadOnlyNodePtr GetFocalNodeConst() const noexcept { return GetFocalNode(); }

  void UnselectAllEdges() noexcept;
  void UnselectAllNodes() noexcept;

  friend class ConceptMapFactory;


  //friend class Command;
  friend class CommandAddSelectedRandom;
  friend class CommandCreateNewConceptMap;
  friend class CommandCreateNewEdge;
  friend class CommandCreateNewNode;
  friend class CommandDeleteConceptMap;
  friend class CommandDeleteNode;
  friend class CommandSetSelectedWithCoordinat;
  friend class CommandUnselectRandom;
  friend bool operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const ConceptMap& m) noexcept;

};

///Test if a ConceptMap can successfully be constructed
///from the nodes and edges
/*
bool CanConstruct(
  const std::vector<Node>& nodes,
  const std::vector<Edge>& edges,
  const bool verbose = false
) noexcept;
*/

///Count the number of CenterNodes
///- regular concept map: 1, the focus
///- sub-concept map: 0 or 1, if the focus is connected to the sub's focus node
int CountCenterNodes(const ConceptMap& conceptmap) noexcept;

///Count the number of Edges connected to a CenterNodes
int CountCenterNodeEdges(const ConceptMap& conceptmap) noexcept;

///Similar to operator==, except that the GUI member variables aren't checked for equality
bool HasSameContent(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;

///Convert a ConceptMap from an XML std::string
std::string ToXml(const ConceptMap& c) noexcept;


bool operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
bool operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const ConceptMap& m) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // DO_NOT_USE_BOOST_GRAPH

#endif // CONCEPTMAPCONCEPTMAP_H
