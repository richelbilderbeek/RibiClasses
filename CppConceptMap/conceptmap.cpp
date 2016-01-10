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
#include "conceptmap.h"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graphviz.hpp>
#include "conceptmapregex.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmapexample.h"
#include "fileio.h"
#include "trace.h"
#include "xml.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "make_custom_and_selectable_vertices_writer.h"
#include "custom_and_selectable_vertices_writer.h"

ribi::cmap::ConceptMap ribi::cmap::DotToConceptMap(const std::string& s)
{
  std::stringstream f;
  f << s;
  ConceptMap g;
  boost::dynamic_properties dp(boost::ignore_other_properties);
  dp.property("label", get(boost::vertex_custom_type, g));
  dp.property("regular", get(boost::vertex_is_selected, g));
  dp.property("label", get(boost::edge_custom_type, g));
  dp.property("regular", get(boost::edge_is_selected, g));
  boost::read_graphviz(f,g,dp);
  return g;
}

std::vector<ribi::cmap::Edge> ribi::cmap::GetEdges(const ConceptMap& c) noexcept
{
  const auto eip = edges(c);
  std::vector<Edge> v(boost::num_edges(c));
  std::transform(eip.first,eip.second,std::begin(v),
    [c](const EdgeDescriptor& d)
    {
      const auto edge_map = get(boost::edge_custom_type, c);
      return get(edge_map, d);
    }
  );
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::GetNodes(const ConceptMap& c) noexcept
{
  const auto vip = vertices(c);
  std::vector<Node> v(boost::num_vertices(c));
  std::transform(vip.first,vip.second,std::begin(v),
    [c](const VertexDescriptor& d)
    {
      const auto vertex_map = get(boost::vertex_custom_type, c);
      return get(vertex_map, d);
    }
  );
  return v;
}

std::vector<ribi::cmap::Edge> ribi::cmap::GetSortedEdges(const ConceptMap& c) noexcept
{
  auto v = GetEdges(c);
  std::sort(std::begin(v),std::end(v));
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::GetSortedNodes(const ConceptMap& c) noexcept
{
  auto v = GetNodes(c);
  std::sort(std::begin(v),std::end(v));
  return v;
}

ribi::cmap::ConceptMap ribi::cmap::LoadFromFile(const std::string& dot_filename)
{
  assert(ribi::FileIo().IsRegularFile(dot_filename));
  std::ifstream f(dot_filename);
  ConceptMap g;
  boost::dynamic_properties dp(boost::ignore_other_properties);
  dp.property("label", get(boost::vertex_custom_type, g));
  dp.property("regular", get(boost::vertex_is_selected, g));
  dp.property("label", get(boost::edge_custom_type, g));
  dp.property("regular", get(boost::edge_is_selected, g));
  boost::read_graphviz(f,g,dp);
  return g;
}

void ribi::cmap::SaveToFile(const ConceptMap& g, const std::string& dot_filename)
{
  std::ofstream f(dot_filename);
  f << ToDot(g);
}

void ribi::cmap::SaveSummaryToFile(const ConceptMap& g, const std::string& dot_filename)
{
  std::ofstream f(dot_filename);

  boost::write_graphviz(f, g,
    [g](std::ostream& out, const VertexDescriptor& vd) {
      const auto pmap = get(boost::vertex_custom_type, g);
      out << "[label=\""
        << get(pmap, vd).GetConcept().GetName()
        << "\"]"
      ;
    },
    [g](std::ostream& out, const EdgeDescriptor& vd) {
      const auto pmap = get(boost::edge_custom_type, g);
      out << "[label=\""
        << get(pmap, vd).GetNode().GetConcept().GetName()
        << "\"]"
      ;
    }
  );
}

ribi::cmap::ConceptMap ribi::cmap::SelectRandomNode(ConceptMap g) noexcept
{
  assert(boost::num_vertices(g) > 0);
  const int n{static_cast<int>(boost::num_vertices(g))};
  const int i{std::rand() % n};
  auto vdi = vertices(g).first;
  for (int j=0; j!=i; ++j, ++vdi) {}
  const auto pmap = get(boost::vertex_is_selected,g);
  put(pmap,*vdi,true);
  return g;
}

std::string ribi::cmap::ToDot(const ConceptMap& g) noexcept
{
  std::stringstream f;
  boost::write_graphviz(f, g,
    make_custom_and_selectable_vertices_writer(
      get(boost::vertex_custom_type,g),
      get(boost::vertex_is_selected,g)
    ),
    make_custom_and_selectable_vertices_writer(
      get(boost::edge_custom_type,g),
      get(boost::edge_is_selected,g)
    )
  );
  return f.str();
}

std::string ribi::cmap::ToXml(const ConceptMap& conceptmap) noexcept
{
 std::stringstream s;
  s << "<conceptmap>";
  s << ToDot(conceptmap);
  s << "</conceptmap>";
  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,12) == "<conceptmap>");
  assert(r.substr(r.size() - 13,13) == "</conceptmap>");
  return r;
}

ribi::cmap::ConceptMap ribi::cmap::UnselectEdges(ConceptMap g) noexcept
{
  const auto eip = edges(g);
  std::for_each(eip.first, eip.second,
    [&g](const EdgeDescriptor ed)
    {
      const auto pmap = get(boost::edge_is_selected, g);
      put(pmap,ed,false);
    }
  );
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::UnselectEverything(ConceptMap conceptmap) noexcept
{
  return UnselectEdges(UnselectNodes(conceptmap));
}

ribi::cmap::ConceptMap ribi::cmap::UnselectNodes(ConceptMap g) noexcept
{
  const auto vip = vertices(g);
  std::for_each(vip.first, vip.second,
    [&g](const VertexDescriptor vd)
    {
      const auto pmap = get(boost::vertex_is_selected, g);
      put(pmap,vd,false);
    }
  );
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::XmlToConceptMap(const std::string& s)
{
 if (s.size() < 13)
  {
    std::stringstream msg;
    msg << __func__ << ": string too short";
    throw std::logic_error(msg.str());
  }
  if (s.substr(0,12) != "<conceptmap>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect starting tag";
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 13,13) != "</conceptmap>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect ending tag";
    throw std::logic_error(msg.str());
  }
  ConceptMap conceptmap;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConceptMap());
    assert(v.size() == 1);
    const std::string dot_str{
      ribi::xml::StripXmlTag(v[0])
    };
    conceptmap = DotToConceptMap(dot_str);
  }
  return conceptmap;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept
{
  os << graphviz_encode(ToXml(conceptmap));
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, ConceptMap& conceptmap)
{
  //eat until '</conceptmap>'
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 13 && s.substr(s.size() - 13,13) == "</conceptmap>") break;
    assert(s != "00000");
  }
  conceptmap = XmlToConceptMap(graphviz_decode(s));
  return is;
}

bool ribi::cmap::operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  //Still imperfect
  return
    boost::isomorphism(lhs,rhs)
    && GetSortedNodes(lhs) == GetSortedNodes(rhs)
    && GetSortedEdges(lhs) == GetSortedEdges(rhs)
  ;
}

bool ribi::cmap::operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  return !(lhs == rhs);
}
