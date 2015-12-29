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
#include "conceptmapfactory.h"

#include <cassert>

#include <boost/lexical_cast.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>

#include "conceptmapcenternodefactory.h"
#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapgraphfactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "trace.h"
#include "testtimer.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::ConceptMapFactory::ConceptMapFactory() noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::Create(
  const Graph& graph
) const noexcept
{
  ConceptMap p(graph);
  assert(!p.GetVerbosity());
  return p;
}

#ifndef NDEBUG
/*
ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::DeepCopy(
  const ConceptMap map) const noexcept
{
  if (!map) return ConceptMap();
  assert(map->IsValid() && "Must be a valid original");

  //Deep-copy the center node if present
  //Deep-copy the non-center nodes
  const auto nodes = map->GetNodes();
  Nodes new_nodes;
  for (const auto& node: nodes)
  {
    Node new_node = NodeFactory().Create();

    //Just to be sure, these eight lines...
    if (node.IsCenterNode())
    {
      new_node = node;
      assert(new_node.IsCenterNode());
    }
    else
    {
      new_node = node;
      assert(!new_node.IsCenterNode());
    }
    assert(new_node == node);
    new_nodes.push_back(new_node);
  }


  //Deep-copy the edges
  const std::vector<Edge> edges = map->GetEdges();
  std::vector<Edge> new_edges;
  for (const Edge& edge: edges)
  {
    //Find the new from node
    const auto from_iter = std::find(nodes.begin(),nodes.end(),*edge->GetFrom());
    assert(from_iter != nodes.end());
    const int from_index = std::distance(nodes.begin(),from_iter);
    assert(from_index < boost::numeric_cast<int>(new_nodes.size()));
    const Node from = new_nodes[from_index];

    //Find the new to node
    const auto to_iter = std::find(nodes.begin(),nodes.end(),*edge->GetTo());
    assert(to_iter != nodes.end());
    const int to_index = std::distance(nodes.begin(),to_iter);
    assert(to_index < boost::numeric_cast<int>(new_nodes.size()));
    const Node to = new_nodes[to_index];

    assert(from_index != to_index);
    assert(from != to);
    const Edge new_edge {
      EdgeFactory().DeepCopy(edge,from,to)
    };
    assert(new_edge);
    assert(*new_edge == *edge);
    new_edges.push_back(new_edge);
  }

  const ConceptMap p = Create(new_nodes,new_edges);
  assert(p);
  assert(*p == map && "Must be a copy");
  assert( p !=  map && "Must be a deep copy");
  assert(p->IsValid() && "Must be a valid copy");
  return p;
}
*/
#endif

///NOTE: maybe let CenterNodes manage their own <center_node> tags
ribi::cmap::Graph ribi::cmap::ConceptMapFactory::FromXmlAsTuple(const std::string &s) const
{
  assert(s.size() >= 27);
  assert(s.substr(0,13) == "<concept_map>");
  assert(s.substr(s.size() - 14,14) == "</concept_map>");

  //Obtain the <conceptmap> ... </conceptmap> string
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,("(<concept_map>.*</concept_map>)"));
  assert(v.size() == 1);
  //Strip the <conceptmap> tags
  const std::string conceptmap_str = ribi::xml::StripXmlTag(v[0]);

  std::stringstream f;
  f << conceptmap_str;

  Graph g;
  boost::dynamic_properties p; //_do_ default construct
  p.property("node_id", get(boost::vertex_custom_type, g));
  p.property("label", get(boost::vertex_custom_type, g));
  p.property("regular", get(boost::vertex_is_selected, g));
  p.property("edge_id", get(boost::edge_custom_type, g));
  p.property("label", get(boost::edge_custom_type, g));
  p.property("regular", get(boost::edge_is_selected, g));
  boost::read_graphviz(f,g,p);
  return g;


  /*
  Nodes nodes;
  {
    //Obtain the <nodes> ... </nodes> strings
    const std::vector<std::string> w
      = Regex().GetRegexMatches(conceptmap_str,Regex().GetRegexNodes());
    assert(w.size() == 1);
    //Strip the <nodes> tags
    const std::string nodes_str = ribi::xml::StripXmlTag(w[0]);

    //CenterNode
    {
      const std::vector<std::string> x
        = Regex().GetRegexMatches(nodes_str,Regex().GetRegexCenterNode());
      assert(x.empty() || x.size() == 1);
      std::for_each(x.begin(),x.end(),
        [&nodes](const std::string& s)
        {
          try {
            Node node {
              CenterNodeFactory().FromXml(s)
            };
            nodes.push_back(node);
          }
          catch (std::logic_error&)
          {
            assert(!"Should not get here");
          }
        }
      );
    }
    //Regular nodes
    {
      const std::vector<std::string> x
        = Regex().GetRegexMatches(nodes_str,Regex().GetRegexNode());
      std::for_each(x.begin(),x.end(),
        [&nodes](const std::string& s)
        {
          const Node node {
            NodeFactory().FromXml(s)
          };
          nodes.push_back(node);
        }
      );
    }
    //Replace the first node by its CenterNode equivalent
    if (!nodes.empty() && !IsCenterNode(nodes[0]))
    {
      const auto old_node = nodes[0];
      const auto concept = old_node.GetConcept();
      const double x = old_node.GetX();
      const double y = old_node.GetY();
      const auto center_node = CenterNodeFactory().Create(concept,x,y);
      nodes[0] = center_node;
    }
    assert(nodes.empty() || IsCenterNode(nodes[0]));
  }
  std::vector<Edge> edges;
  {
    //Obtain the <edges> ... </edges> strings
    const std::vector<std::string> w
      = Regex().GetRegexMatches(conceptmap_str,Regex().GetRegexEdges());
    assert(w.size() == 1);
    //Strip the <edges> tags
    const std::string nodes_str = ribi::xml::StripXmlTag(w[0]);
    //Obtain the <edge> ... </edge> strings
    const std::vector<std::string> x
      = Regex().GetRegexMatches(nodes_str,Regex().GetRegexEdge());
    for (const std::string& s: x)
    {
      Edge edge(
        EdgeFactory().FromXml(s)
      );
      edges.push_back(edge); //TODO: emplace_back here
    }
  }
  return std::make_tuple(edges,nodes);
  */
}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::FromXml(const std::string &s) const
{
  auto g = FromXmlAsTuple(s);
  ConceptMap conceptmap(g);
  return conceptmap;

}

ribi::cmap::ConceptMap ribi::cmap::ConceptMapFactory::GetTest(const int i) const noexcept
{
  return ConceptMap(GraphFactory().GetTest(i));
}

std::vector<ribi::cmap::ConceptMap> ribi::cmap::ConceptMapFactory::GetTests() const noexcept
{
  std::vector<ConceptMap> v;
  for (int i=0; i!=20; ++i) { v.push_back(GetTest(i)); }
  return v;
}

#ifndef NDEBUG
void ribi::cmap::ConceptMapFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    container();
    ConceptMapFactory().GetEmptyConceptMap(); //Calls ConceptMap
    CenterNodeFactory();
    EdgeFactory();
  }
  const bool verbose{true};
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const ConceptMapFactory f;
  {
    if (f.GetNumberOfTests() != static_cast<int>(f.GetTests().size()))
    {
      TRACE(f.GetNumberOfTests());
      TRACE(f.GetTests().size());
    }
    assert(f.GetNumberOfTests() == static_cast<int>(f.GetTests().size()));
  }
  if (verbose) { TRACE("All testing concept maps must be valid"); }
  {
    for (const auto& conceptmap: ConceptMapFactory().GetTests())
    {
      assert(!conceptmap.GetVerbosity());
    }
  }
}
#endif // NDEBUG
