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

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <QPointF>
//#include <QUndoStack>
#include "conceptmapfwd.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
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

  ConceptMap(const ConceptMap&);
  ConceptMap(ConceptMap&&);
  ConceptMap& operator=(const ConceptMap&);
  ConceptMap& operator=(ConceptMap&&);
  ~ConceptMap() noexcept {}

  using ConceptMaps = std::vector<ConceptMap>;
  using EdgePtr = Edge*;
  using Edges = std::vector<Edge>;
  using Nodes = std::vector<Node>;
  using NodePtr = Node*;
  using EdgesAndNodes = std::pair<Edges,Nodes>;
  using ReadOnlyCenterNodePtr = Node*;
  using SubConceptMaps = ConceptMaps; //Just to let the code speak more for itself

  //Add an Edge, assumes that the nodes it points to are in the concept map
  void AddEdge(const Edge& edge) noexcept;

  //Add a node, always works
  void AddNode(const Node& node) noexcept;

  ///Add the nodes to the current (can be zero) selected nodes
  void AddSelected(const Edges& edges) noexcept;
  void AddSelected(const Nodes& nodes) noexcept;
  void AddSelected(const Edges& edges,const Nodes& nodes) noexcept;

  ///Test if this ConceptMap can be constructed successfully
  static bool CanConstruct(
    const Nodes& nodes,
    const Edges& edges
  ) noexcept;

  #ifdef CONCEPTMAP_USE_QUNDOSTACK
  bool CanRedo() const noexcept { return m_undo.canRedo(); }
  bool CanUndo() const noexcept { return m_undo.canUndo(); }
  #endif //CONCEPTMAP_USE_QUNDOSTACK

  ///Prepend the question as a first node, before adding the supplied nodes
  static Nodes CreateNodes(
    const std::string& question,
    const Nodes& nodes
  ) noexcept;

  ///Create all sub-conceptmaps
  ///Note that CreateSubs()[0] is the concept map around the focal question
  SubConceptMaps CreateSubs() const noexcept;

  ///Delete an edge
  void DeleteEdge(const Edge& edge) noexcept;

  ///Delete a node and all the edges connected to it
  void DeleteNode(const Node& node) noexcept;

  #ifdef CONCEPTMAP_USE_QUNDOSTACK
  ///ConceptMap takes over ownership of the Command
  void DoCommand(Command * const command) noexcept;
  #endif // CONCEPTMAP_USE_QUNDOSTACK

  ///Check if the ConceptMap is empty, that is: it has no nodes and (thus) no edges
  bool Empty() const noexcept;

  ///Find the CenterNode, if any
  const Node* FindCenterNode() const noexcept;
        Node* FindCenterNode()       noexcept;

  ///Find the Edge that has the node as its center Node
  ///Returns nullptr if not present
  const Edge * GetEdgeHaving(const Node& node) const noexcept;

  const Edges& GetEdges() const noexcept { return m_edges; }
        Edges& GetEdges()       noexcept { return m_edges; }

  ///Find the Edges that start or end at the node
  Edges GetEdgesConnectedTo(const Node& node) const noexcept;

  ///Get the focal node (always at index zero), if any
  const Node* GetFocalNode() const noexcept;
        Node* GetFocalNode()       noexcept;

  const Nodes& GetNodes() const noexcept { return m_nodes; }
        Nodes& GetNodes()       noexcept { return m_nodes; }

  ///There can be multiple items selected
  const EdgesAndNodes& GetSelected() const noexcept { return m_selected; }
        EdgesAndNodes& GetSelected()       noexcept { return m_selected; }

  const Edges& GetSelectedEdges() const noexcept { return m_selected.first; }
        Edges& GetSelectedEdges()       noexcept { return m_selected.first; }

  const Nodes& GetSelectedNodes() const noexcept { return m_selected.second; }
        Nodes& GetSelectedNodes()       noexcept { return m_selected.second; }

  //const QUndoStack& GetUndo() const noexcept { return m_undo; }
  //      QUndoStack& GetUndo()       noexcept { return m_undo; }

  bool GetVerbosity() const noexcept { return m_verbose; }

  ///Obtain the version
  static std::string GetVersion() noexcept;

  ///Obtain the version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  bool HasEdge(const Edge& edge) const noexcept;
  bool HasNode(const Node& node) const noexcept;

  bool IsSelected(const Edge& node) const noexcept;
  bool IsSelected(const Node& node) const noexcept;

  #ifndef NDEBUG
  ///Check if there are no nulls in the edges and nodes
  bool IsValid() const noexcept;
  #endif


  void MouseMoveEvent(const QPointF& mouse_pos) noexcept;

  void Redo() noexcept;

  ///Add the nodes to the current (can be zero) selected nodes
  void RemoveSelected(const Edges& edges) noexcept;
  void RemoveSelected(const Nodes& nodes) noexcept;
  void RemoveSelected(const Edges& edges,const Nodes& nodes) noexcept;

  ///Set the nodes to the only nodes selected
  void SetSelected(const Nodes& nodes) noexcept;
  void SetSelected(const Edges& edges) noexcept;
  void SetSelected(const Edges& edges,const Nodes& nodes) noexcept; //WORK
  void SetSelected(const EdgesAndNodes& edges_and_nodes) noexcept;

  void SetVerbosity(const bool verbose) noexcept { m_verbose = verbose; }

  void Undo() noexcept;

private:

  ///The edges
  Edges m_edges;

  ///The nodes
  Nodes m_nodes;

  ///The elements selected
  ///- a true Node
  ///- the label in the middle of an edge
  ///- the CenterNode
  EdgesAndNodes m_selected;

  #ifdef CONCEPTMAP_USE_QUNDOSTACK
  ///The undo stack
  ///The Commands aren't const, because Command::Undo changes their state
  //QUndoStack m_undo;
  #endif // CONCEPTMAP_USE_QUNDOSTACK

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

  #ifdef CONCEPTMAP_USE_QUNDOSTACK
  void OnUndo() noexcept;
  #endif // CONCEPTMAP_USE_QUNDOSTACK


  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  ///Unselect the node, assumes it is selected

  void Unselect(const EdgesAndNodes& edges_and_nodes) noexcept;
  void Unselect(const Edges& edges) noexcept;
  void Unselect(const Nodes& nodes) noexcept;

  ///Block constructor, except for the friend ConceptMapFactory
  ConceptMap(const std::string& question) noexcept;

  //Nodes[0] must be the focal question
  explicit ConceptMap(
    const Nodes& nodes = {},
    const Edges& edges = {}
  ) noexcept;
  ///Create a concept map from a cluster
  #ifdef TO_ADD_TO_PROJECTBRAINWEAVER
  ConceptMap(
    const std::string& question,
    const boost::shared_ptr<cmap::Cluster>& cluster);
  #endif

  ///To make the compiler use the const version
  const Node* FindCenterNodeConst() const noexcept { return FindCenterNode(); }
  ///To make the compiler use the const version
  //ReadOnlyNodePtr GetFocalNodeConst() const noexcept { return GetFocalNode(); }

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

};

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

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTMAP_H
