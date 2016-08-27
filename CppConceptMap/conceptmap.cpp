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
#include <boost/algorithm/string/replace.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include "conceptmapregex.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmapexample.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "fileio.h"
#include "trace.h"
#include "xml.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "make_custom_and_selectable_vertices_writer.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "custom_and_selectable_vertices_writer.h"
#include "install_vertex_custom_type.h"
#include "my_custom_vertex.h"
#include "graphviz_decode.h"
#include "create_all_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraphs.h"
#include "save_custom_and_selectable_edges_and_vertices_graph_to_dot.h"
#include "load_undirected_custom_and_selectable_edges_and_vertices_graph_from_dot.h"
#include "load_directed_custom_and_selectable_edges_and_vertices_graph_from_dot.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "get_my_custom_edge.h"
#include "get_my_custom_edges.h"
#include "select_random_vertex.h"

namespace ribi {
namespace cmap {

//Very similar to custom_and_selectable_vertices_writer
template <
  typename my_custom_edge_map,
  typename is_selected_map
>
class edge_writer {
public:
  edge_writer(
    my_custom_edge_map mcem,
    is_selected_map ism
  ) : m_my_custom_edge_map{mcem},
      m_is_selected_map{ism}
  {

  }
  template <class edge_descriptor>
  void operator()(
    std::ostream& out,
    const edge_descriptor& ed
  ) const noexcept {
    const ribi::cmap::Edge edge = get(m_my_custom_edge_map, ed);
    out << "[label=\""
      << edge //Can be Graphviz unfriendly
      << "\", "
    ;

    if (get(m_is_selected_map, ed))
    {
      out << "style = \"dashed\", ";
    }

    const bool has_head{edge.HasHeadArrow()};
    const bool has_tail{edge.HasTailArrow()};
    if ( has_head &&  has_tail) {
      out << "dir = \"both\", arrowhead = \"normal\", arrowtail = \"normal\"";
    }
    if ( has_head && !has_tail) {
      out << "dir = \"forward\", arrowhead = \"normal\"";
    }
    if (!has_head && has_tail)
    {
      out << "dir = \"back\", arrowtail = \"normal\"";
    }
    out << "]";
  }
private:
  my_custom_edge_map m_my_custom_edge_map;
  is_selected_map m_is_selected_map;
};

template <
  typename my_custom_vertex_map,
  typename is_selected_map
>
inline edge_writer<
  my_custom_vertex_map,
  is_selected_map
>
make_edge_writer(
  const my_custom_vertex_map& mcvm, //my_custom_vertex_map
  const is_selected_map& ism //any_is_selected_map
)
{
  return edge_writer<
    my_custom_vertex_map,
    is_selected_map
  >(
    mcvm,
    ism
  );
}


} //~namespace ribi
} //~namespace cmap


int ribi::cmap::CountCenterNodes(const ConceptMap& c) noexcept
{
  return CountCenterNodes(GetNodes(c));
}

std::vector<ribi::cmap::ConceptMap>
ribi::cmap::CreateDirectNeighbourConceptMaps(const ConceptMap& c)
{
  return
  create_all_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraphs(c);
}

void ribi::cmap::DecodeConceptMap(ConceptMap& g) noexcept
{
  const auto vip = vertices(g);
  std::for_each(vip.first, vip.second,
    [&g](const VertexDescriptor vd)
    {
      auto vertex_map = get(boost::vertex_custom_type, g);
      Node node = get(vertex_map, vd);
      node.Decode();
      put(vertex_map, vd, node);
    }
  );

  const auto eip = edges(g);
  std::for_each(eip.first, eip.second,
    [&g](const EdgeDescriptor ed)
    {
      auto edge_map = get(boost::edge_custom_type, g);
      Edge edge = get(edge_map, ed);
      edge.Decode();
      put(edge_map, ed, edge);
    }
  );
}

ribi::cmap::ConceptMap ribi::cmap::DotToConceptMap(const std::string& s)
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  {
    std::ofstream f(temp_filename);
    f << s;
  }
  ConceptMap c = LoadFromFile(temp_filename);
  FileIo().DeleteFile(temp_filename);
  return c;
}

ribi::cmap::VertexDescriptor ribi::cmap::FindCenterNode(const ConceptMap& g)
{
  if (CountCenterNodes(g) != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot find the one center node, as there are "
      << CountCenterNodes(g)
    ;
    throw std::logic_error(msg.str());
  }
  using vd = VertexDescriptor;
  const auto vip = vertices(g);
  const auto i = std::find_if(
    vip.first, vip.second,
    [g](const vd d) {
      const auto my_vertex_map = get(boost::vertex_custom_type, g);
      return IsCenterNode(get(my_vertex_map, d));
    }
  );
  assert(i != vip.second);
  return *i;
}

ribi::cmap::Node ribi::cmap::GetCenterNode(const ConceptMap& c)
{
  if (CountCenterNodes(c) != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot get the one center node, as there are "
      << CountCenterNodes(c)
    ;
    throw std::logic_error(msg.str());
  }
  return GetNode(FindCenterNode(c), c);
}

ribi::cmap::Edge ribi::cmap::GetEdge(
  const ribi::cmap::EdgeDescriptor ed, const ribi::cmap::ConceptMap& g) noexcept
{
  return get_my_custom_edge(ed, g);
}

std::vector<ribi::cmap::Edge> ribi::cmap::GetEdges(const ConceptMap& c) noexcept
{
  //return get_my_custom_edges(c);
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

ribi::cmap::Edge ribi::cmap::GetFirstEdge(const ConceptMap& c)
{
  assert(boost::num_edges(c) > 0);
  return GetEdge(*edges(c).first, c);
}

ribi::cmap::Node ribi::cmap::GetFirstNode(const ConceptMap& c)
{
  if (boost::num_vertices(c) == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Cannot get the first node, if there are zero nodes"
    ;
    throw std::logic_error(msg.str());
  }
  return GetNode(*vertices(c).first, c);
}

ribi::cmap::Node ribi::cmap::GetFrom(const Edge& edge, const ConceptMap& c) noexcept
{
  return GetFrom(::find_first_custom_edge_with_my_edge(edge, c), c);
}

ribi::cmap::Node ribi::cmap::GetFrom(const EdgeDescriptor ed, const ConceptMap& c) noexcept
{
  return GetNode(boost::source(ed, c), c);
}

ribi::cmap::Node ribi::cmap::GetNode(
  const ribi::cmap::VertexDescriptor vd, const ribi::cmap::ConceptMap& g
) noexcept
{
  return get_my_custom_vertex(vd, g);
}

std::vector<ribi::cmap::Node> ribi::cmap::GetNodes(const ConceptMap& c) noexcept
{
  //return get_my_custom_vertexes(g);

  const auto vip = vertices(c);
  std::vector<Node> v(boost::num_vertices(c));
  std::transform(vip.first,vip.second,std::begin(v),
    [c](const VertexDescriptor& d)
    {
      return get_my_custom_vertex(d, c);
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

ribi::cmap::Node ribi::cmap::GetTo(const Edge& edge, const ConceptMap& c) noexcept
{
  return GetTo(::find_first_custom_edge_with_my_edge(edge, c), c);
}

ribi::cmap::Node ribi::cmap::GetTo(const EdgeDescriptor ed, const ConceptMap& c) noexcept
{
  return GetNode(boost::target(ed, c), c);
}

bool ribi::cmap::HasCenterNode(const ConceptMap& c) noexcept
{
  const auto nodes = GetNodes(c);
  const auto i = std::find_if(
    std::begin(nodes),std::end(nodes),
    [](const Node& node) {
      return IsCenterNode(node);
    }
  );
  return i != std::end(nodes);
}

ribi::cmap::ConceptMap ribi::cmap::LoadFromFile(const std::string& dot_filename)
{
  auto g = load_undirected_custom_and_selectable_edges_and_vertices_graph_from_dot<
      decltype(ConceptMap())
    >(dot_filename)
  ;
  DecodeConceptMap(g);
  return g;
}

ribi::cmap::ConceptMap ribi::cmap::RemoveFirstNode(ConceptMap g)
{
  if (!boost::num_vertices(g))
  {
    throw std::invalid_argument("RemoveFirstNode: need at least one node");
  }
  boost::clear_vertex(*vertices(g).first, g);
  boost::remove_vertex(*vertices(g).first, g);
  return g;
}

void ribi::cmap::SaveToFile(const ConceptMap& g, const std::string& dot_filename)
{
  //Cannot use:
  //save_custom_and_selectable_edges_and_vertices_graph_to_dot(g, dot_filename);
  //because we have a directed graph in which edegs have zero/one/two arrow heads

  std::ofstream f(dot_filename);
  boost::write_graphviz(f, g,
    make_custom_and_selectable_vertices_writer(
      get(boost::vertex_custom_type,g),
      get(boost::vertex_is_selected,g)
    ),
    make_edge_writer(
      get(boost::edge_custom_type,g),
      get(boost::edge_is_selected,g)
    )
  );

}

void ribi::cmap::SaveToImage(const ConceptMap& g, const std::string& png_filename)
{
  const std::string dot_filename{"SaveToImage.dot"};
  const std::string svg_filename{"SaveToImage.svg"};
  SaveToFile(g, dot_filename);
  convert_dot_to_svg(dot_filename, svg_filename);
  convert_svg_to_png(svg_filename, png_filename);
  ribi::FileIo().DeleteFile(dot_filename);
  ribi::FileIo().DeleteFile(svg_filename);
}

void ribi::cmap::SaveSummaryToImage(const ConceptMap& g, const std::string& png_filename)
{
  const std::string dot_filename{"SaveToImage.dot"};
  const std::string svg_filename{"SaveToImage.svg"};
  SaveSummaryToFile(g, dot_filename);
  convert_dot_to_svg(dot_filename, svg_filename);
  convert_svg_to_png(svg_filename, png_filename);
  ribi::FileIo().DeleteFile(dot_filename);
  ribi::FileIo().DeleteFile(svg_filename);
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
    [g](std::ostream& out, const EdgeDescriptor& ed) {
      const auto edge_map = get(boost::edge_custom_type, g);
      const auto edge = get(edge_map, ed);
      out << "[label=\""
        << edge.GetNode().GetConcept().GetName()
        << "\", "
       ;

      const bool has_head{edge.HasHeadArrow()};
      const bool has_tail{edge.HasTailArrow()};
      if ( has_head &&  has_tail)
      {
        out << "dir = \"both\", arrowhead = \"normal\", arrowtail = \"normal\"";
      }
      if ( has_head && !has_tail)
      {
        out << "dir = \"forward\", arrowhead = \"normal\"";
      }
      if (!has_head &&  has_tail)
      {
        out << "dir = \"back\", arrowtail = \"normal\"";
      }
      out << "]";
    }
  );
}

void ribi::cmap::SelectRandomNode(
  ConceptMap& g,
  std::mt19937& rng_engine
) noexcept
{
  select_random_vertex(g, rng_engine);
}

std::string ribi::cmap::ToDot(const ConceptMap& g) noexcept
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  SaveToFile(g, temp_filename);
  std::stringstream s;
  s << FileIo().FileToStr(temp_filename);
  return s.str();
}

std::string ribi::cmap::ToXml(const ConceptMap& conceptmap) noexcept
{
  const std::string temp_filename{FileIo().GetTempFileName(".dot")};
  SaveToFile(conceptmap, temp_filename);
  std::stringstream s;
  s << "<conceptmap>"
    << FileIo().FileToStr(temp_filename) // << ToDot(conceptmap);
    << "</conceptmap>"
  ;
  FileIo().DeleteFile(temp_filename);
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
    msg << __func__ << ": string too short, "
      << "received '" << s << "'"
    ;
    throw std::logic_error(msg.str());
  }
  const std::string required_tag{"<conceptmap>"};
  const int required_tag_size{static_cast<int>(required_tag.size())};
  if (s.substr(0, required_tag_size) != required_tag)
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect starting tag, "
      << "required '" << required_tag << "', "
      << "received '" << s << "'"
    ;
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 13,13) != "</conceptmap>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect ending tag";
    throw std::logic_error(msg.str());
  }
  //Need to write the DOT to file
  const std::string dot_filename = ribi::fileio::FileIo().GetTempFileName(".dot");
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConceptMap());
    assert(v.size() == 1);
    const std::string dot_str{
      ribi::xml::StripXmlTag(v[0])
    };

    std::ofstream f(dot_filename);
    f << dot_str;
  }
  ConceptMap conceptmap = LoadFromFile(dot_filename);
  ribi::fileio::FileIo().DeleteFile(dot_filename);
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
