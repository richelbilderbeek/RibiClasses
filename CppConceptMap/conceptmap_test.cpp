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
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmap.h"

#include <sstream>
#include <set>
#include <iterator>

#include <boost/lexical_cast.hpp>

#include <QRegExp>


#include "conceptmapcenternodefactory.h"
#include "conceptmapcompetency.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
//
//#include "qtconceptmapcommandcreatenewnode.h"
//#include "qtconceptmapcommandcreatenewedge.h"
//#include "qtconceptmapcommanddeleteedge.h"
//#include "qtconceptmapcommanddeletenode.h"
//#include "conceptmapcommandunselectrandom.h"
//#include "conceptmapcommandaddselectedrandom.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "container.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

#ifndef NDEBUG
void ribi::cmap::TestConceptMap() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    container();
    CenterNodeFactory();
    ConceptFactory();
    ExamplesFactory();
    NodeFactory();
    EdgeFactory();
    ConceptMapFactory();
    TestHelperFunctions();
  }
  const TestTimer test_timer(__func__,__FILE__,2.0);
  const bool verbose{true};

  if (verbose) { TRACE("Copy constructable"); }
  {
    const ConceptMap a;
    const ConceptMap b(a);
    assert(a == b);
  }
  if (verbose) { TRACE("operator=="); }
  {
    const ConceptMap a = ConceptMapFactory().GetTest(1);
    const ConceptMap b = ConceptMapFactory().GetTest(2);
    assert(a != b);
    ConceptMap c(a);
    assert(c == a);
    assert(c != b);
    c = b;
    assert(c != a);
    assert(c == b);
  }
  if (verbose) { TRACE("Streaming: empty graph"); }
  {
    ConceptMap a;
    std::stringstream s;
    s << a;
    TRACE(s.str());
    ConceptMap b;
    s >> b;
    assert(a == b);
  }
  if (verbose) { TRACE("Streaming: single object"); }
  {
    ConceptMap a{ConceptMapFactory().GetTest(1)};
    std::stringstream s;
    s << a;
    ConceptMap b;
    s >> b;
    assert(a == b);
  }
  if (verbose) { TRACE("Streaming: two objects"); }
  {
    const ConceptMap e = ConceptMapFactory().GetTest(1);
    const ConceptMap f = ConceptMapFactory().GetTest(2);
    std::stringstream s;
    s << e << f;
    ConceptMap g;
    ConceptMap h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    assert(e == g);
    assert(f == h);
  }
  if (verbose) { TRACE("Nasty examples: one object"); }
  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    ConceptMap f;
    assert(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    assert(e == f);
  }
  if (verbose) { TRACE("Nasty examples: two objects"); }
  for (const ConceptMap e: ConceptMapFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    ConceptMap g;
    ConceptMap h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    assert(e == g);
    assert(e == h);
  }
  if (verbose) { TRACE("Dot conversion"); }
  {
    const std::vector<ConceptMap> v = ConceptMapFactory().GetAllTests();
    std::for_each(v.begin(),v.end(),
      [](const ConceptMap& node)
      {
        //Test copy constructor
        const ConceptMap c(node);
        assert(node == c);
        const std::string s{ToDot(c)};
        const ConceptMap d = DotToConceptMap(s);
        assert(c == d);
      }
    );
  }

  assert(!"Green");

  #ifdef NOT_NOT_20151231
  typedef std::vector<Node> Nodes;
  if (verbose) { TRACE("Create from XML"); } //TODO: Remove, this is a duplicate of CMFactory
  {
    const auto conceptmap = ConceptMapFactory().FromXml("<concept_map><nodes><node><concept><name>X</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>1</is_center_node></node><node><concept><name>C</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node><node><concept><name>B</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node><node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node></nodes><edges></edges></concept_map>");
    assert(!conceptmap.GetVerbosity());
  }
  if (verbose) { TRACE("Copy contructor of empty concept map"); }
  {
    const ConceptMap m{ConceptMapFactory().Create()};
    assert(!m.GetVerbosity());
    const ConceptMap n(m);
    assert(m == n);
  }
  if (verbose) { TRACE("Copy contructor of simple homomorphous concept maps"); }
  for (const auto m: ConceptMapFactory().GetTests())
  {
    assert(!m.GetVerbosity());
    const ConceptMap n(m);
    assert(m == n);
  }
  if (verbose) { TRACE("Copy contructor of complex homomorphous concept map [1]"); }
  {
    const ConceptMap m{
      ConceptMapFactory().GetTest(1)
    };
    assert(!m.GetVerbosity());
    const ConceptMap n{m};
    assert(m == n);
  }
  if (verbose) { TRACE("Copy contructor of complex homomorphous concept maps"); }
  for (const auto m: ConceptMapFactory().GetTests())
  {
    assert(!m.GetVerbosity());
    const ConceptMap n(m);
    assert(m == n);
  }
  if (verbose) { TRACE("Copy contructor of heteromorphous concept maps"); }
  for (const auto m: ConceptMapFactory().GetTests())
  {
    assert(!m.GetVerbosity());
    const ConceptMap n(m);
    assert(m == n);
  }


  assert(!"Green");
  if (verbose) { TRACE("Create tests"); }
  {
    //const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    const std::vector<ConceptMap> v{ConceptMapFactory().GetTests()};
    assert(!v.empty());
  }

  if (verbose) { TRACE("conceptmap.XML std::string has to be between <concept_map> tags"); }
  {
    const auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();
    const auto s = cmap::ToXml(conceptmap);
    assert(s.size() >= 27);
    assert(s.substr(0,13) == "<concept_map>");
    assert(s.substr(s.size() - 14,14) == "</concept_map>");
  }
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("conceptmap.XML->ConceptMap must result in two identical concept maps"); }
  {
    //const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.2);
    const ConceptMap m{ConceptMapFactory().GetHeteromorphousTestConceptMap(19)};
    assert(m);
    const std::string s{cmap::ToXml(*m)};
    const ConceptMap d = ConceptMapFactory().FromXml(s);
    assert(*m == *d);
  }
  #endif // FIX_ISSUE_10
  #ifdef TO_ADD_TO_PROJECTBRAINWEAVER
  //Conversion from Cluster
  {
    const std::vector<boost::shared_ptr<Cluster> > clusters = ClusterFactory::GetTests();
    std::for_each(clusters.begin(),clusters.end(),
      [](const boost::shared_ptr<Cluster> & cluster)
      {
        if (cluster)
        {
          const ConceptMap m(ConceptMapFactory().CreateFromCluster("Focal question",cluster));
          assert(m);
          const std::string s = ConceptMap::ToXml(m);
          const ConceptMap n = ConceptMapFactory().FromXml(s);
          assert(n);
          assert(IsEqual(*m,*n));
        }
      }
    );
  }
  #endif
  if (verbose) { TRACE("CreateSubs"); }
  {
    //const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    //Count the number of expected sub concept maps
    {
      const std::vector<ConceptMap> maps
        = ConceptMapFactory().GetTests();
      const int n_heteromorphous_conceptmaps = 20;
      assert(n_heteromorphous_conceptmaps == static_cast<int>(maps.size())
        && "To warn you if you change the number of testing concept maps");
      const std::vector<int> n_subs_expected = { 0,2,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5 } ;
      assert(n_heteromorphous_conceptmaps == static_cast<int>(n_subs_expected.size()));
      assert(n_subs_expected[ 0] == 0);
      assert(n_subs_expected[ 1] == 2);
      assert(n_subs_expected[ 2] == 3);
      assert(n_subs_expected[ 6] == 3);
      assert(n_subs_expected[ 7] == 4);
      assert(n_subs_expected[14] == 4);
      assert(n_subs_expected[15] == 5);
      //assert(n_subs_expected[16] == 5);

      assert(maps.size() == n_subs_expected.size());
      const int sz = static_cast<int>(n_subs_expected.size());
      for (int i=0; i!=sz; ++i)
      {
        const ConceptMap& map = maps[i];

        const std::vector<ConceptMap> subs = map.CreateSubs();
        assert(static_cast<int>(subs.size()) == n_subs_expected[i]);
      }
    }
  }
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("CountCenterNodes"); }
  //Count the number of CenterNode objects
  {
    //const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    for (const ConceptMap map: ConceptMapFactory().GetHeteromorphousTestConceptMaps())
    {
      if (CountCenterNodes(map) == 0) continue;

      //Count the edges connected to CenterNode
      const int n_edges_connected_to_center { CountCenterNodeEdges(map) };
      const int n_center_nodes_expected
        = n_edges_connected_to_center + 1; //+1, because with no edges, you expect the center node only

      //Count the number of sub concept maps with a center node
      const std::vector<ConceptMap> subs = map->CreateSubs();
      const int n_center_nodes_here {
        static_cast<int>(
          std::count_if(subs.begin(),subs.end(),
            [](const ConceptMap sub)
            {
              return CountCenterNodes(sub) > 0;
            }
          )
        )
      };
      const int n_center_nodes_found = n_center_nodes_here;
      TRACE(n_center_nodes_expected);
      TRACE(n_center_nodes_found);
      assert(n_center_nodes_expected == n_center_nodes_found);

    }
  }
  #endif // FIX_ISSUE_10
  if (verbose) { TRACE("Add two nodes, check selected"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const int n_nodes_before{static_cast<int>(conceptmap.GetNodes().size())};
    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);

    assert(conceptmap.GetSelectedNodes().size() == 0);

    conceptmap.AddNode(node_a);

    assert(conceptmap.GetSelectedNodes().size() == 1);

    conceptmap.AddNode(node_b);

    assert(conceptmap.GetSelectedNodes().size() == 2);

    const int n_nodes_after{static_cast<int>(conceptmap.GetNodes().size())};
    assert(n_nodes_after == n_nodes_before + 2);
  }

  if (verbose) { TRACE("Add node twice, must give a warning"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    assert(conceptmap.GetNodes().size() == 0);
    conceptmap.AddNode(node_a);
    assert(conceptmap.GetNodes().size() == 1);
    TRACE("Next trace should be a warning:");
    conceptmap.AddNode(node_a);
    assert(conceptmap.GetNodes().size() == 1);
  }
  if (verbose) { TRACE("Add two nodes and delete one, check selected"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    assert(conceptmap.GetSelectedNodes().size() == 0);
    conceptmap.AddNode(node_a);
    assert(conceptmap.GetSelectedNodes().size() == 1);
    conceptmap.AddNode(node_b);
    assert(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.DeleteNode(node_a);
    assert(conceptmap.GetSelectedNodes().size() == 1);
  }
  if (verbose) { TRACE("Add two nodes, unselect node_a"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    conceptmap.AddNode(node_a);
    conceptmap.AddNode(node_b);
    assert(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.RemoveSelectedness( Nodes( { node_a } ) );
    assert(conceptmap.GetSelectedNodes().size() == 1);
  }
  if (verbose) { TRACE("Add two nodes, unselect node_b"); }
  {
    auto conceptmap = ConceptMapFactory().GetEmptyConceptMap();

    const auto node_a = NodeFactory().GetTests().at(0);
    const auto node_b = NodeFactory().GetTests().at(1);
    conceptmap.AddNode(node_a);
    conceptmap.AddNode(node_b);
    assert(conceptmap.GetSelectedNodes().size() == 2);
    conceptmap.RemoveSelectedness( Nodes( { node_b } ) );
    assert(conceptmap.GetSelectedNodes().size() == 1);
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
    assert(std::abs(edge->GetNode().GetX() - x3) < 1.0);
    assert(std::abs(edge->GetNode().GetY() - y3) < 1.0);
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
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.CountSelectedEdges() == 1);
    assert(conceptmap.CountSelectedNodes() == 0);
  }
  if (verbose) { TRACE("DeleteNode: delete all two nodes of a concept map"); }
  {
    ConceptMap conceptmap = ConceptMapFactory().GetTest(1);

    assert(conceptmap.GetNodes().size() == 2);
    {
      const auto node = conceptmap.GetNodes().back();
      conceptmap.DeleteNode(node);
      assert(conceptmap.GetNodes().size() == 1);
    }
    {
      const auto node = conceptmap.GetNodes().back();
      conceptmap.DeleteNode(node);
      assert(conceptmap.GetNodes().size() == 0);
    }
  }
  if (verbose) { TRACE("DeleteNode: delete node of a concept map twice"); }
  {
    ConceptMap conceptmap = ConceptMapFactory().GetTest(1);

    assert(conceptmap.GetNodes().size() == 2);
    const auto node = conceptmap.GetNodes().back();
    conceptmap.DeleteNode(node);
    assert(conceptmap.GetNodes().size() == 1);
    conceptmap.DeleteNode(node);
    assert(conceptmap.GetNodes().size() == 1);
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

      assert(conceptmap.GetEdges().size() == n_edges);
      assert(j < conceptmap.GetEdges().size());
      const Edge edge = conceptmap.GetEdges()[j];
      conceptmap.DeleteEdge(edge);
      assert(conceptmap.GetEdges().size() == n_edges - 1
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
    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetEdges().size() == 1);
    conceptmap.DeleteNode(node_from);
    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetEdges().size() == 0);
  }
  #endif // FIX_ISSUE_10
  if (verbose) { TRACE("Is GetNode()[0] a CenterNode?"); }
  {
    ////const TestTimer test_timer(boost::lexical_cast<std::string>(__LINE__),__FILE__,0.1);
    const auto conceptmaps = ConceptMapFactory().GetTests();
    for (const auto& conceptmap: conceptmaps)
    {
      if (conceptmap.GetNodes().empty()) continue;
      assert(conceptmap.FindCenterNode() && "Assume a CenterNode at the center of ConceptMap");
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

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(!conceptmap.GetSelectedNodes().empty());

    const auto node = conceptmap.GetSelectedNodes().front();
    assert(conceptmap.GetEdgesConnectedTo(node).size() == 1);

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

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);
  }
  #endif // FIX_ISSUE_10

  #endif // NOT_NOT_20151231
}
#endif
