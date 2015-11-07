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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "conceptmap.h"

#include <iostream>
#include <set>
#include <sstream>
#include <functional>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>

#include "conceptmapcenternodefactory.h"
#include "conceptmapcenternode.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapcommand.h"
#include "conceptmapcommandaddselectedrandom.h"
#include "conceptmapcommandcreatenewnode.h"
#include "conceptmapcommandcreatenewedge.h"
#include "conceptmapcommandfactory.h"
#include "conceptmapcommandunselectrandom.h"
#include "conceptmapfactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::ConceptMap::ConceptMap(const std::string& question) noexcept
  : //Signals first, as these are public
    m_signal_add_edge{},
    m_signal_add_node{},
    m_signal_conceptmap_changed{},
    m_signal_delete_edge{},
    m_signal_delete_node{},
    m_signal_selected_changed{},
    m_edges( {} ),
    m_nodes(CreateNodes(question, {} )),
    m_selected{},
    m_undo{},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  assert(ConceptMap::CanConstruct(m_nodes,m_edges));
  assert(FindCenterNode()
    && "Assume a CenterNode at the center of ConceptMap");
  assert(IsCenterNode(*FindCenterNode())
    && "The CenterNode found must really be a CenterNode");
  assert(FindCenterNode()->GetConcept().GetName() == question
    && "The CenterNode must display the focus");
  #endif
}

ribi::cmap::ConceptMap::ConceptMap(
    const Nodes& nodes,
    const Edges& edges
)  noexcept
  :
    m_signal_add_edge{},
    m_signal_add_node{},
    m_signal_conceptmap_changed{},
    m_signal_delete_edge{},
    m_signal_delete_node{},
    m_signal_selected_changed{},
    m_edges(edges),
    m_nodes(nodes),
    m_selected{},
    m_undo{},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  if (!ConceptMap::CanConstruct(nodes,edges))
  {
    TRACE("ERROR");
    const std::size_t n_nodes = nodes.size();
    for (std::size_t i=0; i!=n_nodes; ++i)
    {
      std::cout << i << ": " << nodes[i].ToXml() << '\n';
    }

    const std::size_t n_edges = edges.size();
    for (std::size_t i=0; i!=n_edges; ++i)
    {
      const auto edge = edges[i];
      const auto from_iter = std::find(std::begin(nodes),std::end(nodes),*edge->GetFrom());
      const auto to_iter = std::find(std::begin(nodes),std::end(nodes),*edge->GetTo());
      assert(from_iter != nodes.end());
      assert(to_iter != nodes.end());
      std::cout
        << i << ": "
        << Edge::ToXml(boost::const_pointer_cast<const Edge>(edge),nodes)
        << '\n';
    }
  }
  assert(ConceptMap::CanConstruct(nodes,edges));
  #endif
}

#ifdef TO_ADD_TO_PROJECTBRAINWEAVER
ribi::cmap::ConceptMap::ConceptMap(
  const std::string& question,
  const boost::shared_ptr<cmap::Cluster>& cluster)
  : m_edges{},
    m_nodes(CreateNodes(question, {} ))
{
  #ifndef NDEBUG
  Test();
  #endif

  const std::vector<boost::shared_ptr<ribi::cmap::Concept>>& v = cluster->Get();
  const int n = boost::numeric_cast<int>(v.size());
  for (int i=0; i!=n; ++i)
  {
    const int x = 0;
    const int y = 0;
    const boost::shared_ptr<Node> node = NodeFactory().Create(v[i],x,y);
    assert(node);
    m_nodes.push_back(node);
  }
  assert(v.size() + 1 == m_nodes.size()
    && "Assume the ConceptMap has as much nodes as the cluster has concepts + one focal question");
}
#endif //TO_ADD_TO_PROJECTBRAINWEAVER

void ribi::cmap::ConceptMap::AddEdge(const EdgePtr& edge) noexcept
{
  assert(edge);
  if (std::find(std::begin(m_edges),std::end(m_edges),edge) != std::end(m_edges))
  {
    if (m_verbose) { TRACE("Warning: edge already added"); }
    return;
  }

  assert(edge->GetFrom());
  assert(edge->GetTo());

  //Add Nodes if they are not present yet
  //if (!HasNode(edge->GetFrom())) { AddNode(edge->GetFrom()); }
  //if (!HasNode(edge->GetTo())  ) { AddNode(edge->GetTo()  ); }


  assert(HasNode(*edge->GetFrom()));
  assert(HasNode(*edge->GetTo()));

  assert(std::count(std::begin(m_nodes),std::end(m_nodes),*edge->GetFrom()) == 1
    && "First enter the node this edge originates from");
  assert(std::count(std::begin(m_nodes),std::end(m_nodes),*edge->GetTo()) == 1
    && "First enter the node this edge targets to");
  m_edges.push_back(edge);

  this->SetSelected( Edges( { edge } ) );

  m_signal_add_edge(edge);
}

void ribi::cmap::ConceptMap::AddNode(const Node& node) noexcept
{
  if (std::find(std::begin(m_nodes),std::end(m_nodes),node) != std::end(m_nodes))
  {
    if (m_verbose) { TRACE("Warning: node already added"); }
    return;
  }

  this->AddSelected( { node } );

  m_nodes.push_back(node);
  //m_signal_add_node(node);

  assert(std::count(begin(m_nodes),end(m_nodes),node) == 1
    && "Every node must be unique");
  //Do not let concept map signal that a new node has been added:
  //keep it as stupid as possible. Let ConceptMapWidget
  //have this behavior
}


bool ribi::cmap::ConceptMap::CanConstruct(
  const Nodes& /* nodes */,
  const Edges& edges) noexcept
{
  const bool trace_verbose = false;

  //Test if there are 'two-way' edges, that is, one edge going from A to B
  //and another edge going from B to A
  {
    const int n_edges = edges.size();
    for (int i=0; i!=n_edges; ++i)
    {
      const boost::shared_ptr<ribi::cmap::Edge> & a = edges[i];
      const auto a_from = a->GetFrom();
      const auto a_to   = a->GetTo();
      for (int j=i+1; j!=n_edges; ++j)
      {
        assert(i != j);
        assert(j < n_edges);
        const boost::shared_ptr<Edge> & b = edges[j];
        assert(a.get() != b.get() && "Assume different pointers");
        const auto b_from = b->GetFrom();
        const auto b_to   = b->GetTo();
        if (a_from == b_from && a_to == b_to)
        {
          if (trace_verbose)
          {
            TRACE("Cannot have two edges from the same node to the same node");
          }
          return false;
        }
        if (a_from == b_to && a_to == b_from)
        {
          if (trace_verbose)
          {
            TRACE("Cannot have two edges from the same node to the same node");
          }
          return false;
        }
      }
    }
  }
  return true;
}

ribi::cmap::ConceptMap::Nodes ribi::cmap::ConceptMap::CreateNodes(
  const std::string& question,
  const Nodes& nodes) noexcept
{
  Nodes v;
  const Concept concept {
    ConceptFactory().Create(
      question,
      ExamplesFactory().Create(), //No examples
      false, //Is not complex
      -1, //No rated complexity
      -1, //No rated concreteness
      -1  //No rated specificity
    )
  };
  const CenterNode center_node {
    CenterNodeFactory().Create(concept,0.0,0.0)
  };
  v.push_back(center_node);
  std::copy(begin(nodes),end(nodes),std::back_inserter(v));
  assert(v.size() == nodes.size() + 1);
  return v;
}

ribi::cmap::ConceptMap::SubConceptMaps ribi::cmap::ConceptMap::CreateSubs() const noexcept
{
  SubConceptMaps v;
  for (const auto& focal_node: m_nodes)
  {
    //Collect all edges connected top the focal node (which is m_nodes[i])
    Nodes nodes;
    Edges edges;

    nodes.push_back(focal_node);

    for (const boost::shared_ptr<Edge>& focal_edge: m_edges)
    {
     if (focal_edge->GetFrom() == &focal_node)
      {
        edges.push_back(focal_edge);
        assert(focal_edge->GetTo() != &focal_node);
        nodes.push_back(*focal_edge->GetTo());
      }
      else if (focal_edge->GetTo() == &focal_node)
      {
        edges.push_back(focal_edge);
        assert(focal_edge->GetFrom() != &focal_node);
        nodes.push_back(*focal_edge->GetFrom());
      }
    }
    assert(!nodes.empty());
    //Put CenterNode in front
    //const auto iter = container().FindIf(nodes,[](const auto& node) { return IsCenterNode(node); } );
    const auto iter = std::find_if(std::begin(nodes),std::end(nodes),[](const Node& node) { return IsCenterNode(node); } );
    if (iter != nodes.end()) { std::swap(nodes.front(),*iter); }
    assert(ConceptMap::CanConstruct(nodes,edges) && "Only construct valid concept maps");
    const boost::shared_ptr<ConceptMap> conceptmap(new ConceptMap(nodes,edges));
    assert(conceptmap->IsValid());
    v.push_back(conceptmap);
  }
  return v;
}

void ribi::cmap::ConceptMap::DeleteEdge(const EdgePtr& edge) noexcept
{
  DeleteEdge(boost::dynamic_pointer_cast<const Edge>(edge));
}

void ribi::cmap::ConceptMap::DeleteEdge(const ReadOnlyEdgePtr& edge) noexcept
{
  #ifndef NDEBUG
  assert(edge);
  //One cannot be sure if the edges are already deleted or not
  if(std::count(begin(m_edges),end(m_edges),edge) == 0) return;
  assert(std::count(begin(m_edges),end(m_edges),edge) != 0
    && "The edge must exist");
  assert(std::count(begin(m_edges),end(m_edges),edge) == 1
    && "Every edge is unique");
  const std::size_t n_edges_before = m_edges.size();
  #endif


  assert(edge);

  ///Edge might already be deleted by something else
  if (GetEdges().empty()) return;
  {
    const auto& edges = GetEdges();
    if (std::find(begin(edges),end(edges),edge) == edges.end()) return;
  }

  m_edges.erase(std::remove(begin(m_edges),end(m_edges),edge),m_edges.end());

  m_signal_delete_edge(edge);


  #ifndef NDEBUG
  const std::size_t n_edges_after = m_edges.size();
  assert(n_edges_before - 1 == n_edges_after);
  #endif
}

void ribi::cmap::ConceptMap::DeleteNode(const Node& node) noexcept
{
  if (std::count(std::begin(m_nodes),std::end(m_nodes),node) == 0)
  {
    //There are multiple ways to delete a node:
    // - QtConceptMap: delete the QtNode, which deletes the Node
    // - ConceptMapWidget: deletes the Node, which signals its observers of this event
    //Because the sequence
    return;
  }

  const auto to_be_deleted = GetEdgesConnectedTo(node);

  for (boost::shared_ptr<Edge> edge: to_be_deleted)
  {
    DeleteEdge(edge);
  }

  //Delete the node itself
  m_nodes.erase(
    std::remove(
      std::begin(m_nodes),
      std::end(m_nodes),
      node
    ),
    std::end(m_nodes)
  );

  //Unselect the node
  this->Unselect( ConstNodes( { node } ) );

  //If there is no node selected anymore, give focus to a possible first node
  if (GetSelectedNodes().empty() && !GetNodes().empty())
  {
    this->SetSelected(Nodes( { GetNodes().front() } ) );
  }

  m_signal_delete_node(node);
}

bool ribi::cmap::ConceptMap::Empty() const noexcept
{
  assert(!(m_nodes.empty() && !m_edges.empty())
    && "If there are no nodes, there cannot be edges");
  return m_nodes.empty(); // && m_edges.empty();
}

const ribi::cmap::CenterNode* ribi::cmap::ConceptMap::FindCenterNode() const noexcept
{
  return ribi::cmap::FindCenterNode(m_nodes);
  /*
  const auto iter = std::find_if(std::begin(m_nodes),std::end(m_nodes),
    [](const auto& node)
    {
      return IsCenterNode(node);
    }
  );
  if (iter == std::end(m_nodes))
  {
    return nullptr;
  }
  else
  {
    return &(*iter);
  }
  */
}

ribi::cmap::CenterNode* ribi::cmap::ConceptMap::FindCenterNode() noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  const CenterNode* center_node {
    const_cast<const ConceptMap*>(this)->FindCenterNode() //Add const because compiler cannt find the right version
    //?used to be dynamic_cast?
  };
  return const_cast<CenterNode*>(center_node);
}

ribi::cmap::ConceptMap::EdgePtr ribi::cmap::ConceptMap::GetEdgeHaving(const Node& node) const noexcept
{
  const auto iter = std::find_if(
    std::begin(m_edges),
    std::end(m_edges),
    [node](const EdgePtr& edge) { return edge->GetNode() == node; }
  );
  if (iter == std::end(m_edges)) { return EdgePtr(); }
  return *iter;
}

ribi::cmap::ConceptMap::ReadOnlyEdges ribi::cmap::ConceptMap::GetEdges() const noexcept
{
  return AddConst(m_edges);
}

ribi::cmap::ConceptMap::Edges ribi::cmap::ConceptMap::GetEdgesConnectedTo(const ReadOnlyNodePtr& node) const noexcept
{
  Edges edges;
  std::copy_if(
    std::begin(m_edges),
    std::end(m_edges),
    std::back_inserter(edges),
    [node](const EdgePtr& edge)
    {
      return edge->GetFrom() == &node || edge->GetTo() == &node;
    }
  );
  return edges;
}


const ribi::cmap::Node* ribi::cmap::ConceptMap::GetFocalNode() const noexcept
{
  if (m_nodes.empty()) return nullptr;
  return &m_nodes[0];
}

ribi::cmap::Node* ribi::cmap::ConceptMap::GetFocalNode() noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  const Node* focal_node {
    dynamic_cast<const ConceptMap*>(this)->GetFocalNode() //Compiler cannot distinguish member functions by type
  };
  return const_cast<Node*>(focal_node);
}

std::string ribi::cmap::ConceptMap::GetVersion() noexcept
{
  return "2.2";
}

std::vector<std::string> ribi::cmap::ConceptMap::GetVersionHistory() noexcept
{
  return {
    "2013-12-xx: Version 1.0: initial version",
    "2013-12-23: Version 1.1: started versioning",
    "2014-02-08: Version 1.2: support an empty concept map"
    "2014-03-24: Version 1.3: distinguished correctly between focus and selected"
    "2015-08-13: Version 2.0: merge of ConceptMap and ConceptMapWidget",
    "2015-08-28: Version 2.1: removed many useless member variables",
    "2015-11-01: Version 2.2: made Node a regular type"
  };
}

bool ribi::cmap::ConceptMap::HasEdge(const boost::shared_ptr<const Edge>& edge) const noexcept
{
  return std::count(std::begin(m_edges),std::end(m_edges),edge);
}

bool ribi::cmap::ConceptMap::HasNode(const Node& node) const noexcept
{
  return container().Count(m_nodes,node);
}

bool ribi::cmap::HasSameContent(
  const ribi::cmap::ConceptMap& lhs,
  const ribi::cmap::ConceptMap& rhs
) noexcept
{
  const bool trace_verbose{false};

  if (lhs.GetEdges().size() != rhs.GetEdges().size())
  {
    if (trace_verbose) { TRACE("Number of edges differ"); }
    return false;
  }
  if (lhs.GetNodes().size() != rhs.GetNodes().size())
  {
    if (trace_verbose)
    {
      TRACE("Number of nodes differ");
      TRACE(lhs.GetNodes().size());
      TRACE(rhs.GetNodes().size());
    }
    return false;
  }
  //Same Concepts
  {
    const auto nodes_lhs = lhs.GetNodes();
    std::multiset<Concept> concepts_lhs;
    std::transform(
      std::begin(nodes_lhs),std::end(nodes_lhs),
      std::inserter(concepts_lhs,std::begin(concepts_lhs)),
      [](const Node& node) { return node.GetConcept(); }
    );

    const auto& nodes_rhs = rhs.GetNodes();
    std::multiset<Concept> concepts_rhs;
    std::transform(
      std::begin(nodes_rhs),
      std::end(nodes_rhs),
      std::inserter(concepts_rhs,std::begin(concepts_rhs)),
      [](const Node& node) { return node.GetConcept(); }
    );
    if (std::mismatch(std::begin(concepts_lhs),concepts_lhs.end(),std::begin(concepts_rhs))
      != std::make_pair(concepts_lhs.end(),concepts_rhs.end()))
    {
      if (trace_verbose) { TRACE("Node concepts differ"); }
      return false;
    }
  }
  //Same Edges
  {
    const std::vector<boost::shared_ptr<const Edge>> edges_lhs = lhs.GetEdges();
    std::multiset<Concept> concepts_lhs;
    std::transform(std::begin(edges_lhs),std::end(edges_lhs),
      std::inserter(concepts_lhs,std::begin(concepts_lhs)),
      [](const boost::shared_ptr<const Edge>& edge)
      {
        assert(edge);
        return edge->GetNode().GetConcept();
      }
    );

    const std::vector<boost::shared_ptr<const Edge>> edges_rhs = rhs.GetEdges();

    std::multiset<Concept> concepts_rhs;
    std::transform(begin(edges_rhs),end(edges_rhs),
      std::inserter(concepts_rhs,std::begin(concepts_rhs)),
      [](const boost::shared_ptr<const cmap::Edge>& edge)
      {
        assert(edge);
        return edge->GetNode().GetConcept();
      }
    );
    if (std::mismatch(std::begin(concepts_lhs),concepts_lhs.end(),std::begin(concepts_rhs))
      != std::make_pair(concepts_lhs.end(),concepts_rhs.end()))
    {
      if (trace_verbose) { TRACE("Edge concepts differ"); }
      return false;
    }
  }
  //Check if for each edge a same 'from' and 'to' concept can be found
  {
    typedef std::tuple<std::string,std::string,std::string> FakeEdge;
    typedef std::vector<FakeEdge> FakeEdges;

    assert(lhs.GetEdges().size() == rhs.GetEdges().size());

    FakeEdges v;
    const int sz = lhs.GetEdges().size();
    for (int i=0; i!=sz; ++i)
    {
      const auto from_node = lhs.GetEdges()[i]->GetFrom();
      const std::string str_from = from_node->GetConcept().GetName();
      const std::string str_mid = lhs.GetEdges()[i]->GetNode().GetConcept().GetName();
      const auto to_node = lhs.GetEdges()[i]->GetTo();
      const std::string str_to = to_node->GetConcept().GetName();
      //Only if arrow is reversed, reverse the fake edge
      if (
           lhs.GetEdges()[i]->HasTailArrow() == true
        && lhs.GetEdges()[i]->HasHeadArrow() == false)
      {
        v.push_back(std::make_tuple(str_to,str_mid,str_from));
      }
      else if (lhs.GetEdges()[i]->HasTailArrow() == lhs.GetEdges()[i]->HasHeadArrow())
      {
        //Two-way or zero-way arrow, add it in both directions
        v.push_back(std::make_tuple(str_to,str_mid,str_from));
        v.push_back(std::make_tuple(str_from,str_mid,str_to));
      }
      else
      {
        //Normal arrow
        v.push_back(std::make_tuple(str_from,str_mid,str_to));
      }
    }
    assert(boost::numeric_cast<int>(v.size()) >= sz);

    FakeEdges w;
    for (int i=0; i!=sz; ++i)
    {
      const auto from_node = rhs.GetEdges()[i]->GetFrom();
      const std::string str_from = from_node->GetConcept().GetName();
      const std::string str_mid = rhs.GetEdges()[i]->GetNode().GetConcept().GetName();
      const auto to_node = rhs.GetEdges()[i]->GetTo();
      const std::string str_to = to_node->GetConcept().GetName();
      //w.push_back(std::make_tuple(str_from,str_mid,str_to));
      //Only if arrow is reversed, reverse the fake edge
      if (
           rhs.GetEdges()[i]->HasTailArrow() == true
        && rhs.GetEdges()[i]->HasHeadArrow() == false)
      {
        w.push_back(std::make_tuple(str_to,str_mid,str_from));
      }
      else if (rhs.GetEdges()[i]->HasTailArrow() == rhs.GetEdges()[i]->HasHeadArrow())
      {
        //Two-way or zero-way arrow, add it in both directions
        w.push_back(std::make_tuple(str_to,str_mid,str_from));
        w.push_back(std::make_tuple(str_from,str_mid,str_to));
      }
      else
      {
        //Normal arrow
        w.push_back(std::make_tuple(str_from,str_mid,str_to));
      }
    }
    assert(static_cast<int>(w.size()) >= sz);
    if (v.size() != w.size()) return false;

    std::sort(std::begin(v),std::end(v));
    std::sort(std::begin(w),std::end(w));
    if (v != w)
    {
      //#define REALLY_SHOW_ME_THIS_7364894385876473475934758934753
      #ifdef REALLY_SHOW_ME_THIS_7364894385876473475934758934753
      #ifndef NDEBUG
      for (int i=0; i!=sz; ++i)
      {
        std::stringstream s;
        s << "[" << (i+1) << "/" << sz << "]: ("
          << std::get<0>(v[i]) << "," << std::get<1>(v[i]) << "," << std::get<2>(v[i])
          << ") , ("
          << std::get<0>(w[i]) << "," << std::get<1>(w[i]) << "," << std::get<2>(w[i])
          << ")";
        TRACE(s);
      }
      #endif
      #endif
      return false;
    }
  }
  return true;
}

#ifndef NDEBUG
bool ribi::cmap::ConceptMap::IsValid() const noexcept
{
  for (const boost::shared_ptr<Edge>& edge: m_edges)
  {
    if (!edge)
    {
      TRACE("Edge is nullptr");
      return false;
    }
    if (!edge->GetTo())
    {
      TRACE("edge->GetTo() is nullptr");
      return false;
    }
    if (!edge->GetFrom())
    {
      TRACE("edge->GetFrom() is nullptr");
      return false;
    }
    if (container().Count(m_nodes,*edge->GetTo()) != 1)
    {
      TRACE("edge->GetTo() points to node not in the concept map");
      return false;
    }
    if(container().Count(m_nodes,*edge->GetFrom()) != 1)
    {
      TRACE("edge->GetFrom() points to node not in the concept map");
      return false;
    }
  }
  return true;
}
#endif

int ribi::cmap::CountCenterNodes(
  const ribi::cmap::ConceptMap::ReadOnlyConceptMapPtr& conceptmap
) noexcept
{
  const int cnt = CountCenterNodes(conceptmap->GetNodes());
  assert(cnt < 2 && "A concept map can have one or zero (a sub-conceptmap) center nodes");
  return cnt;
}

int ribi::cmap::CountCenterNodeEdges(
  const ribi::cmap::ConceptMap::ReadOnlyConceptMapPtr& conceptmap
) noexcept
{
  const int n_center_nodes{CountCenterNodes(conceptmap)};
  assert(n_center_nodes < 2 && "A concept map can have one or zero (a sub-conceptmap) center nodes");
  if (n_center_nodes == 0) return 0;
  const auto v = conceptmap->GetEdges();
  const int cnt{
    static_cast<int>(
      container().CountIf(v, //std::count_if(v.begin(),v.end(),
        [](const boost::shared_ptr<const Edge> edge)
        {
          return IsConnectedToCenterNode(edge);
        }
      )
    )
  };
  return cnt;
}

bool ribi::cmap::operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  const bool verbose{true};
  //Compare nodes
  {
    const auto lhs_nodes = lhs.GetNodes();
    const auto rhs_nodes = rhs.GetNodes();
    if (lhs_nodes.size() != rhs_nodes.size())
    {
      if (verbose) { TRACE("Number of nodes differs"); }
      return false;
    }
    if (!
      std::equal(
        std::begin(lhs_nodes),
        std::end(  lhs_nodes),
        std::begin(rhs_nodes),
        [](const Node& lhs_node,
           const Node& rhs_node)
        {
          return lhs_node == rhs_node;
        }
      )
    )
    {
      if (verbose) { TRACE("Node differs"); }
      return false;
    }
  }
  //Compare edges
  {
    const std::vector<boost::shared_ptr<const cmap::Edge>> lhs_edges = lhs.GetEdges();
    const std::vector<boost::shared_ptr<const cmap::Edge>> rhs_edges = rhs.GetEdges();
    if (lhs_edges.size() != rhs_edges.size())
    {
      if (verbose) { TRACE("Number of edges differ"); }
      return false;
    }
    if (!
      std::equal(
        std::begin(lhs_edges),
        std::end(  lhs_edges),
        std::begin(rhs_edges),
        [](const boost::shared_ptr<const cmap::Edge> lhs_edge,
           const boost::shared_ptr<const cmap::Edge> rhs_edge)
        {
          return *lhs_edge == *rhs_edge;
        }
      )
    )
    {
      if (verbose) { TRACE("Edge differs"); }
      return false;
    }
  }


  if (lhs.GetSelected() != rhs.GetSelected())
  {
    if (verbose) { TRACE("Selectedness differs"); }
    return false;
  }
  //if (lhs.m_undo != rhs.m_undo) return false; //Cannot do this :-(
  if (lhs.m_undo.count() != rhs.m_undo.count())
  {
    if (verbose) { TRACE("Undo stack differs"); }
    return false; //Proxy
  }
  /*
  return std::equal( //Does not work for QUndoStack :-(
    std::begin(lhs.m_undo),
    std::end(lhs.m_undo),
    std::begin(rhs.m_undo),
    [](boost::shared_ptr<const Command> p,
       boost::shared_ptr<const Command> q)
    {
      return p->text() == q->text();
    }
  );
  */
  return true;

}

bool ribi::cmap::operator!=(const ribi::cmap::ConceptMap& lhs, const ribi::cmap::ConceptMap& rhs) noexcept
{
  return !(lhs == rhs);
}




void ribi::cmap::ConceptMap::AddSelected(const std::vector<boost::shared_ptr<Edge>>& edges) noexcept
{
  assert(container().Count(edges,nullptr) == 0);
  std::copy(edges.begin(),edges.end(),std::back_inserter(m_selected.first));

  //Remove duplicates
  std::sort(std::begin(m_selected.first),std::end(m_selected.first));
  const auto new_end = std::unique(std::begin(m_selected.first),std::end(m_selected.first));
  m_selected.first.erase(new_end,std::end(m_selected.first));

  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::AddSelected(const Nodes& nodes) noexcept
{
  std::copy(std::begin(nodes),std::end(nodes),std::back_inserter(m_selected.second));

  //Remove duplicates
  std::sort(std::begin(m_selected.second),std::end(m_selected.second));
  const auto new_end = std::unique(std::begin(m_selected.second),std::end(m_selected.second));
  m_selected.second.erase(new_end,std::end(m_selected.second));

  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::AddSelected(
  const std::vector<boost::shared_ptr<Edge>>& edges,
  const Nodes& nodes
) noexcept
{
  AddSelected(edges);
  AddSelected(nodes);
}

boost::shared_ptr<ribi::cmap::ConceptMap> ribi::cmap::ConceptMap::CreateEmptyConceptMap() noexcept
{
  const boost::shared_ptr<ConceptMap> conceptmap {
    ConceptMapFactory().Create()
  };
  assert(conceptmap);
  return conceptmap;
}

boost::shared_ptr<ribi::cmap::Edge> ribi::cmap::ConceptMap::CreateNewEdge() noexcept
{
  assert(GetSelectedNodes().size() == 2);

  const Node from = GetSelectedNodes()[0];
  const Node to   = GetSelectedNodes()[1];
  const boost::shared_ptr<Edge> edge {
    EdgeFactory().Create(from,to)
  };

  //Add the Edge
  AddEdge(edge);

  //Signal the new Edge
  m_signal_add_edge(edge);

  //Keep track of what is selected
  this->AddSelected( { edge } );
  this->Unselect( { *edge->GetFrom() } );
  this->Unselect( { *edge->GetTo() } );

  return edge;
}

ribi::cmap::Node ribi::cmap::ConceptMap::CreateNewNode() noexcept
{
  #ifndef NDEBUG
  static std::string my_string = "A";
  const Node node {
    NodeFactory().CreateFromStrings(my_string)
  };
  ++my_string[0];
  #else
  const boost::shared_ptr<Node> node {
    NodeFactory().CreateFromStrings("...")
  };
  #endif

  //Add the Node
  AddNode(node);

  //Signal the new Node
  m_signal_add_node(node);

  //Adding a Node should select it
  this->AddSelected( { node } ); //Must be after 'm_signal_add_node(node);'

  return node;
}

void ribi::cmap::ConceptMap::DoCommand(Command * const command) noexcept
{
  #ifndef NDEBUG
  const int before{m_undo.count()};
  #endif //NDEBUG

  assert(command);

  //Push and, by this, do the command
  m_undo.push(command);

  #ifndef NDEBUG
  const int after{m_undo.count()};
  assert(after == before + 1);
  #endif // NDEBUG
}

std::vector<boost::shared_ptr<ribi::cmap::Edge>> ribi::cmap::ConceptMap::GetRandomEdges(
  std::vector<boost::shared_ptr<const Edge>> edges_to_exclude
) noexcept
{
  if (GetEdges().empty()) { return Edges(); }
  Edges edges = GetEdges();
  const auto cmp = [](const boost::shared_ptr<const Edge>& lhs, const boost::shared_ptr<const Edge>& rhs) { return lhs.get() < rhs.get(); };
  std::sort(std::begin(edges),std::end(edges),cmp);
  std::sort(std::begin(edges_to_exclude),std::end(edges_to_exclude),cmp);
  //Find the Edges present in nodes, absent in nodes_to_exclude
  Edges focus_edges;
  std::set_difference(
    std::begin(edges),std::end(edges),
    std::begin(edges_to_exclude),std::end(edges_to_exclude),
    std::back_inserter(focus_edges),
    cmp
  );

  if (focus_edges.empty()) return focus_edges;
  if (focus_edges.size() == 1) return focus_edges;
  std::random_shuffle(focus_edges.begin(),focus_edges.end());
  const int n = 1 + (std::rand() % (focus_edges.size() - 1));
  focus_edges.resize(n);
  return focus_edges;
}

boost::shared_ptr<ribi::cmap::Edge> ribi::cmap::ConceptMap::GetRandomEdge(
  std::vector<boost::shared_ptr<const Edge>> edges_to_exclude) noexcept
{
  const auto v(GetRandomEdges(edges_to_exclude));
  boost::shared_ptr<Edge> p;
  if (!v.empty()) p = v[0];
  return p;
}

ribi::cmap::ConceptMap::Nodes ribi::cmap::ConceptMap::GetRandomNodes(
  Nodes nodes_to_exclude
) noexcept
{
  if (GetNodes().empty()) { return Nodes(); }
  Nodes nodes = GetNodes();
  std::sort(nodes.begin(),nodes.end());
  std::sort(nodes_to_exclude.begin(),nodes_to_exclude.end());
  //Find the Nodes present in nodes, absent in nodes_to_exclude
  Nodes focus_nodes;
  std::set_difference(
    nodes.begin(),nodes.end(),
    nodes_to_exclude.begin(),nodes_to_exclude.end(),
    std::back_inserter(focus_nodes)
  );

  if (focus_nodes.empty()) return focus_nodes;
  if (focus_nodes.size() == 1) return focus_nodes;
  std::random_shuffle(focus_nodes.begin(),focus_nodes.end());
  const int n = 1 + (std::rand() % (focus_nodes.size() - 1));
  while (n != static_cast<int>(focus_nodes.size()))
  {
    assert(!focus_nodes.empty());
    focus_nodes.pop_back();
  }
  return focus_nodes;
}

ribi::cmap::Node ribi::cmap::ConceptMap::GetRandomNode(
  const Nodes& nodes_to_exclude
)
{
  const auto v(GetRandomNodes(nodes_to_exclude));
  if (v.empty())
  {
    std::stringstream msg;
    msg << __func__ << ": no other nodes present";
    throw std::logic_error(msg.str());
  }
  return v[0];
}

bool ribi::cmap::ConceptMap::IsSelected(const ReadOnlyEdgePtr& edge) const noexcept
{
  const auto iter = std::find(
    std::begin(m_selected.first),
    std::end(m_selected.first),
    edge
  );
  if (iter != std::end(m_selected.first)) return true;
  return false;
}

bool ribi::cmap::ConceptMap::IsSelected(const ReadOnlyNodePtr& node) const noexcept
{
  const auto iter = std::find(
    std::begin(m_selected.second),
    std::end(m_selected.second),
    node
  );
  if (iter != std::end(m_selected.second)) return true;
  const auto edge = this->GetEdgeHaving(node);
  return edge ? true : false;
}

void ribi::cmap::ConceptMap::OnUndo() noexcept
{
  assert(m_undo.canUndo());
  m_undo.undo();
}

/*
void ribi::cmap::ConceptMap::SetSelected(
  const ConstEdges& edges,
  const Nodes& nodes
) noexcept
{
  SetSelected(RemoveConst(edges),nodes);
}
*/

void ribi::cmap::ConceptMap::Redo() noexcept
{
  assert(m_undo.canRedo());
  m_undo.redo();
}

void ribi::cmap::ConceptMap::RemoveSelected(const std::vector<boost::shared_ptr<Edge>>& edges) noexcept
{
  if (GetVerbosity())
  {
    std::clog << "ribi::cmap::ConceptMap::RemoveSelected of "
      << edges.size() << " edges"
    ;
  }

  assert(std::count(std::begin(edges),std::end(edges),nullptr) == 0);
  for (const auto edge: edges)
  {
    const auto new_end = std::remove(
      std::begin(m_selected.first),
      std::end(m_selected.first),
      edge
    );
    if (new_end == std::end(m_selected.first) && m_verbose)
    {
      std::clog << "Warning: edge to be removed not found" << std::endl;
    }
    m_selected.first.erase(new_end,std::end(m_selected.first));
  }
  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::RemoveSelected(
  const Nodes& nodes
) noexcept
{
  if (GetVerbosity())
  {
    std::clog << "ribi::cmap::ConceptMap::RemoveSelected of "
      << nodes.size() << " nodes"
    ;
  }

  //Take care: it may be node at the center of an edge
  for (const auto node: nodes)
  {
    const auto new_end = std::remove(
      std::begin(m_selected.second),
      std::end(m_selected.second),
      node
    );
    if (new_end == std::end(m_selected.second))
    {
      const auto edge = this->GetEdgeHaving(node);
      if (!edge)
      {
        if (m_verbose) { std::clog << "Warning: cannot find node to unselect" << std::endl; }
      }
      else
      {
        if (m_verbose) { std::clog << "Removing selectedness of edge containing node" << std::endl; }
        this->RemoveSelected( Edges( { edge } ) );
      }
    }
    else
    {
      if (m_verbose)
      {
        std::clog << "Removing selectedness of node" << std::endl;
      }
      m_selected.second.erase(new_end,std::end(m_selected.second));
    }
  }
  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::RemoveSelected(
  const std::vector<boost::shared_ptr<Edge>>& edges,
  const Nodes& nodes
) noexcept
{
  RemoveSelected(edges);
  RemoveSelected(nodes);
}

void ribi::cmap::ConceptMap::SetSelected(
  const Edges& edges,
  const Nodes& nodes
) noexcept
{
  if (GetVerbosity())
  {
    std::clog << "ribi::cmap::ConceptMap::SetSelected of "
      << edges.size() << " edges and "
      << nodes.size() << " nodes"
      << '\n'
    ;
  }
  m_selected.first = edges;
  m_selected.second = nodes;
  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::SetSelected(const Edges& edges) noexcept
{
  SetSelected(edges, {});
}

void ribi::cmap::ConceptMap::SetSelected(const Nodes& nodes) noexcept
{
  SetSelected( {}, nodes);
}

void ribi::cmap::ConceptMap::SetSelected(const EdgesAndNodes& nodes_and_edges) noexcept
{
  SetSelected(nodes_and_edges.first, nodes_and_edges.second);
}

void ribi::cmap::ConceptMap::SetSelected(const ConstEdgesAndNodes& nodes_and_edges) noexcept
{
  SetSelected(RemoveConst(nodes_and_edges.first), nodes_and_edges.second);
}

std::string ribi::cmap::ToXml(const ConceptMap& map) noexcept
{
  std::stringstream s;
  s << "<concept_map>";
  s << "<nodes>";
  const auto nodes = map.GetNodes();
  for (const auto& node: nodes)
  {
    s << node.ToXml();
  }
  s << "</nodes>";
  s << "<edges>";
  for (const auto edge: map.GetEdges())
  {
    s << Edge::ToXml(edge,nodes);
  }
  s << "</edges>";
  s << "</concept_map>";

  const std::string r = s.str();
  return r;
}

void ribi::cmap::ConceptMap::Undo() noexcept
{
  assert(m_undo.canUndo());
  m_undo.undo();
}

void ribi::cmap::ConceptMap::Unselect(
  const ConstEdgesAndNodes& edges_and_nodes
) noexcept
{
  Unselect(edges_and_nodes.first);
  Unselect(edges_and_nodes.second);
}

void ribi::cmap::ConceptMap::Unselect(
  const ConstEdges& edges
) noexcept
{
  for (const auto edge: edges)
  {
    m_selected.first.erase(
      std::remove(
        std::begin(m_selected.first),
        std::end(m_selected.first),
        edge
      ),
      std::end(m_selected.first)
    );
  }

  m_signal_selected_changed(m_selected);
}

void ribi::cmap::ConceptMap::Unselect(
  const Nodes& nodes
) noexcept
{
  for (const auto node: nodes)
  {
    m_selected.second.erase(
      std::remove(
        std::begin(m_selected.second),
        std::end(m_selected.second),
        node
      ),
      std::end(m_selected.second)
    );
  }
  m_signal_selected_changed(m_selected);
}
