#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>

#include "conceptmapgraphfactory.h"

#include <cassert>

#include <boost/lexical_cast.hpp>

#include "conceptmapcenternodefactory.h"
#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "trace.h"
#include "testtimer.h"
#include "xml.h"

#pragma GCC diagnostic pop

ribi::cmap::GraphFactory::GraphFactory() noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::VertexDescriptor ribi::cmap::GraphFactory::AddVertex(
  const Node& node, Graph& g
) const noexcept
{
  const auto vd = boost::add_vertex(g);
  const auto pmap = get(boost::vertex_custom_type, g);
  put(pmap, vd, node);
  return vd;
}

void ribi::cmap::GraphFactory::AddEdge(
  const Edge& edge,
  const VertexDescriptor& vd_from,
  const VertexDescriptor& vd_to,
  Graph& g
) const noexcept
{
  const auto ed = boost::add_edge(vd_from, vd_to, g);
  assert(ed.second);
  const auto pmap = get(boost::edge_custom_type, g);
  put(pmap, ed.first, edge);
  //return ed.first;

}

ribi::cmap::Graph ribi::cmap::GraphFactory::FromXml(const std::string &s) const
{
  assert(s.size() < 13 || s.substr(0,13) != "<concept_map>");

  std::stringstream f;
  f << s;

  Graph g;
  boost::dynamic_properties dp(boost::ignore_other_properties);
  dp.property("label", get(boost::vertex_custom_type, g));
  dp.property("regular", get(boost::vertex_is_selected, g));
  dp.property("edge_id", get(boost::edge_custom_type, g));
  dp.property("label", get(boost::edge_custom_type, g));
  dp.property("regular", get(boost::edge_is_selected, g));
  boost::read_graphviz(f,g,dp);
  return g;
}

std::vector<ribi::cmap::Graph> ribi::cmap::GraphFactory::GetNastyTests() const noexcept
{
  std::vector<Graph> v{
    GetNasty0()
  };
  return v;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetTest(const int i) const noexcept
{
  switch (i)
  {
    case 0: return Get0();
    case 1: return Get1();
    case 2: return Get2();
    case 3: return Get3();
    case 4: return Get4();
    case 5: return Get5();
    case 6: return Get6();
    default: assert(!"Should not get here");
  }
  assert(!"Should not get here");
  return Graph();
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get0() const noexcept
{
  return Graph();
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get1() const noexcept
{
  Graph g;
  //AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  AddVertex(NodeFactory().CreateFromStrings("A"), g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get2() const noexcept
{
  Graph g;
  AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  AddVertex(NodeFactory().CreateFromStrings("A"), g);
  AddVertex(NodeFactory().CreateFromStrings("B"), g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get3() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  AddEdge(
    Edge(Node(Concept("edge_a concept"),1.2,3.4)),vd_1,vd_2,g
  );
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get4() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("B"), g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_1,vd_2,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_2,vd_3,g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get5() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("B"), g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_1,vd_2,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_2,vd_3,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_3,vd_1,g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get6() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("B"), g);
  const auto vd_4 = AddVertex(NodeFactory().CreateFromStrings("C"), g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_1,vd_2,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_2,vd_3,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_3,vd_1,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_4,vd_1,g);
  return g;
}

#ifdef REALLY_CARE_20151229
ribi::cmap::Graph ribi::cmap::GraphFactory::Get15() const noexcept
{
  //[15]
  const auto concept_c(ConceptFactory().Create("B", { {"B-1",cmap::Competency::uninitialized} },0,1,2));
  const auto concept_d(ConceptFactory().Create("C", { {"C-1",cmap::Competency::uninitialized},{"C-2",cmap::Competency::misc}},-1,1,2));
  const auto concept_e(ConceptFactory().Create("D", { {"D-1",cmap::Competency::misc},{"D-2",cmap::Competency::misc},{"D-3",cmap::Competency::misc}},-1,-1,-1));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));
  const Node node_e(NodeFactory().Create(concept_e));

  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      node_c,
      node_d,
      node_e
    };

  const auto concept_f(ConceptFactory().Create("1"));
  const auto concept_h(ConceptFactory().Create("2",{{"2-I",cmap::Competency::misc} } ));
  const auto concept_g(ConceptFactory().Create("3",{{"3-I",cmap::Competency::misc},{"3-II",cmap::Competency::misc} } ));
  const auto concept_i(ConceptFactory().Create("4",{{"4-I",cmap::Competency::misc},{"4-II",cmap::Competency::misc},{"4-III",cmap::Competency::misc} } ));
  const auto concept_j(ConceptFactory().Create("5",{{"5-I",cmap::Competency::misc},{"5-II",cmap::Competency::misc},{"5-III",cmap::Competency::misc},{"5-IV",cmap::Competency::misc} } ));
  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));
  const auto node_h(NodeFactory().Create(concept_h,3.4,5.6));
  const auto node_i(NodeFactory().Create(concept_i,4.5,6.7));
  const auto node_j(NodeFactory().Create(concept_j,5.6,7.8));

  Edges edges
    =
    {
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_h,nodes.at(4),false,nodes.at(3),true),
      EdgeFactory().Create(node_i,nodes.at(1),false,nodes.at(4),true),
      EdgeFactory().Create(node_j,nodes.at(0),false,nodes.at(1),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get16() const noexcept
{
  //[16]: complex rated concept map
  const auto concept_c(ConceptFactory().Create("B", { {"B-1: profession",cmap::Competency::profession} },0,1,2));
  const auto concept_d(ConceptFactory().Create("C", { {"C-1: organisations",cmap::Competency::organisations},{"C-2: social_surroundings",cmap::Competency::social_surroundings}},2,1,0));
  const auto concept_e(ConceptFactory().Create("D", { {"D-1: target_audience",cmap::Competency::target_audience},{"D-2: ti_knowledge",cmap::Competency::ti_knowledge},{"D-3: prof_growth",cmap::Competency::prof_growth},{"D-4: misc",cmap::Competency::misc}},0,1,2));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));
  const Node node_e(NodeFactory().Create(concept_e));

  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      node_c,
      node_d,
      node_e
    };

  const auto concept_f(ConceptFactory().Create("1"));
  const auto concept_h(ConceptFactory().Create("2",{{"2-I",cmap::Competency::misc} } ));
  const auto concept_g(ConceptFactory().Create("3",{{"3-I",cmap::Competency::misc},{"3-II",cmap::Competency::misc} } ));
  const auto concept_i(ConceptFactory().Create("4",{{"4-I",cmap::Competency::misc},{"4-II",cmap::Competency::misc},{"4-III",cmap::Competency::misc} } ));
  const auto concept_j(ConceptFactory().Create("5",{{"5-I",cmap::Competency::misc},{"5-II",cmap::Competency::misc},{"5-III",cmap::Competency::misc},{"5-IV",cmap::Competency::misc} } ));
  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));
  const auto node_h(NodeFactory().Create(concept_h,3.4,5.6));
  const auto node_i(NodeFactory().Create(concept_i,4.5,6.7));
  const auto node_j(NodeFactory().Create(concept_j,5.6,7.8));


  Edges edges
    =
    {
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_h,nodes.at(4),false,nodes.at(3),true),
      EdgeFactory().Create(node_i,nodes.at(1),false,nodes.at(4),true),
      EdgeFactory().Create(node_j,nodes.at(0),false,nodes.at(1),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get17() const noexcept
{
  //[17]: complex rated concept map with many examples
  const auto concept_c(ConceptFactory().Create("B",
    {
      {"B-P",cmap::Competency::profession},
      {"B-O",cmap::Competency::organisations},
      {"B-S",cmap::Competency::social_surroundings},
      {"B-TA",cmap::Competency::target_audience},
      {"B-TK",cmap::Competency::ti_knowledge},
      {"B-PG",cmap::Competency::prof_growth},
      {"B-M",cmap::Competency::misc}
    },
    0,1,2));
  const auto concept_d(ConceptFactory().Create("C",
    { {"C-1: organisations",cmap::Competency::organisations},{"C-2: social_surroundings",cmap::Competency::social_surroundings}},
    2,1,0));
  const auto concept_e(ConceptFactory().Create("D",
    { {"D-1: target_audience",cmap::Competency::target_audience},{"D-2: ti_knowledge",cmap::Competency::ti_knowledge},{"D-3: prof_growth",cmap::Competency::prof_growth},{"D-4: misc",cmap::Competency::misc}},
    0,1,2));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));
  const Node node_e(NodeFactory().Create(concept_e));

  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      node_c,
      node_d,
      node_e
    };

  const auto concept_f(ConceptFactory().Create("1",
    {{"2-I",cmap::Competency::misc}}
    ));
  const auto concept_h(ConceptFactory().Create("2",
    {{"2-I",cmap::Competency::misc} }
    ));
  const auto concept_g(ConceptFactory().Create("3",
    {{"3-I",cmap::Competency::misc},{"3-II",cmap::Competency::misc} }
    ));
  const auto concept_i(ConceptFactory().Create("4",
    {{"4-I",cmap::Competency::misc},{"4-II",cmap::Competency::misc},{"4-III",cmap::Competency::misc} }
    ));
  const auto concept_j(ConceptFactory().Create("5",
    {{"5-I",cmap::Competency::misc},{"5-II",cmap::Competency::misc},{"5-III",cmap::Competency::misc},{"5-IV",cmap::Competency::misc} }
    ));

  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));
  const auto node_h(NodeFactory().Create(concept_h,3.4,5.6));
  const auto node_i(NodeFactory().Create(concept_i,4.5,6.7));
  const auto node_j(NodeFactory().Create(concept_j,5.6,7.8));

  Edges edges
    =
    {
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_h,nodes.at(4),false,nodes.at(3),true),
      EdgeFactory().Create(node_i,nodes.at(1),false,nodes.at(4),true),
      EdgeFactory().Create(node_j,nodes.at(0),false,nodes.at(1),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get18() const noexcept
{
  //[18]: complex rated concept map with many long concept names and examples
  const auto concept_c(ConceptFactory().Create(
    "B: This is a concept that has all types of competencies as its examples, each example name being multiple lines as well",
    {
      {"B-P: this is B its first example (out of seven) and it is categorized as a competency in the profession domain",cmap::Competency::profession},
      {"B-O: this is B its second example (out of seven) and it is categorized as a competency in the organisation domain",cmap::Competency::organisations},
      {"B-S: this is B its third example (out of seven) and it is categorized as a competency in the social surroundings domain",cmap::Competency::social_surroundings},
      {"B-TA: this is B its fourth example (out of seven) and it is categorized as a competency in the target audience domain",cmap::Competency::target_audience},
      {"B-TK: this is B its fifth example (out of seven) and it is categorized as a competency in the technical instrumental knowledge domain",cmap::Competency::ti_knowledge},
      {"B-PG: this is B its sixth example (out of seven) and it is categorized as a competency in the professional growth domain",cmap::Competency::prof_growth},
      {"B-M: this is B its seventh example (out of seven) and it is categorized as a competency in the misc domain",cmap::Competency::misc}
    },
    0,1,2));
  const auto concept_d(ConceptFactory().Create(
    "C: This is a concept that has only two of the seven types of competencies as its examples, each example name being multiple lines as well",
    {
      {"C-O: this is C its first example (out of two) and it is categorized as a competency in the organisation domain",cmap::Competency::organisations},
      {"C-S: this is C its second example (out of two) and it is categorized as a competency in the social surroundings domain",cmap::Competency::social_surroundings}
    },
    2,1,0));
  const auto concept_e(ConceptFactory().Create(
    "D: This is a concept that has only four of the seven types of competencies as its examples, each example name being multiple lines as well",
    {
      {"D-TA: this is D its first example (out of four) and it is categorized as a competency in the target audience domain",cmap::Competency::target_audience},
      {"D-TK: this is D its second example (out of four) and it is categorized as a competency in the technical instrumental knowledge domain",cmap::Competency::ti_knowledge},
      {"D-PG: this is D its third example (out of four) and it is categorized as a competency in the professional growth domain",cmap::Competency::prof_growth},
      {"D-M: this is D its fourth example (out of four) and it is categorized as a competency in the misc domain",cmap::Competency::misc}
    },
    0,1,2));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));
  const Node node_e(NodeFactory().Create(concept_e));

  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X: This is the center node concept that can have no examples, oterwise each of its example name would be multiple lines"),
      NodeFactory().CreateFromStrings("A: This is a concept that has none of all types of competencies as its examples, oterwise each of its example name would be multiple lines"),
      node_c,
      node_d,
      node_e
    };

  const auto concept_f(
    ConceptFactory().Create(
      "1: the first description of a relation that has one example. This description spans multiple lines as it is longer than eighty characters",
      {
        //{"2-I: this misc example ",cmap::Competency::misc}
      }
    )
  );
  const auto concept_h(
    ConceptFactory().Create(
      "2: the second description of a relation that has one example. This description spans multiple lines as it is longer than eighty characters",
      {
        {"2-I: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc}
      }
    )
  );
  const auto concept_g(
    ConceptFactory().Create(
      "3: the third description of a relation that has one example. This description spans multiple lines as it is longer than eighty characters",
      {
        {"3-I: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"3-II: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc}
      }
    )
  );
  const auto concept_i(
    ConceptFactory().Create(
      "4: the fourth description of a relation that has one example. This description spans multiple lines as it is longer than eighty characters",
      {
        {"4-I: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"4-II: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"4-III: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc}
      }
    )
  );
  const auto concept_j(
    ConceptFactory().Create(
      "5: the fifth description of a relation that has one example. This description spans multiple lines as it is longer than eighty characters",
      {
        {"5-I: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"5-II: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"5-III: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc},
        {"5-IV: an example of a description of a relation. This example spans multiple lines as it is longer than eighty characters",cmap::Competency::misc}
      }
    )
  );

  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));
  const auto node_h(NodeFactory().Create(concept_h,3.4,5.6));
  const auto node_i(NodeFactory().Create(concept_i,4.5,6.7));
  const auto node_j(NodeFactory().Create(concept_j,5.6,7.8));

  Edges edges
    =
    {
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_h,nodes.at(4),false,nodes.at(3),true),
      EdgeFactory().Create(node_i,nodes.at(1),false,nodes.at(4),true),
      EdgeFactory().Create(node_j,nodes.at(0),false,nodes.at(1),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::Get19() const noexcept
{
  //[19]: complex rated concept map with all nodes connected
  const auto concept_c(ConceptFactory().Create("B", { {"B-1: profession",cmap::Competency::profession} },0,1,2));
  const auto concept_d(ConceptFactory().Create("C", { {"C-1: organisations",cmap::Competency::organisations},{"C-2: social_surroundings",cmap::Competency::social_surroundings}},2,1,0));
  const auto concept_e(ConceptFactory().Create("D", { {"D-1: target_audience",cmap::Competency::target_audience},{"D-2: ti_knowledge",cmap::Competency::ti_knowledge},{"D-3: prof_growth",cmap::Competency::prof_growth},{"D-4: misc",cmap::Competency::misc}},0,1,2));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));
  const Node node_e(NodeFactory().Create(concept_e));

  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      node_c,
      node_d,
      node_e
    };

  const auto concept_f(ConceptFactory().Create("1"));
  const auto concept_h(ConceptFactory().Create("2",{{"2-I",cmap::Competency::misc} } ));
  const auto concept_g(ConceptFactory().Create("3",{{"3-I",cmap::Competency::misc},{"3-II",cmap::Competency::misc} } ));
  const auto concept_i(ConceptFactory().Create("4",{{"4-I",cmap::Competency::misc},{"4-II",cmap::Competency::misc},{"4-III",cmap::Competency::misc} } ));

  ///Concepts connected to the center should never be visible
  const auto concept_j(ConceptFactory().Create("..."));
  const auto concept_k(ConceptFactory().Create("..."));
  const auto concept_l(ConceptFactory().Create("..."));
  const auto concept_m(ConceptFactory().Create("..."));
  const auto concept_n(ConceptFactory().Create("..."));

  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));
  const auto node_h(NodeFactory().Create(concept_h,3.4,5.6));
  const auto node_i(NodeFactory().Create(concept_i,4.5,6.7));
  const auto node_j(NodeFactory().Create(concept_j,5.6,7.8));
  const auto node_k(NodeFactory().Create(concept_k,6.7,8.9));
  const auto node_l(NodeFactory().Create(concept_l,7.8,9.0));
  const auto node_m(NodeFactory().Create(concept_m,8.9,0.1));

  Edges edges
    =
    {
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true ),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true ),
      EdgeFactory().Create(node_h,nodes.at(4),false,nodes.at(3),true ),
      EdgeFactory().Create(node_i,nodes.at(1),false,nodes.at(4),true ),
      EdgeFactory().Create(node_j,nodes.at(0),false,nodes.at(1),true ),
      EdgeFactory().Create(node_k,nodes.at(0),false,nodes.at(2),false),
      EdgeFactory().Create(node_l,nodes.at(0),false,nodes.at(3),true ),
      EdgeFactory().Create(node_m,nodes.at(0),true ,nodes.at(4),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges)
  );

  assert(conceptmap.IsValid());
  return conceptmap;
}

#endif // REALLY_CARE_20151229

ribi::cmap::Graph ribi::cmap::GraphFactory::GetNasty0() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X<Y"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings(" A>B"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("BC "), g);
  const auto vd_4 = AddVertex(NodeFactory().CreateFromStrings(" C "), g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_1,vd_2,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_2,vd_3,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_3,vd_1,g);
  AddEdge(EdgeFactory().Create((Node(ConceptFactory().Create("edge_a concept"),1.2,3.4))),vd_4,vd_1,g);
  return g;
}

std::vector<ribi::cmap::Graph > ribi::cmap::GraphFactory::GetAllTests() const noexcept
{
  std::vector<Graph> v{
    Get0(),
    Get1(),
    Get2(),
    Get3(),
    Get4(),
    Get5(),
    Get6(),
  };
  return v;
}

#ifndef NDEBUG
void ribi::cmap::GraphFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    container();
    GraphFactory().GetEmptyGraph(); //Calls Graph
  }
  const bool verbose{true};
  CenterNodeFactory();
  EdgeFactory();
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const GraphFactory f;
  {
    if (f.GetNumberOfAllTests() != static_cast<int>(f.GetAllTests().size()))
    {
      TRACE(f.GetNumberOfAllTests());
      TRACE(f.GetAllTests().size());
    }
    assert(f.GetNumberOfAllTests() == static_cast<int>(f.GetAllTests().size()));
  }
  if (verbose) { TRACE("All the testing concept maps to be valid"); }
  {
    const int n{f.GetNumberOfAllTests()};
    for (int i=0; i!=n; ++i) {
      f.GetTest(i);
    }
  }
}
#endif // NDEBUG
