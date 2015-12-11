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

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapfactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::ConceptMap::ConceptMap(const std::string& question) noexcept
  : m_edges( {} ),
    m_nodes(CreateNodes(question, {} )),
    m_selected{},
    //m_undo{},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  assert(CanConstruct(m_nodes,m_edges));
  assert(FindCenterNode()
    && "Assume a CenterNode at the center of ConceptMap");
  assert(IsCenterNode(*FindCenterNode())
    && "The CenterNode found must really be a CenterNode");
  assert(FindCenterNode()->GetConcept().GetName() == question
    && "The CenterNode must display the focus");
  #endif
}

ribi::cmap::ConceptMap::ConceptMap(
  Nodes& nodes,
  Edges& edges
)  noexcept
  : m_edges{},
    m_nodes{},
    m_selected{},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  std::swap(nodes,m_nodes);
  std::swap(edges,m_edges);
  if (!CanConstruct(nodes,edges))
  {
    TRACE("ERROR");
    const std::size_t n_nodes = nodes.size();
    for (std::size_t i=0; i!=n_nodes; ++i)
    {
      std::cout << i << ": " << ToXml(nodes[i]) << '\n';
    }

    const std::size_t n_edges = edges.size();
    for (std::size_t i=0; i!=n_edges; ++i)
    {
      const auto edge = edges[i];
      const auto from_iter = std::find(std::begin(nodes),std::end(nodes),*edge.GetFrom());
      const auto to_iter = std::find(std::begin(nodes),std::end(nodes),*edge.GetTo());
      assert(from_iter != nodes.end());
      assert(to_iter != nodes.end());
      std::cout
        << i << ": "
        << Edge::ToXml(edge,nodes)
        << '\n';
    }
  }
  assert(CanConstruct(nodes,edges));
  assert(IsValid());
  #endif
}

ribi::cmap::ConceptMap::ConceptMap(const ConceptMap& rhs)
  : m_edges{},
    m_nodes{},
    m_selected{},
    m_verbose{false}
{
  using EdgesAndNodes = std::tuple<std::vector<Edge>,std::vector<Node>>;
  assert(rhs.IsValid());
  assert(CanConstruct(rhs.GetNodes(),rhs.GetEdges()));
  const std::string xml{ToXml(rhs)};
  const EdgesAndNodes t{ConceptMapFactory().FromXmlAsTuple(xml)};
  assert(CanConstruct(std::get<1>(t),std::get<0>(t)));
  m_edges = std::get<0>(t);
  m_nodes = std::get<1>(t);
  assert(CanConstruct(m_nodes,m_edges));
  assert(IsValid());
}

ribi::cmap::ConceptMap::ConceptMap(ConceptMap&& rhs)
  : m_edges{std::move(rhs.m_edges)},
    m_nodes{std::move(rhs.m_nodes)},
    m_selected{std::move(rhs.m_selected)},
    m_verbose{std::move(rhs.m_verbose)}
{
  assert(IsValid());
}

ribi::cmap::ConceptMap& ribi::cmap::ConceptMap::operator=(const ConceptMap& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  m_selected = {};
  m_verbose = rhs.m_verbose;
  const auto t = ConceptMapFactory().FromXmlAsTuple(ToXml(rhs));
  m_edges = std::get<0>(t);
  m_nodes = std::get<1>(t);
  assert(IsValid());
  return *this;
}

ribi::cmap::ConceptMap& ribi::cmap::ConceptMap::operator=(ConceptMap&& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  m_selected = std::move(rhs.m_selected);
  m_verbose = std::move(rhs.m_verbose);
  m_edges = std::move(rhs.m_edges);
  m_nodes = std::move(rhs.m_nodes);
  assert(IsValid());
  return *this;
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

  const std::vector<ribi::cmap::Concept>& v = cluster->Get();
  const int n = boost::numeric_cast<int>(v.size());
  for (int i=0; i!=n; ++i)
  {
    const int x = 0;
    const int y = 0;
    const Node node = NodeFactory().Create(v[i],x,y);
    assert(node);
    m_nodes.push_back(node);
  }
  assert(v.size() + 1 == m_nodes.size()
    && "Assume the ConceptMap has as much nodes as the cluster has concepts + one focal question");
}
#endif //TO_ADD_TO_PROJECTBRAINWEAVER

void ribi::cmap::ConceptMap::AddEdge(const Edge& edge) noexcept
{
  if (std::find(std::begin(m_edges),std::end(m_edges),edge) != std::end(m_edges))
  {
    if (m_verbose) { TRACE("Warning: edge already added"); }
    return;
  }

  assert(edge.GetFrom());
  assert(edge.GetTo());

  //Add Nodes if they are not present yet
  //if (!HasNode(edge.GetFrom())) { AddNode(edge.GetFrom()); }
  //if (!HasNode(edge.GetTo())  ) { AddNode(edge.GetTo()  ); }


  assert(HasNode(*edge.GetFrom()));
  assert(HasNode(*edge.GetTo()));

  assert(std::count(std::begin(m_nodes),std::end(m_nodes),*edge.GetFrom()) == 1
    && "First enter the node this edge originates from");
  assert(std::count(std::begin(m_nodes),std::end(m_nodes),*edge.GetTo()) == 1
    && "First enter the node this edge targets to");
  m_edges.push_back(edge);

  this->SetSelected( Edges( { edge } ) );
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


bool ribi::cmap::CanConstruct(
  const std::vector<Node>& nodes,
  const std::vector<Edge>& edges,
  const bool verbose
) noexcept
{
  //Test if there are 'two-way' edges, that is, one edge going from A to B
  //and another edge going from B to A
  {
    const int n_edges{static_cast<int>(edges.size())};
    for (int i{0}; i!=n_edges; ++i)
    {
      const Edge& edge_a = edges[i];
      assert(&edge_a == &edges[i]);
      if (!edge_a.GetFrom())
      {
        if (verbose) { TRACE("edge_a.GetFrom() is nullptr"); }
        return false;
      }
      if (!edge_a.GetTo())
      {
        if (verbose) { TRACE("edge_a.GetTo() is nullptr"); }
        return false;
      }
      const Node *a_from{edge_a.GetFrom()};
      const Node *a_to  {edge_a.GetTo()  };
      assert(a_from == edge_a.GetFrom());
      assert(a_to   == edge_a.GetTo());

      for (int j{i+1}; j!=n_edges; ++j)
      {
        assert(i != j);
        assert(j < n_edges);
        const Edge& edge_b{edges[j]};
        assert(&edge_b == &edges[j]);
        if (!edge_b.GetFrom())
        {
          if (verbose) { TRACE("edge_b.GetFrom() is nullptr"); }
          return false;
        }
        if (!edge_b.GetTo())
        {
          if (verbose) { TRACE("edge_b.GetTo() is nullptr"); }
          return false;
        }
        const Node *b_from{edge_b.GetFrom()};
        const Node *b_to  {edge_b.GetTo()};
        assert(b_from == edge_b.GetFrom());
        assert(b_to   == edge_b.GetTo());
        if (a_from == b_from && a_to == b_to)
        {
          if (verbose) { TRACE("Cannot have two edges from the same node to the same node"); }
          return false;
        }
        if (a_from == b_to && a_to == b_from)
        {
          if (verbose) { TRACE("Cannot have two edges from the same node to the same node"); }
          return false;
        }
      }
    }
  }
  const int n_edges{static_cast<int>(edges.size())};
  for (int i{0}; i!=n_edges; ++i)
  {
    bool from_found{false};
    bool to_found{false};
    const int n_nodes{static_cast<int>(nodes.size())};
    //Read all addresses of the nodes
    for (int j{0}; j!=n_nodes; ++j) {
      if (edges[i].GetTo()   == &nodes[j]) { to_found = true; }
      if (edges[i].GetFrom() == &nodes[j]) { from_found = true; }
    }
    if (!from_found)
    {
      if (verbose) { TRACE("edge.GetTo() points to node not in the concept map"); }
      TRACE("ERROR");
      for (const auto& n: nodes) { TRACE(&n); TRACE(n);}
      TRACE(edges[i]);
      TRACE("~ERROR");
      return false;
    }
    if (!to_found)
    {
      if (verbose) { TRACE("edge.GetFrom() points to node not in the concept map"); }
      TRACE("ERROR");
      for (const auto& n: nodes) { TRACE(&n); TRACE(n);}
      TRACE(edges[i]);
      TRACE("~ERROR");
      return false;
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
  const Node center_node {
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

    for (const Edge& focal_edge: m_edges)
    {
     if (focal_edge.GetFrom() == &focal_node)
      {
        edges.push_back(focal_edge);
        assert(focal_edge.GetTo() != &focal_node);
        nodes.push_back(*focal_edge.GetTo());
      }
      else if (focal_edge.GetTo() == &focal_node)
      {
        edges.push_back(focal_edge);
        assert(focal_edge.GetFrom() != &focal_node);
        nodes.push_back(*focal_edge.GetFrom());
      }
    }
    assert(!nodes.empty());
    //Put CenterNode in front
    //const auto iter = container().FindIf(nodes,[](const auto& node) { return IsCenterNode(node); } );
    const auto iter = std::find_if(std::begin(nodes),std::end(nodes),[](const Node& node) { return IsCenterNode(node); } );
    if (iter != nodes.end()) { std::swap(nodes.front(),*iter); }
    assert(CanConstruct(nodes,edges) && "Only construct valid concept maps");
    const ConceptMap conceptmap(nodes,edges);
    assert(conceptmap.IsValid());
    v.push_back(conceptmap);
  }
  return v;
}

void ribi::cmap::ConceptMap::DeleteEdge(const Edge& edge) noexcept
{
  #ifndef NDEBUG
  //One cannot be sure if the edges are already deleted or not
  if(std::count(begin(m_edges),end(m_edges),edge) == 0) return;
  assert(std::count(begin(m_edges),end(m_edges),edge) != 0
    && "The edge must exist");
  assert(std::count(begin(m_edges),end(m_edges),edge) == 1
    && "Every edge is unique");
  const std::size_t n_edges_before = m_edges.size();
  #endif



  ///Edge might already be deleted by something else
  if (GetEdges().empty()) return;
  {
    const auto& edges = GetEdges();
    if (std::find(begin(edges),end(edges),edge) == edges.end()) return;
  }

  m_edges.erase(std::remove(begin(m_edges),end(m_edges),edge),m_edges.end());

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

  for (const auto& edge: to_be_deleted)
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
  this->Unselect( { node } );

  //If there is no node selected anymore, give focus to a possible first node
  if (GetSelectedNodes().empty() && !GetNodes().empty())
  {
    this->SetSelected(Nodes( { GetNodes().front() } ) );
  }
}

bool ribi::cmap::ConceptMap::Empty() const noexcept
{
  assert(!(m_nodes.empty() && !m_edges.empty())
    && "If there are no nodes, there cannot be edges");
  return m_nodes.empty(); // && m_edges.empty();
}

const ribi::cmap::Node* ribi::cmap::ConceptMap::FindCenterNode() const noexcept
{
  const auto iter = ribi::cmap::FindCenterNode(m_nodes);
  return iter == std::end(m_nodes) ? nullptr : &(*iter);
}

ribi::cmap::Node* ribi::cmap::ConceptMap::FindCenterNode() noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  const Node* center_node {
    const_cast<const ConceptMap*>(this)->FindCenterNode() //Add const because compiler cannt find the right version
    //?used to be dynamic_cast?
  };
  return const_cast<Node*>(center_node);
}

const ribi::cmap::Edge * ribi::cmap::ConceptMap::GetEdgeHaving(const Node& node) const noexcept
{
  auto iter = Container().FindIf(
    m_edges,
    [node](const Edge& edge) { return edge.GetNode() == node; }
  );
  if (iter == std::end(m_edges)) return nullptr;
  return &(*iter);
}

ribi::cmap::ConceptMap::Edges ribi::cmap::ConceptMap::GetEdgesConnectedTo(const Node& node) const noexcept
{
  Edges edges;
  std::copy_if(
    std::begin(m_edges),
    std::end(m_edges),
    std::back_inserter(edges),
    [node](const Edge& edge)
    {
      return edge.GetFrom() == &node || edge.GetTo() == &node;
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

bool ribi::cmap::ConceptMap::HasEdge(const Edge& edge) const noexcept
{
  return container().Count(m_edges,edge);
}

bool ribi::cmap::ConceptMap::HasNode(const Node& node) const noexcept
{
  return container().Count(m_nodes,node);
}

bool ribi::cmap::HasSameContent(
  const ConceptMap& lhs,
  const ConceptMap& rhs
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
    const std::vector<Edge> edges_lhs = lhs.GetEdges();
    std::multiset<Concept> concepts_lhs;
    std::transform(std::begin(edges_lhs),std::end(edges_lhs),
      std::inserter(concepts_lhs,std::begin(concepts_lhs)),
      [](const Edge& edge)
      {
        return edge.GetNode().GetConcept();
      }
    );

    const std::vector<Edge> edges_rhs = rhs.GetEdges();

    std::multiset<Concept> concepts_rhs;
    std::transform(begin(edges_rhs),end(edges_rhs),
      std::inserter(concepts_rhs,std::begin(concepts_rhs)),
      [](const Edge& edge)
      {
        return edge.GetNode().GetConcept();
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
      assert(i >= 0);
      assert(i < static_cast<int>(lhs.GetEdges().size()));
      const auto from_node = lhs.GetEdges()[i].GetFrom();
      assert(from_node);
      const std::string str_from = from_node->GetConcept().GetName();
      const std::string str_mid = lhs.GetEdges()[i].GetNode().GetConcept().GetName();
      const auto to_node = lhs.GetEdges()[i].GetTo();
      assert(to_node);
      const std::string str_to = to_node->GetConcept().GetName();
      //Only if arrow is reversed, reverse the fake edge
      if (
           lhs.GetEdges()[i].HasTailArrow() == true
        && lhs.GetEdges()[i].HasHeadArrow() == false)
      {
        v.push_back(std::make_tuple(str_to,str_mid,str_from));
      }
      else if (lhs.GetEdges()[i].HasTailArrow() == lhs.GetEdges()[i].HasHeadArrow())
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
      const auto from_node = rhs.GetEdges()[i].GetFrom();
      const std::string str_from = from_node->GetConcept().GetName();
      const std::string str_mid = rhs.GetEdges()[i].GetNode().GetConcept().GetName();
      const auto to_node = rhs.GetEdges()[i].GetTo();
      const std::string str_to = to_node->GetConcept().GetName();
      //w.push_back(std::make_tuple(str_from,str_mid,str_to));
      //Only if arrow is reversed, reverse the fake edge
      if (
           rhs.GetEdges()[i].HasTailArrow() == true
        && rhs.GetEdges()[i].HasHeadArrow() == false)
      {
        w.push_back(std::make_tuple(str_to,str_mid,str_from));
      }
      else if (rhs.GetEdges()[i].HasTailArrow() == rhs.GetEdges()[i].HasHeadArrow())
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
    if (v != w) { return false; }
  }
  return true;
}

#ifndef NDEBUG
bool ribi::cmap::ConceptMap::IsValid() const noexcept
{
  return CanConstruct(m_nodes,m_edges);
}
#endif

int ribi::cmap::CountCenterNodes(const ConceptMap& conceptmap) noexcept
{
  const int cnt = CountCenterNodes(conceptmap.GetNodes());
  assert(cnt < 2 && "A concept map can have one or zero (a sub-conceptmap) center nodes");
  return cnt;
}

int ribi::cmap::CountCenterNodeEdges(
  const ConceptMap& conceptmap
) noexcept
{
  const int n_center_nodes{CountCenterNodes(conceptmap)};
  assert(n_center_nodes < 2 && "A concept map can have one or zero (a sub-conceptmap) center nodes");
  if (n_center_nodes == 0) return 0;
  const auto v = conceptmap.GetEdges();
  const int cnt{
    static_cast<int>(
      container().CountIf(v, //std::count_if(v.begin(),v.end(),
        [](const Edge& edge)
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
    const auto& lhs_edges = lhs.GetEdges();
    const auto& rhs_edges = rhs.GetEdges();
    if (lhs_edges.size() != rhs_edges.size())
    {
      if (verbose) { TRACE("Number of edges differ"); }
      return false;
    }
    if (!
      std::equal(
        std::begin(lhs_edges),
        std::end(  lhs_edges),
        std::begin(rhs_edges)
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
  return true;

}

bool ribi::cmap::operator!=(const ribi::cmap::ConceptMap& lhs, const ribi::cmap::ConceptMap& rhs) noexcept
{
  return !(lhs == rhs);
}




void ribi::cmap::ConceptMap::AddSelected(const std::vector<Edge>& edges) noexcept
{
  std::copy(edges.begin(),edges.end(),std::back_inserter(m_selected.first));

  //Remove duplicates
  std::sort(std::begin(m_selected.first),std::end(m_selected.first));
  const auto new_end = std::unique(std::begin(m_selected.first),std::end(m_selected.first));
  m_selected.first.erase(new_end,std::end(m_selected.first));
}

void ribi::cmap::ConceptMap::AddSelected(const Nodes& nodes) noexcept
{
  std::copy(std::begin(nodes),std::end(nodes),std::back_inserter(m_selected.second));

  //Remove duplicates
  std::sort(std::begin(m_selected.second),std::end(m_selected.second));
  const auto new_end = std::unique(std::begin(m_selected.second),std::end(m_selected.second));
  m_selected.second.erase(new_end,std::end(m_selected.second));
}

void ribi::cmap::ConceptMap::AddSelected(
  const std::vector<Edge>& edges,
  const Nodes& nodes
) noexcept
{
  AddSelected(edges);
  AddSelected(nodes);
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMap::CreateEmptyConceptMap() noexcept
{
  const ConceptMap conceptmap {
    ConceptMapFactory().Create()
  };
  return conceptmap;
}

ribi::cmap::Edge ribi::cmap::ConceptMap::CreateNewEdge() noexcept
{
  assert(GetSelectedNodes().size() == 2);

  const Node from = GetSelectedNodes()[0];
  const Node to   = GetSelectedNodes()[1];
  const Edge edge(EdgeFactory().Create(from,to));

  //Add the Edge
  AddEdge(edge);

  //Keep track of what is selected
  this->AddSelected( { edge } );
  this->Unselect( { *(edge.GetFrom()) } );
  this->Unselect( { *(edge.GetTo()) } );

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
  const Node node {
    NodeFactory().CreateFromStrings("...")
  };
  #endif

  //Add the Node
  AddNode(node);

  //Adding a Node should select it
  this->AddSelected( { node } ); //Must be after 'm_signal_add_node(node);'

  return node;
}

std::vector<ribi::cmap::Edge> ribi::cmap::ConceptMap::GetRandomEdges(
  std::vector<Edge> edges_to_exclude
) noexcept
{
  if (GetEdges().empty()) { return Edges(); }
  Edges edges = GetEdges();
  const auto cmp = [](const Edge& lhs, const Edge& rhs) {
    return lhs.GetNode() < rhs.GetNode();
  };
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
  while (n < static_cast<int>(focus_edges.size())) {
    assert(!focus_edges.empty());
    focus_edges.pop_back();
  }
  //focus_edges.resize(n);
  return focus_edges;
}

ribi::cmap::Edge ribi::cmap::ConceptMap::GetRandomEdge(
  std::vector<Edge> edges_to_exclude)
{
  const auto v(GetRandomEdges(edges_to_exclude));
  if (v.empty())
  {
    std::stringstream msg;
    msg << __func__ << ": no random edge found";
    throw std::logic_error(msg.str());
  }
  assert(!v.empty());
  return v[0];
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

bool ribi::cmap::ConceptMap::IsSelected(const Edge& edge) const noexcept
{
  const auto iter = std::find(
    std::begin(m_selected.first),
    std::end(m_selected.first),
    edge
  );
  if (iter != std::end(m_selected.first)) return true;
  return false;
}

bool ribi::cmap::ConceptMap::IsSelected(const Node& node) const noexcept
{
  const int n_found_in_nodes = Container().Count(
    m_selected.second,
    node
  );
  if (n_found_in_nodes) return true;

  const int n_found_in_edge = Container().CountIf(
    m_selected.first,
    [node](const Edge& edge) { return edge.GetNode() == node; }
  );
  if (n_found_in_edge) return true;
  return false;
}

void ribi::cmap::ConceptMap::RemoveSelected(const std::vector<Edge>& edges) noexcept
{
  if (GetVerbosity())
  {
    std::clog << "ribi::cmap::ConceptMap::RemoveSelected of "
      << edges.size() << " edges"
    ;
  }

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
      const Edge* const edge = this->GetEdgeHaving(node);
      if (edge == nullptr)
      {
        if (m_verbose) { std::clog << "Warning: cannot find node to unselect" << std::endl; }
      }
      else
      {
        if (m_verbose) { std::clog << "Removing selectedness of edge containing node" << std::endl; }
        this->RemoveSelected( Edges( { *edge } ) );
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
}

void ribi::cmap::ConceptMap::RemoveSelected(
  const std::vector<Edge>& edges,
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

std::string ribi::cmap::ToXml(const ConceptMap& map) noexcept
{
  assert(map.IsValid());
  assert(CanConstruct(map.GetNodes(),map.GetEdges()));
  std::stringstream s;
  s << "<concept_map>";
  s << "<nodes>";
  const std::vector<Node>& nodes{map.GetNodes()};
  assert(&nodes == &map.GetNodes());
  assert(CanConstruct(nodes,map.GetEdges(),true));
  assert(CanConstruct(nodes, {} ));
  assert(CanConstruct(nodes,map.GetEdges(),true));
  const int n_nodes{static_cast<int>(nodes.size())};
  for (int i=0; i!=n_nodes; ++i)
  {
    assert(CanConstruct(nodes,map.GetEdges(),true));
    assert(&nodes[i] == &map.GetNodes()[i]);
    assert(i >= 0);
    assert(i < static_cast<int>(nodes.size()));
    s << ToXml(nodes[i]); //ToXml is a noexcept free function
    assert(&nodes[i] == &map.GetNodes()[i]);
    TRACE(map);
    assert(CanConstruct(nodes,map.GetEdges(),true)); //Crashes here
  }

//  std::for_each(
//    std::begin(nodes),std::end(nodes),
//    [&s,&nodes,&map](const Node& node)
//    {
//      assert(CanConstruct(nodes,map.GetEdges(),true));
//      s << node.ToXml();
//      assert(CanConstruct(nodes,map.GetEdges(),true));
//    }
//  );
//  for (const Node& node: nodes)
//  {
//    assert(CanConstruct(nodes,map.GetEdges(),true));
//    s << node.ToXml();
//    assert(CanConstruct(nodes,map.GetEdges(),true));
//  }
  s << "</nodes>";
  s << "<edges>";
  assert(CanConstruct(nodes,map.GetEdges(),true));
  const std::vector<Edge>& edges = map.GetEdges();
  assert(&edges == &map.GetEdges());
  assert(&nodes == &map.GetNodes());
  assert(edges.empty() || &edges[0] == &map.GetEdges()[0]);
  assert(nodes.empty() || &nodes[0] == &map.GetNodes()[0]);
  assert(CanConstruct(nodes,map.GetEdges(),true)); //Crashed here, due to {} initialization of reference
  assert(CanConstruct(nodes,edges,true));
  for (const Edge& edge: edges)
  {
    assert(container().Count(edges,edge) == 1);
    assert(CanConstruct(nodes, { edge } ));
    s << Edge::ToXml(edge,nodes);
  }
  s << "</edges>";
  s << "</concept_map>";

  const std::string r = s.str();
  return r;
}

void ribi::cmap::ConceptMap::Unselect(
  const EdgesAndNodes& edges_and_nodes
) noexcept
{
  Unselect(edges_and_nodes.first);
  Unselect(edges_and_nodes.second);
}

void ribi::cmap::ConceptMap::Unselect(
  const Edges& edges
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
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const ConceptMap& m) noexcept
{
  const int n_nodes{static_cast<int>(m.m_nodes.size())};
  for (int i=0; i!=n_nodes; ++i) {
    os << "node [" << i << "] is at address " << &m.m_nodes[i] << '\n';
  }
  const int n_edges{static_cast<int>(m.m_edges.size())};
  for (int i=0; i!=n_edges; ++i) {
    os << "edge [" << i << "] goes from "
      << &(*m.m_edges[i].GetFrom())
      << " to "
      << &(*m.m_edges[i].GetTo())
      << '\n'
    ;
  }
  return os;
}
