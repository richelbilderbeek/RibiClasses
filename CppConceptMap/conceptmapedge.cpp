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
#include "container.h"
#include "conceptmap.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "conceptmapconceptfactory.h"
#include "conceptmaphelper.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "is_graphviz_friendly.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

int ribi::cmap::Edge::sm_ids = 0; //ID to assign

ribi::cmap::Edge::Edge(
  const Node& node
) :
    m_id{sm_ids++},
    m_node{node}
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::Edge::~Edge() noexcept
{

}

std::string ribi::cmap::Edge::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::cmap::Edge::GetVersionHistory() noexcept
{
  return {
    "2013-xx-xx: Version 1.0: initial version",
    "2014-06-01: Version 1.1: replaced Concept, X and Y by a Node",
    "2015-11-07: Version 1.2: use Node instead of a Node smart pointer",
    "2015-12-28: Version 2.0: Edge is part of a Boost.Graph",
  };
}

void ribi::cmap::Edge::SetNode(const Node& node) noexcept
{
  m_node = node;
}

/*
void ribi::cmap::Edge::SetHeadArrow(const bool has_head_arrow) noexcept
{
  m_head_arrow = has_head_arrow;
}

void ribi::cmap::Edge::SetTailArrow(const bool has_tail_arrow) noexcept
{
  m_tail_arrow = has_tail_arrow;
}
*/

#ifndef NDEBUG
void ribi::cmap::Edge::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{true};
  const std::vector<Node> nodes{NodeFactory().GetTest(0),NodeFactory().GetTest(1)};
  if (verbose) { TRACE("Copy constructor"); }
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    const auto edge2(edge1);
    assert(edge1 == edge2);
  }
  if (verbose) { TRACE("Assignment operator"); }
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    auto edge2 = EdgeFactory().GetTest(1);
    assert(edge1 != edge2);
    edge2 = edge1;
    assert(edge1 == edge2);
  }
  if (verbose) { TRACE("Operator=="); }
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    const auto edge2 = EdgeFactory().GetTest(0);
    assert(edge1 == edge2);
  }
  if (verbose) { TRACE("Operator=="); }
  {
    const auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    assert(edge1 == edge2);
  }
  if (verbose) { TRACE("Operator!="); }
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    const auto edge2 = EdgeFactory().GetTest(1);
    assert(edge1 != edge2);
  }
  if (verbose) { TRACE("Edge->XML->Edge must result in the same edge"); }
  {
    const Edge edge_before{EdgeFactory().GetTest(0)};
    const std::string s{ToXml(edge_before)};
    const Edge edge_after{XmlToEdge(s)};
    assert(ToXml(edge_before) == ToXml(edge_after));
    assert(edge_before == edge_after);
  }
  if (verbose) { TRACE("Stream operator"); }
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    std::stringstream s;
    s << edge1;
    Edge edge2;
    s >> edge2;
    assert(edge1 == edge2);
  }
  if (verbose) { TRACE("Stream operators"); }
  {
    const Edge e = EdgeFactory().GetTest(1);
    const Edge f = EdgeFactory().GetTest(2);
    std::stringstream s;
    s << e << f;
    Edge g;
    Edge h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    assert(e == g);
    assert(f == h);
  }
  if (verbose) { TRACE("Stream operator for nasty"); }
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Edge f;
    assert(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    assert(e == f);
  }
  if (verbose) { TRACE("Stream operators for nasty"); }
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    Edge g;
    Edge h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    assert(e == g);
    assert(e == h);
  }
}
#endif

std::string ribi::cmap::Edge::ToStr() const noexcept
{

  std::stringstream s;
  s << (*this);
  return s.str();
}

std::string ribi::cmap::ToXml(
  const Edge& edge
) noexcept
{
  std::stringstream s;
  s << "<edge>";
  s << ToXml(edge.GetNode().GetConcept());
  s << "<x>" << edge.GetNode().GetX() << "</x>";
  s << "<y>" << edge.GetNode().GetY() << "</y>";
  s << "</edge>";

  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,6) == "<edge>");
  assert(r.substr(r.size() - 7,7) == "</edge>");

  return r;
}

ribi::cmap::Edge ribi::cmap::XmlToEdge(
  const std::string& s
)
{
  using ribi::xml::StripXmlTag;
  assert(s.size() >= 13);
  assert(s.substr(0,6) == "<edge>");
  assert(s.substr(s.size() - 7,7) == "</edge>");
  //m_concept
  Concept concept = ConceptFactory().Create();
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConcept());
    assert(v.size() == 1);
    concept = XmlToConcept(v[0]);
  }
  //m_x
  double x = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexX());
    assert(v.size() == 1);
    x = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  //m_y
  double y = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexY());
    assert(v.size() == 1);
    y = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  Node node(concept,x,y);
  Edge edge(
    node
  );
  return edge;
}

bool ribi::cmap::operator==(const ribi::cmap::Edge& lhs, const ribi::cmap::Edge& rhs)
{
  const bool verbose{false};
  if (verbose)
  {
    if (lhs.GetNode() != rhs.GetNode()) TRACE("Node differs");
  }
  return
    lhs.GetNode() == rhs.GetNode()
  ;
}

bool ribi::cmap::operator!=(const cmap::Edge& lhs, const cmap::Edge& rhs)
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const cmap::Edge& lhs, const cmap::Edge& rhs)
{
  return lhs.GetNode() < rhs.GetNode();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Edge& edge) noexcept
{
  const std::string s{graphviz_encode(ToXml(edge))};
  assert(is_graphviz_friendly(s));
  os << s;
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Edge& edge)
{
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 7 && s.substr(s.size() - 7,7) == "</edge>") break;
  }
  edge = XmlToEdge(graphviz_decode(s));
  return is;
}
