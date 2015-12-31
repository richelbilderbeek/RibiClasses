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
#include "conceptmap.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graphviz.hpp>
#include "conceptmapregex.h"
#include "fileio.h"
#include "trace.h"
#include "xml.h"

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
  TRACE(r);
  return r;
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
    TRACE(dot_str);
    conceptmap = DotToConceptMap(dot_str);
  }
  return conceptmap;
}

std::string ribi::cmap::ToDot(const ConceptMap& g) noexcept
{
  std::stringstream f;
  boost::write_graphviz(
    f,
    g,
    [g](
      std::ostream& out, const VertexDescriptor& d) {
      const auto pmap = get(boost::vertex_custom_type, g);
      const auto& m = get(pmap, d);
      out << "[label=" << m << "]";
    },
    [g](std::ostream& out, const EdgeDescriptor& d) {
      const auto pmap = get(boost::edge_custom_type, g);
      const auto& m = get(pmap, d);
      out << "[label=" << m << "]";
    }
  );
  return f.str();
}

ribi::cmap::ConceptMap ribi::cmap::DotToConceptMap(const std::string& s)
{
  ConceptMap g;

  try
  {
    std::stringstream f;
    f << s;
    TRACE(s);
    boost::dynamic_properties p;
    p.property("node_id", get(boost::vertex_custom_type, g));
    p.property("label", get(boost::vertex_custom_type, g));
    p.property("edge_id", get(boost::edge_custom_type, g));
    p.property("label", get(boost::edge_custom_type, g));
    boost::read_graphviz(f,g,p);
  }
  catch (std::exception& e)
  {
    TRACE(e.what());
    TRACE(s);
    assert(!"Should not get here");
  }
  return g;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const ConceptMap& conceptmap) noexcept
{
  os << ToXml(conceptmap);
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, ConceptMap& conceptmap)
{
  //eat until '</conceptmap>'
  is >> std::noskipws;
  std::string s;
  for (int i=0; ; ++i)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 13 && s.substr(s.size() - 13,13) == "</conceptmap>") break;
    assert(i != 1000);
  }
  conceptmap = XmlToConceptMap(s);
  return is;
}

bool ribi::cmap::operator==(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  //Does not detect different labels and/or edges
  return boost::isomorphism(lhs,rhs);
}

bool ribi::cmap::operator!=(const ConceptMap& lhs, const ConceptMap& rhs) noexcept
{
  return !(lhs == rhs);
}

void ribi::cmap::SaveToFile(const ConceptMap& g, const std::string& dot_filename)
{
  std::ofstream f(dot_filename);
  f << ToDot(g);
}

ribi::cmap::ConceptMap ribi::cmap::LoadFromFile(const std::string& dot_filename)
{
  assert(ribi::FileIo().IsRegularFile(dot_filename));
  return {};
}

#ifdef DO_NOT_USE_BOOST_GRAPH

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
//#include <QPointF>
#include "conceptmapvertexcustomtype.h"
#include "conceptmapvertexisselected.h"
#include "conceptmapedgecustomtype.h"
#include "conceptmapedgeisselected.h"
#include "conceptmapfwd.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmap.h"
//#include <boost/graph/properties.hpp>

#include <iostream>
#include <set>
#include <sstream>
#include <functional>


//#include <boost/bind.hpp>
//#include <boost/lambda/lambda.hpp>
//#include <boost/lexical_cast.hpp>

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

template <
  typename my_custom_edge_or_vertex_map,
  typename is_selected_map
>
class custom_and_selectable_edges_or_vertices_writer {
public:
  custom_and_selectable_edges_or_vertices_writer(
    my_custom_edge_or_vertex_map any_my_custom_edge_or_vertex_map,
    is_selected_map any_is_selected_map
  ) : m_my_custom_edge_or_vertex_map{any_my_custom_edge_or_vertex_map},
      m_is_selected_map{any_is_selected_map}
  {

  }
  template <class vertex_descriptor>
  void operator()(
    std::ostream& out,
    const vertex_descriptor& vd
  ) const noexcept {
    out << "[label=\"" << m_my_custom_edge_or_vertex_map[vd]
      << "\", regular=\"" << m_is_selected_map[vd]
      << "\"]"
    ;
  }
private:
  my_custom_edge_or_vertex_map m_my_custom_edge_or_vertex_map;
  is_selected_map m_is_selected_map;
};

template <
  typename my_custom_edge_or_vertex_map,
  typename is_selected_map
>
inline custom_and_selectable_edges_or_vertices_writer<
  my_custom_edge_or_vertex_map,
  is_selected_map
>
make_custom_and_selectable_edges_or_vertices_writer(
  const my_custom_edge_or_vertex_map& any_my_custom_edge_or_vertex_map,
  const is_selected_map& any_is_selected_map
)
{
  return custom_and_selectable_edges_or_vertices_writer<
    my_custom_edge_or_vertex_map,
    is_selected_map
  >(
    any_my_custom_edge_or_vertex_map,
    any_is_selected_map
  );
}


/*
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
*/

ribi::cmap::ConceptMap::ConceptMap(
  const Graph& graph
)  noexcept
  : m_graph{graph},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  #endif
}

/*
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
*/

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

void ribi::cmap::ConceptMap::AddEdge(
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  const Edge& edge
) noexcept
{
  if (HasEdge(edge))
  {
    if (m_verbose) { TRACE("Warning: edge already added"); }
    return;
  }

  //assert(HasNodeWithIndex(edge.GetFromIndex()));
  //assert(HasNodeWithIndex(edge.GetToIndex()));
  //const auto vd_from = GetNodeWithIndex(edge.GetFromIndex());
  //const auto vd_to = GetNodeWithIndex(edge.GetToIndex());
  const auto aer = boost::add_edge(vd_from, vd_to, edge, m_graph);
  assert(aer.second);

  ClearAllSelectednesses();

  const auto is_selected_map = get(
    boost::vertex_is_selected, m_graph
  );
  put( //not boost::get
    is_selected_map,
    aer.second,
    true
  );
  assert(AreAllEdgeIdsUnique());
}

ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::AddNode(const Node& node) noexcept
{
  if (HasNode(node))
  {
    if (m_verbose) { TRACE("Warning: node already added"); }
    return FindNode(node);
  }

  const auto vd = boost::add_vertex(node, m_graph);
  auto is_selected_map = get(
    boost::vertex_is_selected, m_graph
  );
  is_selected_map[vd] = true;

  assert(AreAllNodeIdsUnique());

  return vd;
}


/*
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
      if (!edge_a.GetToIndex())
      {
        if (verbose) { TRACE("edge_a.GetTo() is nullptr"); }
        return false;
      }
      const Node *a_from{edge_a.GetFrom()};
      const Node *a_to  {edge_a.GetToIndex()  };
      assert(a_from == edge_a.GetFrom());
      assert(a_to   == edge_a.GetToIndex());

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
        if (!edge_b.GetToIndex())
        {
          if (verbose) { TRACE("edge_b.GetTo() is nullptr"); }
          return false;
        }
        const Node *b_from{edge_b.GetFrom()};
        const Node *b_to  {edge_b.GetToIndex()};
        assert(b_from == edge_b.GetFrom());
        assert(b_to   == edge_b.GetToIndex());
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
*/

void ribi::cmap::ConceptMap::ClearAllSelectednesses() noexcept
{
  {
    const auto is_selected_map = get(boost::vertex_is_selected, m_graph);
    const auto vip = vertices(m_graph);
    const auto j = vip.second;
    for (auto i = vip.first; i!=j; ++i) {
      put(is_selected_map,*i,false);
    }
  }
  {
    const auto is_selected_map = get(boost::edge_is_selected, m_graph);
    const auto vip = edges(m_graph);
    const auto j = vip.second;
    for (auto i = vip.first; i!=j; ++i) {
      put(is_selected_map,*i,false);
    }
  }
}

int ribi::cmap::ConceptMap::CountSelectedNodes() const noexcept
{
  const auto vip = vertices(m_graph);
  const auto cnt = std::count_if(vip.first, vip.second,
    [this](const VertexDescriptor& vd)
    {
      const auto is_selected_map = get(boost::vertex_is_selected, m_graph);
      return get(is_selected_map,vd);
    }
  );
  return static_cast<int>(cnt);
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

ribi::cmap::ConceptMap ribi::cmap::ConceptMap::CreateSub(const VertexDescriptor& vd) const noexcept
{
  const auto g = m_graph;
  Graph h;
  std::map<VertexDescriptor,VertexDescriptor> m; //old - new
  {
    {
      const auto vd_h = boost::add_vertex(h);
      m.insert(std::make_pair(vd,vd_h));
    }
    //Copy vertices
    {
      const auto vdsi = boost::adjacent_vertices(vd, g);
      const auto j = vdsi.second;
      for (auto i = vdsi.first; i!=j; ++i)
      {
        const auto vd_h = boost::add_vertex(h);
        m.insert(std::make_pair(*i,vd_h));
      }
    }
    //Copy vertex customness and selectedness
    for (const auto p: m)
    {
      const auto old_custom_vertexes_map = get(boost::vertex_custom_type,g);
      auto new_custom_vertexes_map = get(boost::vertex_custom_type,h);
      const auto vd_old = p.first;
      const auto vd_new = p.second;

      new_custom_vertexes_map[vd_new] = old_custom_vertexes_map[vd_old];

      const auto old_vertex_is_selected_map = get(boost::vertex_is_selected,g);
      auto new_vertex_is_selected_map = get(boost::vertex_is_selected,h);
      new_vertex_is_selected_map[vd_new] = old_vertex_is_selected_map[vd_old];
    }
  }
  {
    std::map<EdgeDescriptor,EdgeDescriptor> n; //old - new
    //Collect and add edges
    {
      const auto eip = edges(g);
      const auto j = eip.second;
      for (auto i = eip.first; i!=j; ++i)
      {
        const auto vd_from = source(*i, g);
        const auto vd_to = target(*i, g);
        if (m.find(vd_from) == std::end(m)) continue;
        if (m.find(vd_to) == std::end(m)) continue;
        const auto ed = edge(vd_from, vd_to, g).first;
        const auto aer = boost::add_edge(m[vd_from],m[vd_to], h);
        assert(aer.second);
        n.insert(std::make_pair(ed, aer.first));
      }
      //Copy edge customness and selectedness
      for (const auto p: n)
      {
        const auto old_custom_edges_map = get(boost::edge_custom_type,g);
        auto new_custom_edges_map = get(boost::edge_custom_type,h);
        const auto ed_old = p.first;
        const auto ed_new = p.second;

        new_custom_edges_map[ed_new] = old_custom_edges_map[ed_old];

        const auto old_edge_is_selected_map = get(boost::edge_is_selected,g);
        auto new_edge_is_selected_map = get(boost::edge_is_selected,h);
        new_edge_is_selected_map[ed_new] = old_edge_is_selected_map[ed_old];
      }
    }
  }
  ConceptMap concept_map(h);
  return concept_map;
}

ribi::cmap::ConceptMap::SubConceptMaps
  ribi::cmap::ConceptMap::CreateSubs() const noexcept
{
  std::vector<ConceptMap> v;
  const auto vip = vertices(m_graph);
  const auto j = vip.second;
  for (auto i = vip.first; i!=j; ++i) {
    v.emplace_back(
      CreateSub(
        *i
      )
    );
  }
  return v;
  /*
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
        assert(focal_edge.GetToIndex() != &focal_node);
        nodes.push_back(*focal_edge.GetToIndex());
      }
      else if (focal_edge.GetToIndex() == &focal_node)
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
  */
}

void ribi::cmap::ConceptMap::DeleteEdge(const Edge& edge) noexcept
{
  if(!HasEdge(edge)) return;
  boost::remove_edge(FindEdge(edge),m_graph);
  assert(!HasEdge(edge));
}

void ribi::cmap::ConceptMap::DeleteNode(const Node& node) noexcept
{
  if(!HasNode(node)) return;

  const auto vd = FindNode(node);
  boost::clear_vertex(vd, m_graph);
  boost::remove_vertex(vd, m_graph);
  /*

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
  */

  //If there is no node selected anymore, give focus to a possible first node
  if (HasSelectedNodes() && boost::num_vertices(m_graph) > 0)
  {
    auto is_selected_map = get(boost::vertex_is_selected, m_graph);
    const auto first_vd = *(vertices(m_graph).first);
    is_selected_map[first_vd] = true;
  }
}

bool ribi::cmap::ConceptMap::Empty() const noexcept
{
  return boost::num_vertices(m_graph) == 0;
}

ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::FindCenterNode() const noexcept
{
  assert(HasCenterNode());

  const auto node_map = get(boost::vertex_custom_type, m_graph);
  const auto vip = vertices(m_graph);
  const auto j = vip.second;
  for (auto i = vip.first; i!=j; ++i)
  {
    const auto vd = *i;
    if (get(node_map, vd).IsCenterNode()) { return vd; }
  }
  assert(!"Should not get here");
  throw; //Will always crash the program
}

ribi::cmap::EdgeDescriptor ribi::cmap::ConceptMap::FindEdge(const Edge& edge) const noexcept
{
  const auto eip = edges(m_graph);
  const auto i = std::find_if(eip.first,eip.second,
    [this, edge](const EdgeDescriptor& vd) {
      const auto edge_map = get(boost::edge_custom_type, m_graph);
      return get(edge_map,vd) == edge;
    }
  );
  assert(i != eip.second);
  return *i;
}

ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::FindNode(const Node& node) const noexcept
{
  const auto vip = vertices(m_graph);
  const auto i = std::find_if(vip.first,vip.second,
    [this, node](const VertexDescriptor& vd) {
      const auto node_map = get(boost::vertex_custom_type, m_graph);
      return get(node_map,vd) == node;
    }
  );
  assert(i != vip.second);
  return *i;
}



ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::FindCenterNode() noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  return const_cast<const ConceptMap&>(*this).FindCenterNode();
}

ribi::cmap::EdgeDescriptor ribi::cmap::ConceptMap::GetEdgeHaving(const Node& node) const noexcept
{
  assert(HasEdgeHaving(node));
  const auto edge_map = get(boost::edge_custom_type, m_graph);
  const auto eip = edges(m_graph);
  const auto j = eip.second;
  for (auto i = eip.first; i!=j; ++i)
  {
    const auto ed = *i;
    if (edge_map[ed].GetNode().GetId() == node.GetId()) { return ed; }
  }
  assert(!"Should not get here");
  throw; //Will always crash the program
}

/*
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
*/

ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::GetFocalNode() const noexcept
{
  assert(boost::num_vertices(m_graph) > 0);
  return *(vertices(m_graph).first);
}

ribi::cmap::VertexDescriptor ribi::cmap::ConceptMap::GetFocalNode() noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  return const_cast<const ConceptMap&>(*this).GetFocalNode();
}

std::string ribi::cmap::ConceptMap::GetVersion() noexcept
{
  return "3.0";
}

std::vector<std::string> ribi::cmap::ConceptMap::GetVersionHistory() noexcept
{
  return {
    "2013-12-xx: Version 1.0: initial version",
    "2013-12-23: Version 1.1: started versioning",
    "2014-02-08: Version 1.2: support an empty concept map",
    "2014-03-24: Version 1.3: distinguished correctly between focus and selected",
    "2015-08-13: Version 2.0: merge of ConceptMap and ConceptMapWidget",
    "2015-08-28: Version 2.1: removed many useless member variables",
    "2015-11-01: Version 2.2: made Node a regular type",
    "2015-12-28: Version 3.0: use of Boost.Graph"
  };
}

bool ribi::cmap::ConceptMap::HasEdge(const Edge& edge) const noexcept
{
  const auto edge_map = get(boost::edge_custom_type, m_graph);
  const auto eip = edges(m_graph);
  const auto j = eip.second;
  for (auto i = eip.first; i!=j; ++i)
  {
    const auto ed = *i;
    if (edge_map[ed] == edge) { return true; }
  }
  return false;
}

bool ribi::cmap::ConceptMap::HasNode(const Node& node) const noexcept
{
  const auto node_map = get(boost::vertex_custom_type, m_graph);
  const auto vip = vertices(m_graph);
  const auto j = vip.second;
  for (auto i = vip.first; i!=j; ++i)
  {
    const auto vd = *i;
    if (node_map[vd] == node) { return true; }
  }
  return false;
}

/*
bool ribi::cmap::HasSameContent(
  const ConceptMap& lhs,
  const ConceptMap& rhs
) noexcept
{
  const bool trace_verbose{false};

  if (lhs.GetNumberOfEdges() != rhs.GetNumberOfEdges())
  {
    if (trace_verbose) { TRACE("Number of edges differ"); }
    return false;
  }
  if (lhs.GetNumberOfNodes() != rhs.GetNumberOfNodes())
  {
    if (trace_verbose)
    {
      TRACE("Number of nodes differ");
    }
    return false;
  }
  //Same Nodes
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
  //Check if the out degrees match
  {
    std::multiset<int> lhs_out_degrees;
    {
      const auto g = lhs.GetGraph();
      const auto vis = vertices(g);
      const auto j = vis.second;
      for (auto i = vis.first; i!=j; ++i) {
        lhs_out_degrees.insert(
          out_degree(*i,g)
        );
      }
    }
    std::multiset<int> rhs_out_degrees;
    {
      const auto g = rhs.GetGraph();
      const auto vis = vertices(g);
      const auto j = vis.second;
      for (auto i = vis.first; i!=j; ++i) {
        rhs_out_degrees.insert(
          out_degree(*i,g)
        );
      }
    }
    if (lhs_out_degrees != rhs_out_degrees)
    {
      if (trace_verbose) { TRACE("Out degrees differ"); }
      return false;
    }
  }

  #ifdef DO_NOT_USE_PROXY
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
      const auto from_node = lhs.GetEdges()[i].GetFromIndex();
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
  #endif
  return true;
}
*/

bool ribi::cmap::ConceptMap::HasSelectedNodes() const noexcept
{
  return CountSelectedNodes();
}

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
        [conceptmap](const Edge& edge)
        {
          return conceptmap.IsConnectedToCenterNode(edge);
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


  if (lhs.CountSelectedNodes() != rhs.CountSelectedNodes())
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
  for (const auto edge: edges) {
    const auto ed = FindEdge(edge);
    auto selectde_edges_map = get(boost::edge_is_selected,m_graph);
    selectde_edges_map[ed] = true;
  }
}

void ribi::cmap::ConceptMap::AddSelected(const Nodes& nodes) noexcept
{
  for (const auto node: nodes) {
    const auto vd = FindNode(node);
    auto selected_vertices_map = get(boost::vertex_is_selected,m_graph);
    selected_vertices_map[vd] = true;
  }
}

void ribi::cmap::ConceptMap::AddSelected(
  const std::vector<Edge>& edges,
  const Nodes& nodes
) noexcept
{
  AddSelected(edges);
  AddSelected(nodes);
}

/*
ribi::cmap::ConceptMap ribi::cmap::ConceptMap::CreateEmptyConceptMap() noexcept
{
  const ConceptMap conceptmap {
    ConceptMapFactory().Create()
  };
  return conceptmap;
}
*/

bool ribi::cmap::ConceptMap::AreAllEdgeIdsUnique() const noexcept
{
  const auto eip = edges(m_graph);
  std::set<int> ids;
  std::transform(eip.first,eip.second,
    std::inserter(ids,ids.begin()),
    [this](const EdgeDescriptor& ed)
    {
      const auto pmap = get(boost::edge_custom_type,m_graph);
      return get(pmap,ed).GetId();
    }
  );
  return ids.size() == boost::num_edges(m_graph);
}

bool ribi::cmap::ConceptMap::AreAllNodeIdsUnique() const noexcept
{
  const auto vip = vertices(m_graph);
  std::set<int> ids;
  std::transform(vip.first,vip.second,
    std::inserter(ids,ids.begin()),
    [this](const VertexDescriptor& vd)
    {
      const auto pmap = get(boost::vertex_custom_type,m_graph);
      return get(pmap,vd).GetId();
    }
  );
  return ids.size() == boost::num_vertices(m_graph);
}


ribi::cmap::Edge ribi::cmap::ConceptMap::CreateNewEdge() noexcept
{
  assert(GetSelectedNodes().size() == 2);

  const Node from = GetSelectedNodes()[0];
  const Node to   = GetSelectedNodes()[1];
  const Edge edge(EdgeFactory().Create(from,to));

  //Add the Edge
  AddEdge(
    FindNode(from),
    FindNode(to),
    edge
  );

  //Keep track of what is selected
  this->AddSelected( { edge } );

  {
    const auto vd_from = FindNode(from);
    const auto vd_to = FindNode(to);
    auto selected_vertices_map = get(boost::vertex_is_selected,m_graph);
    selected_vertices_map[vd_from] = false;
    selected_vertices_map[vd_to] = false;
  }

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
  assert(HasEdge(edge));
  const auto ed = FindEdge(edge);
  const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
  return edge_is_selected_map[ed];
}

bool ribi::cmap::ConceptMap::IsSelected(const Node& node) const noexcept
{
  assert(HasNode(node) || HasEdgeHaving(node));

  if (HasNode(node))
  {
    const auto vd = FindNode(node);
    const auto vertex_is_selected_map = get(boost::vertex_is_selected, m_graph);
    return vertex_is_selected_map[vd];
  }
  else
  {
    const auto ed = FindEdgeHaving(node);
    const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
    return edge_is_selected_map[ed];
  }
}

void ribi::cmap::ConceptMap::RemoveSelectedness(const std::vector<Edge>& edges) noexcept
{
  if (GetVerbosity())
  {
    std::clog << __func__ << ": of "
      << edges.size() << " edges"
    ;
  }

  for (const auto edge: edges)
  {
    if (!HasEdge(edge))
    {
      std::clog << "Warning: edge to be removed not found" << std::endl;
    }
    else
    {
      const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
      const auto ed = FindEdge(edge);
      edge_is_selected_map[ed] = false;
    }
  }
}

void ribi::cmap::ConceptMap::RemoveSelectedness(
  const Nodes& nodes
) noexcept
{
  if (GetVerbosity())
  {
    std::clog << __func__ << ": of "
      << nodes.size() << " nodes"
    ;
  }

  //Take care: it may be node at the center of an edge
  for (const auto node: nodes)
  {
    if (HasNode(node))
    {
      const auto node_is_selected_map = get(boost::vertex_is_selected, m_graph);
      const auto vd = FindNode(node);
      node_is_selected_map[vd] = false;
    }
    else if (HasEdgeHaving(node))
    {
      const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
      const auto ed = FindEdgeHaving(node);
      edge_is_selected_map[ed] = false;
    }
    else
    {
      if (m_verbose) { std::clog << "Warning: cannot find node to unselect" << std::endl; }
    }
  }
}

void ribi::cmap::ConceptMap::RemoveSelectedness(
  const std::vector<Edge>& edges,
  const Nodes& nodes
) noexcept
{
  RemoveSelectedness(edges);
  RemoveSelectedness(nodes);
}

void ribi::cmap::ConceptMap::SetSelected(
  const Edges& edges,
  const Nodes& nodes
) noexcept
{
  UnselectAllNodes();
  UnselectAllEdges();

  for (const auto node: nodes)
  {
    const auto node_is_selected_map = get(boost::vertex_is_selected, m_graph);
    const auto vd = FindNode(node);
    node_is_selected_map[vd] = true;
  }

  for (const auto edge: edges)
  {
    const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
    const auto ed = FindEdge(edge);
    edge_is_selected_map[ed] = true;
  }
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
  std::stringstream s;
  s << "<concept_map>" << map << "</concept_map>";
  return s.str();
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
    const auto edge_is_selected_map = get(boost::edge_is_selected, m_graph);
    const auto ed = FindEdge(edge);
    edge_is_selected_map[ed] = false;
  }

}

void ribi::cmap::ConceptMap::Unselect(
  const Nodes& nodes
) noexcept
{
  for (const auto node: nodes)
  {
    const auto node_is_selected_map = get(boost::vertex_is_selected, m_graph);
    const auto vd = FindNode(node);
    node_is_selected_map[vd] = false;
  }
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const ConceptMap& m) noexcept
{
  const auto g = m.GetGraph();

  ::boost::write_graphviz(
    os,
    g,
    make_custom_and_selectable_edges_or_vertices_writer(
      get(boost::vertex_custom_type,g),
      get(boost::vertex_is_selected,g)
    ),
    make_custom_and_selectable_edges_or_vertices_writer(
      get(boost::edge_custom_type,g),
      get(boost::edge_is_selected,g)
    )
  );
  return os;
}
#endif // DO_NOT_USE_BOOST_GRAPH
