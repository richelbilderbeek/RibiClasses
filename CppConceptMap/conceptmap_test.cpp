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
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/lexical_cast.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QRegExp>
#pragma GCC diagnostic pop

#include "conceptmapcenternodefactory.h"
#include "conceptmapcompetency.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapexamplevertexinvariant.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmaphelper.h"
#include "conceptmapisexampleverticesisomorphic.h"
#include "conceptmapisexamplesverticesisomorphic.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "custom_vertex_invariant.h"
#include "fileio.h"
#include "install_vertex_custom_type.h"
#include "is_custom_vertices_isomorphic.h"

#include "make_custom_and_selectable_vertices_writer.h"
#include "my_custom_vertex.h"
#include "testtimer.h"
#include "xml.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_concept_map_is_copy_constructable)
{
  using namespace ribi::cmap;
  const ConceptMap a;
  const ConceptMap b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_operator_is_equal)
{
  using namespace ribi::cmap;
  const ConceptMap a = ConceptMapFactory().GetTest(1);
  const ConceptMap b = ConceptMapFactory().GetTest(2);
  BOOST_CHECK(a != b);
  ConceptMap c(a);
  BOOST_CHECK(c == a);
  BOOST_CHECK(c != b);
  c = b;
  BOOST_CHECK(c != a);
  BOOST_CHECK(c == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_load_node)
{
  using namespace ribi::cmap;
  const std::string s{
    "<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>"
  };
  std::stringstream t;
  t << s;
  Node n;
  t >> n;
  BOOST_CHECK(n.GetConcept().GetName() == "A");
  BOOST_CHECK(ToXml(n) == s);
  ConceptMap g;
  const auto vd = boost::add_vertex(g);
  const auto pmap = get(boost::vertex_custom_type, g);
  put(pmap, vd, n);
  const std::string d{ToDot(g)};
  const std::string dot{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>\", regular=\"0\"];\n"
    "}"
  };
  if (d != dot)
  {
    std::cerr << dot << '\n';
    std::cerr << d << '\n';
  }
  BOOST_CHECK(d == dot); //
  ConceptMap c{DotToConceptMap(dot)};
  BOOST_CHECK(boost::num_edges(c) == 0);
  BOOST_CHECK(boost::num_vertices(c) == 1);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_dot_conversion)
{
  using namespace ribi::cmap;
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string dot{ToDot(c)};
    ConceptMap d{DotToConceptMap(dot)};
    BOOST_CHECK(c == d);
    const std::string dot2{ToDot(d)};
    BOOST_CHECK(dot == dot2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_xml_conversion)
{
  using namespace ribi::cmap;
  for (ConceptMap c: ConceptMapFactory().GetAllTests())
  {
    const std::string xml{ToXml(c)};
    ConceptMap d{XmlToConceptMap(xml)};
    BOOST_CHECK(c == d);
    const std::string xml2{ToXml(d)};
    BOOST_CHECK(xml == xml2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_streaming_empty_graph)
{
  using namespace ribi::cmap;
  ConceptMap a;
  std::stringstream s;
  s << a;
  ConceptMap b;
  s >> b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_simple_dot_to_concept_map)
{
  using namespace ribi::cmap;
  std::string s{
    "graph G {\n"
    "0[label=\"<node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node>\", regular=\"0\"];\n"
    "}"
  };
  {
    std::ofstream f("tmp.dot");
    f << s;
  }
  const ConceptMap c = DotToConceptMap(s);
  BOOST_CHECK(s == ToDot(c));
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_single_object)
{
  using namespace ribi::cmap;
  ConceptMap a{ConceptMapFactory().GetTest(1)};
  std::stringstream s;
  s << a;
  ConceptMap b;
  s >> b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_objects)
{
  using namespace ribi::cmap;
  const ConceptMap e = ConceptMapFactory().GetTest(1);
  const ConceptMap f = ConceptMapFactory().GetTest(2);
  std::stringstream s;
  s << e << f;
  ConceptMap g;
  ConceptMap h;
  s >> g >> h;
  BOOST_CHECK(e == g);
  BOOST_CHECK(f == h);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_one_nasty_object)
{
  using namespace ribi::cmap;
  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    {
      std::ofstream f("tmp.dot");
      f << e;
    }
    std::stringstream s;
    s << e;
    ConceptMap f;
    BOOST_CHECK(e != f);
    s >> f;
    BOOST_CHECK(GetSortedEdges(e) == GetSortedEdges(f));
    BOOST_CHECK(e == f);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_stream_two_nasty_objects)
{
  using namespace ribi::cmap;
  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    ConceptMap g;
    ConceptMap h;
    s >> g >> h;
    BOOST_CHECK(e == g);
    BOOST_CHECK(e == h);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_save_and_load_nasty_test)
{
  using namespace ribi::cmap;
  for (const ConceptMap c: ConceptMapFactory().GetNastyTests())
  {
    const auto filename = ribi::fileio::FileIo().GetTempFileName();
    SaveToFile(c, filename);
    const ConceptMap d = LoadFromFile(filename);
    ribi::fileio::FileIo().DeleteFile(filename);
    BOOST_CHECK(c == d);
  }
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_count_center_nodes)
{
  using namespace ribi::cmap;
  for (const ConceptMap& map: ConceptMapFactory().GetAllTests())
  {
    BOOST_CHECK(CountCenterNodes(map) == 0 || CountCenterNodes(map) == 1);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_test)
{
  using namespace ribi::cmap;
  //BOOST_CHECK(!"Fixed 10?");
  #ifdef NOT_NOT_20151231
  #ifdef FIX_ISSUE_10
  #endif // FIX_ISSUE_10
  if (verbose) { TRACE("Add two nodes, check selected"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const int n_nodes_before{static_cast<int>(boost::num_vertices(conceptmap))};
    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);

    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 0);

    conceptmap.AddNode(node_a);

    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);

    conceptmap.AddNode(node_b);

    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 2);

    const int n_nodes_after{static_cast<int>(boost::num_vertices(conceptmap))};
    BOOST_CHECK(n_nodes_after == n_nodes_before + 2);
  }

  if (verbose) { TRACE("Add node twice, must give a warning"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 0);
    conceptmap.AddNode(node_a);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
    TRACE("Next trace should be a warning:");
    conceptmap.AddNode(node_a);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
  }
  if (verbose) { TRACE("Add two nodes and delete one, check selected"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 0);
    conceptmap.AddNode(node_a);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
    conceptmap.AddNode(node_b);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.DeleteNode(node_a);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
  }
  if (verbose) { TRACE("Add two nodes, unselect node_a"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    conceptmap.AddNode(node_a);
    conceptmap.AddNode(node_b);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.RemoveSelectedness( Nodes( { node_a } ) );
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
  }
  if (verbose) { TRACE("Add two nodes, unselect node_b"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    conceptmap.AddNode(node_a);
    conceptmap.AddNode(node_b);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.RemoveSelectedness( Nodes( { node_b } ) );
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
  }
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("Add nodes and edge, edge its node must be in between the nodes"); }
  {
    const auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const double x1{100.0};
    const double y1{200.0};
    const double x2{300.0};
    const double y2{400.0};
    const double x3{(x1+x2)/2.0};
    const double y3{(y1+y2)/2.0};
    auto node_a = NodeFactory().GetTests().at(0);
    auto node_b = NodeFactory().GetTests().at(0);
    node_a.SetX(x1);
    node_a.SetY(y1);
    node_b.SetX(x2);
    node_b.SetY(y2);
    const auto edge = EdgeFactory().Create(node_a,node_b);
    conceptmap.AddEdge(edge);
    BOOST_CHECK(std::abs(edge.GetNode().GetX() - x3) < 1.0);
    BOOST_CHECK(std::abs(edge.GetNode().GetY() - y3) < 1.0);
  }
  #endif // FIX_ISSUE_10
  if (verbose) { TRACE("Add nodes and edge, check selected"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    const auto edge = EdgeFactory().Create(node_a,node_b);
    const auto vd_a = conceptmap.AddNode(node_a);
    const auto vd_b = conceptmap.AddNode(node_b);
    conceptmap.AddEdge(vd_a, vd_b, edge);
    BOOST_CHECK(boost::num_edges(conceptmap) == 1);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    BOOST_CHECK(conceptmap.CountSelectedEdges() == 1);
    BOOST_CHECK(conceptmap.CountSelectedNodes() == 0);
  }
  if (verbose) { TRACE("DeleteNode: delete all two nodes of a concept map"); }
  {
    ConceptMap conceptmap = ConceptMapFactory().GetTest(1);

    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    {
      const auto node = conceptmap.GetNodes().back();
      conceptmap.DeleteNode(node);
      BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
    }
    {
      const auto node = conceptmap.GetNodes().back();
      conceptmap.DeleteNode(node);
      BOOST_CHECK(boost::num_vertices(conceptmap) == 0);
    }
  }
  if (verbose) { TRACE("DeleteNode: delete node of a concept map twice"); }
  {
    ConceptMap conceptmap = ConceptMapFactory().GetTest(1);

    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    const auto node = conceptmap.GetNodes().back();
    conceptmap.DeleteNode(node);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
    conceptmap.DeleteNode(node);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
  }
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("Deletion of edge"); }
  {
    //const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    const int test_index{19};
    const std::size_t n_edges = ConceptMapFactory().GetHeteromorphousTestConceptMap(test_index)->GetEdges().size();
    for (std::size_t j=0; j!=n_edges; ++j)
    {
      const ConceptMap conceptmap = ConceptMapFactory().GetHeteromorphousTestConceptMap(test_index);

      BOOST_CHECK(boost::num_edges(conceptmap) == n_edges);
      BOOST_CHECK(j < boost::num_edges(conceptmap));
      const Edge edge = conceptmap.GetEdges()[j];
      conceptmap.DeleteEdge(edge);
      BOOST_CHECK(boost::num_edges(conceptmap) == n_edges - 1
        && "Edge must really be gone");
    }
  }
  if (verbose) { TRACE("Deletion of edge, by deleting the from node"); }
  {
    const ConceptMap conceptmap = ConceptMapFactory().GetEmptyConceptMap();
    const auto node_from = NodeFactory().GetTest(0);
    const auto node_to = NodeFactory().GetTest(0);
    conceptmap.AddNode(node_from);
    conceptmap.AddNode(node_to);
    const auto edge = EdgeFactory().GetTest(0,node_from,node_to);
    conceptmap.AddEdge(edge);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    BOOST_CHECK(boost::num_edges(conceptmap) == 1);
    conceptmap.DeleteNode(node_from);
    BOOST_CHECK(boost::num_vertices(conceptmap) == 1);
    BOOST_CHECK(boost::num_edges(conceptmap) == 0);
  }
  #endif // FIX_ISSUE_10
  if (verbose) { TRACE("Is GetNode()[0] a CenterNode?"); }
  {
    ////const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    const auto conceptmaps = ConceptMapFactory().GetTests();
    for (const auto& conceptmap: conceptmaps)
    {
      if (conceptmap.GetNodes().empty()) continue;
      BOOST_CHECK(conceptmap.FindCenterNode() && "Assume a CenterNode at the center of ConceptMap");
    }
  }
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("Delete Node-that-is-head-of-Edge, then Undo"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();

    //Create two nodes
    for (int i=0; i!=2; ++i)
    {
      const auto command = new CommandCreateNewNode(conceptmap);
      conceptmap.DoCommand(command);
    }

    //Create an edge
    {
      const auto command = new CommandCreateNewEdge(conceptmap);
      conceptmap.DoCommand(command);
    }

    //Select a node
    {
      conceptmap.SetSelected( Nodes( { conceptmap.GetNodes().front() } ) );
    }

    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    BOOST_CHECK(boost::num_edges(conceptmap) == 1);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
    BOOST_CHECK(conceptmap.GetSelectedEdges().size() == 0);
    BOOST_CHECK(!conceptmap.GetSelectedNodes().empty());

    const auto node = conceptmap.GetSelectedNodes().front();
    BOOST_CHECK(conceptmap.GetEdgesConnectedTo(node).size() == 1);

    //Delete the node, edge will be deleted as well
    {
      const auto command = new CommandDeleteNode(
        conceptmap, node
      );
      command->SetVerbosity(true);
      conceptmap.DoCommand(command);
    }

    //Undo the deletion, should bring back node and edge
    conceptmap.Undo();

    BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
    BOOST_CHECK(boost::num_edges(conceptmap) == 1);
    BOOST_CHECK(conceptmap.GetSelectedNodes().size() == 1);
    BOOST_CHECK(conceptmap.GetSelectedEdges().size() == 0);
  }
  #endif // FIX_ISSUE_10

  #endif // NOT_NOT_20151231
}
