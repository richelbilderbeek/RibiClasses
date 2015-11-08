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
#include "conceptmapedge.h"

#include <sstream>

#include <boost/lambda/lambda.hpp>

#include "counter.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"

#include "conceptmapconceptfactory.h"
#include "conceptmaphelper.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::Edge::Edge(
  const Node& node,
  const Node& from,
  const bool tail_arrow,
  const Node& to,
  const bool head_arrow)
  : m_signal_from_changed{},
    m_signal_head_arrow_changed{},
    m_signal_node_changed{},
    m_signal_tail_arrow_changed{},
    m_signal_to_changed{},
    m_from(&from),
    m_head_arrow(head_arrow),
    m_node(node),
    m_tail_arrow(tail_arrow),
    m_to(&to)
{
  #ifndef NDEBUG
  Test();
  #endif
  assert(m_from);
  assert(m_to);
  assert(m_from !=  m_to);
  assert(m_from != &m_node);
  assert(m_to   != &m_node);

  //Subscribe to all Concept signals to re-emit m_signal_edge_changed
  //this->m_node->m_signal_concept_changed.connect(
  //  boost::bind(&ribi::cmap::Edge::OnConceptChanged,this,boost::lambda::_1)
  //);

//  this->m_from->m_signal_x_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );
//  this->m_from->m_signal_y_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );
//  this->m_from->m_signal_concept_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );

//  this->m_to->m_signal_x_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );
//  this->m_to->m_signal_y_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );
//  this->m_to->m_signal_concept_changed.connect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );

}

ribi::cmap::Edge::~Edge() noexcept
{
//  this->m_node->m_signal_concept_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnConceptChanged,this)
//  );

//  this->m_from->m_signal_x_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );
//  this->m_from->m_signal_y_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );
//  this->m_from->m_signal_concept_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnFromChanged,this,boost::lambda::_1)
//  );

//  this->m_to->m_signal_x_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );
//  this->m_to->m_signal_y_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );
//  this->m_to->m_signal_concept_changed.disconnect(
//    boost::bind(&ribi::cmap::Edge::OnToChanged,this,boost::lambda::_1)
//  );

}

std::string ribi::cmap::Edge::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::cmap::Edge::GetVersionHistory() noexcept
{
  return {
    "2013-xx-xx: Version 1.0: initial version",
    "2014-06-01: Version 1.1: replaced Concept, X and Y by a Node"
    "2015-11-07: Version 1.2: use Node instead of a Node smart pointer"
  };
}


void ribi::cmap::Edge::OnConceptChanged(Node * const
#ifndef NDEBUG
  node
#endif
) noexcept
{
  assert(*node == GetNode());
  this->m_signal_node_changed(this);
}

void ribi::cmap::Edge::OnFromChanged(Node * const
#ifndef NDEBUG
  node
#endif
) noexcept
{
  const bool verbose{false};
  if (verbose) { TRACE("Slot ribi::cmap::Edge::OnFromChanged"); }
  assert(node == this->GetFrom());
  if (verbose) { TRACE("Emitting ribi::cmap::Edge::m_signal_from_changed"); }
  this->m_signal_from_changed(this);
}

void ribi::cmap::Edge::OnToChanged(Node * const
#ifndef NDEBUG
  node
#endif
) noexcept
{
  const bool verbose{false};
  if (verbose) { TRACE("Slot ribi::cmap::Edge::OnFromChanged"); }

  assert(node == this->GetTo());
  if (verbose) { TRACE("Emitting ribi::cmap::Edge::m_signal_to_changed"); }

  this->m_signal_to_changed(this);
}

void ribi::cmap::Edge::SetNode(const NodePtr& node) noexcept
{
  assert(node);
  if (m_node != *node)
  {
    m_node = *node;
    m_signal_node_changed(this);
  }
}

void ribi::cmap::Edge::SetHeadArrow(const bool has_head_arrow) noexcept
{
  if (m_head_arrow != has_head_arrow)
  {
    m_head_arrow = has_head_arrow;
    m_signal_head_arrow_changed(this);
  }
}

void ribi::cmap::Edge::SetTailArrow(const bool has_tail_arrow) noexcept
{
  if (m_tail_arrow != has_tail_arrow)
  {
    m_tail_arrow = has_tail_arrow;
    m_signal_tail_arrow_changed(this);
  }
}


#ifndef NDEBUG
void ribi::cmap::Edge::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  Node from{NodeFactory().GetTest(0)};
  Node to{NodeFactory().GetTest(1)};
  const std::vector<Node> nodes = {from,to};
  if (verbose) { TRACE("Operator=="); }
  {
    const auto edge1 = EdgeFactory().GetTest(0,from,to);
    const auto edge2 = EdgeFactory().GetTest(0,from,to);
    assert( edge1 !=  edge2);
    assert(*edge1 == *edge2);
  }
  if (verbose) { TRACE("Operator!="); }
  {
    const auto edge1 = EdgeFactory().GetTest(0,from,to);
    const auto edge2 = EdgeFactory().GetTest(1,from,to);
    assert( edge1 !=  edge2);
    assert(*edge1 != *edge2);
  }
  if (verbose) { TRACE("Deep copy"); }
  {
    const auto nodes = Node::GetTests();
    assert(nodes.size() >= 2);
    const Node node_from = nodes[0];
    const Node node_to   = nodes[1];
    const boost::shared_ptr<const Edge> edge{EdgeFactory().GetTest(0,node_from,node_to)};
    const boost::shared_ptr<const Edge> copy{EdgeFactory().DeepCopy(edge,node_from,node_to)};
    assert( edge !=  copy);
    assert(*edge == *copy);
  }
  if (verbose) { TRACE("Edge->XML->Edge must result in the same edge"); }
  {
    const auto edge_before = EdgeFactory().GetTest(0,from,to);
    const std::string s{ToXml(edge_before,nodes)};
    const auto edge_after = EdgeFactory().FromXml(s,nodes);
    assert( edge_before !=  edge_after);
    assert(*edge_before == *edge_after);
  }
  if (verbose) { TRACE("If a Edge's head arrow is changed, a signal must be emitted"); }
  {
    const boost::shared_ptr<Edge> edge{EdgeFactory().GetTest(0,from,to)};
    Counter c{0}; //For receiving the signal
    edge->m_signal_head_arrow_changed.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
    edge->SetHeadArrow(!edge->HasHeadArrow());
    assert(c.Get()==1);
  }
  //From
  if (verbose) { TRACE("If Edge its source/from is equal to the one given in the constructor"); }
  {
    const boost::shared_ptr<Edge> edge{EdgeFactory().GetTest(0,from,to)};
    assert(*edge->GetFrom() == from);
    assert(*edge->GetTo() == to);
  }
}
#endif

std::string ribi::cmap::Edge::ToStr() const noexcept
{

  std::stringstream s;
  s << (*this);
  return s.str();
}

std::string ribi::cmap::Edge::ToXml(
  const ReadOnlyEdgePtr& edge,
  const Nodes& nodes
) noexcept
{
  std::stringstream s;
  s << "<edge>";
  s << edge->GetNode().GetConcept().ToXml();

  assert(edge->GetFrom());
  assert(edge->GetTo());
  const auto from_iter = std::find(begin(nodes),end(nodes),*edge->GetFrom());
  const auto to_iter = std::find(begin(nodes),end(nodes),*edge->GetTo());
  assert(from_iter != nodes.end());
  assert(to_iter != nodes.end());
  const int from_index = std::distance(nodes.begin(),from_iter);
  const int to_index = std::distance(nodes.begin(),to_iter);
  assert(from_index >= 0);
  assert(from_index < boost::numeric_cast<int>(nodes.size()));
  assert(to_index >= 0);
  assert(to_index < boost::numeric_cast<int>(nodes.size()));
  assert(from_index != to_index);

  s << "<from>" << from_index << "</from>";
  s << "<head_arrow>" << edge->HasHeadArrow() << "</head_arrow>";
  s << "<tail_arrow>" << edge->HasTailArrow() << "</tail_arrow>";
  s << "<to>" << to_index << "</to>";
  s << "<x>" << edge->GetNode().GetX() << "</x>";
  s << "<y>" << edge->GetNode().GetY() << "</y>";
  s << "</edge>";

  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,6) == "<edge>");
  assert(r.substr(r.size() - 7,7) == "</edge>");

  return r;
}

bool ribi::cmap::IsConnectedToCenterNode(const boost::shared_ptr<const Edge> edge) noexcept
{
  assert(!(IsCenterNode(*edge->GetFrom()) && IsCenterNode(*edge->GetTo()))
    && "An Edge cannot be connected to two CenterNodes");
  return IsCenterNode(*edge->GetFrom()) || IsCenterNode(*edge->GetTo());

}


bool ribi::cmap::operator==(const ribi::cmap::Edge& lhs, const ribi::cmap::Edge& rhs)
{
  const bool verbose{false};
  if (verbose)
  {
    if ( lhs.GetNode()      !=  rhs.GetNode()) TRACE("Node differs");
    if (*lhs.GetFrom()      != *rhs.GetFrom()) TRACE("From node differs");
    if (*lhs.GetTo()        != *rhs.GetTo()) TRACE("To node differs");
    if ( lhs.HasHeadArrow() != rhs.HasHeadArrow()) TRACE("Has head arrow differs");
    if ( lhs.HasTailArrow() != rhs.HasTailArrow()) TRACE("Has tail arrow differs");
  }
  return
        lhs.GetNode()      ==  rhs.GetNode()
    && *lhs.GetFrom()      == *rhs.GetFrom()
    && *lhs.GetTo()        == *rhs.GetTo()
    &&  lhs.HasHeadArrow() == rhs.HasHeadArrow()
    &&  lhs.HasTailArrow() == rhs.HasTailArrow()
  ;
}

bool ribi::cmap::operator!=(const cmap::Edge& lhs, const cmap::Edge& rhs)
{
  return !(lhs == rhs);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Edge& edge) noexcept
{
  os << edge.GetNode()
    << ", from: " << edge.GetFrom()
    << "(arrowhead: " << edge.HasHeadArrow()
    << "), to: " << edge.GetTo()
    << "(arrowhead: " << edge.HasTailArrow()
    << ")"
  ;

  return os;
}
