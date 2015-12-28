#include "conceptmapgraphfactory.h"

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

ribi::cmap::Graph ribi::cmap::GraphFactory::FromXml(const std::string &s) const
{
  assert(s.size() < 13 || s.substr(0,13) != "<concept_map>");

  std::stringstream f;
  f << s;

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
}

std::vector<ribi::cmap::Graph > ribi::cmap::GraphFactory::GetAllTests() const noexcept
{
  Graphs v;
  {
    {
      const auto w = GraphFactory::GetSimpleHomomorphousTestGraphs();
      std::copy(w.begin(),w.end(),std::back_inserter(v));
    }
    {
      const std::vector<Graph> w = GraphFactory::GetComplexHomomorphousTestGraphs();
      std::copy(w.begin(),w.end(),std::back_inserter(v));
    }
    {
      const auto w = GraphFactory::GetHeteromorphousTestGraphs();
      std::copy(w.begin(),w.end(),std::back_inserter(v));
    }
  }
  //Add empty concept map
  {
    Graph p;
    v.push_back(p);
  }
  return v;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph(const int i) const noexcept
{
  switch (i)
  {
    case 0: return GetHeteromorphousTestGraph0();
    case 1: return GetHeteromorphousTestGraph1();
    case 2: return GetHeteromorphousTestGraph2();
    case 3: return GetHeteromorphousTestGraph3();
    case 4: return GetHeteromorphousTestGraph4();
    case 5: return GetHeteromorphousTestGraph5();
    case 6: return GetHeteromorphousTestGraph6();
    case 7: return GetHeteromorphousTestGraph7();
    case 8: return GetHeteromorphousTestGraph8();
    case 9: return GetHeteromorphousTestGraph9();
    case 10: return GetHeteromorphousTestGraph10();
    case 11: return GetHeteromorphousTestGraph11();
    case 12: return GetHeteromorphousTestGraph12();
    case 13: return GetHeteromorphousTestGraph13();
    case 14: return GetHeteromorphousTestGraph14();
    case 15: return GetHeteromorphousTestGraph15();
    case 16: return GetHeteromorphousTestGraph16();
    case 17: return GetHeteromorphousTestGraph17();
    case 18: return GetHeteromorphousTestGraph18();
    case 19: return GetHeteromorphousTestGraph19();
    default: assert(!"Should not get here");
  }
  assert(!"Should not get here");
  return Graph();
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph0() const noexcept
{
  return Graph();
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph1() const noexcept
{
  Graph g;
  AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  AddVertex(NodeFactory().CreateFromStrings("A"), g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph2() const noexcept
{
  Graph g;
  AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  AddVertex(NodeFactory().CreateFromStrings("A"), g);
  AddVertex(NodeFactory().CreateFromStrings("B"), g);
  return g;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph3() const noexcept
{
  Graph g;
  const auto vd_1 = AddVertex(CenterNodeFactory().CreateFromStrings("X"), g);
  const auto vd_2 = AddVertex(NodeFactory().CreateFromStrings("A"), g);
  const auto vd_3 = AddVertex(NodeFactory().CreateFromStrings("B"), g);
  AddEdge(Edge(NodeFactory().Create(Concept("edge_a concept"),1.2,3.4)));
  return g;

  const auto concept_d(ConceptFactory().Create("edge_a concept"));
  const auto node_d(NodeFactory().Create(concept_d,1.2,3.4));

  Edges edges
    =
    {
      //cmap::EdgeFactory().Create(concept_d,1.2,3.4,nodes.at(1),false,nodes.at(2),true)
      EdgeFactory().Create(node_d,nodes.at(1),false,nodes.at(2),true)
    };
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph4() const noexcept
{
  //[4]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B")
    };

  const auto concept_d(ConceptFactory().Create("edge_a concept"));
  const auto node_d(NodeFactory().Create(concept_d,1.2,3.4));

  Edges edges
    =
    {
      //cmap::EdgeFactory().Create(concept_d,1.2,3.4,nodes.at(2),false,nodes.at(1),true)
      EdgeFactory().Create(node_d,nodes.at(2),false,nodes.at(1),true)
    };

  const auto conceptmap
    = GraphFactory().Create(nodes,edges);


  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph5() const noexcept
{
  //[5]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B")
    };

  const auto concept_d(ConceptFactory().Create("1"));
  const auto node_d(NodeFactory().Create(concept_d,1.2,3.4));

  Edges edges
    =
    {
      EdgeFactory().Create(node_d,nodes.at(1),false,nodes.at(2),true)
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph6() const noexcept
{
  //[6]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B")
    };
  const auto concept_d(ConceptFactory().Create("1"));
  const auto node_d(NodeFactory().Create(concept_d,1.2,3.4));

  Edges edges
    =
    {
      EdgeFactory().Create(node_d,nodes.at(2),false,nodes.at(1),true)
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph7() const noexcept
{
  //[7]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  Edges edges
    =
    {

    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph8() const noexcept
{
  //[8]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create());
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto edge_a(EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(2),true));


  Edges edges
    =
    {
      edge_a
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph9() const noexcept
{
  //[9]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create());
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const Edge edge_a(EdgeFactory().Create(node_e,nodes.at(2),false,nodes.at(3),true));

  Edges edges
    =
    {
      edge_a
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph10() const noexcept
{
  //[10]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create());
  const auto concept_f(ConceptFactory().Create());
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(2),true),
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(3),true)
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph11() const noexcept
{
  //[11]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));

  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(2),true),
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(3),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(1),true)
    };

  const Graph conceptmap
    = GraphFactory().Create(nodes,edges);


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph12() const noexcept
{
  //[12]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));

  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_f,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_g,nodes.at(1),false,nodes.at(3),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph13() const noexcept
{
  //[13]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("3"));
  const auto concept_g(ConceptFactory().Create("2"));

  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(2),true),
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(3),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(1),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph14() const noexcept
{
  //[14]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("3"));
  const auto concept_g(ConceptFactory().Create("2"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_f,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_g,nodes.at(1),false,nodes.at(3),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));


  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph15() const noexcept
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

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph16() const noexcept
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

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph17() const noexcept
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

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph18() const noexcept
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

ribi::cmap::Graph ribi::cmap::GraphFactory::GetHeteromorphousTestGraph19() const noexcept
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

std::vector<ribi::cmap::Graph > ribi::cmap::GraphFactory::GetHeteromorphousTestGraphs() const noexcept
{
  std::vector<Graph> v{
    GetHeteromorphousTestGraph0(),
    GetHeteromorphousTestGraph1(),
    GetHeteromorphousTestGraph2(),
    GetHeteromorphousTestGraph3(),
    GetHeteromorphousTestGraph4(),
    GetHeteromorphousTestGraph5(),
    GetHeteromorphousTestGraph6(),
    GetHeteromorphousTestGraph7(),
    GetHeteromorphousTestGraph8(),
    GetHeteromorphousTestGraph9(),
    GetHeteromorphousTestGraph10(),
    GetHeteromorphousTestGraph11(),
    GetHeteromorphousTestGraph12(),
    GetHeteromorphousTestGraph13(),
    GetHeteromorphousTestGraph14(),
    GetHeteromorphousTestGraph15(),
    GetHeteromorphousTestGraph16(),
    GetHeteromorphousTestGraph17(),
    GetHeteromorphousTestGraph18(),
    GetHeteromorphousTestGraph19()
  };
  return v;
}


ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph(const int i) const noexcept
{
  switch (i)
  {
    case 0: return GetComplexHomomorphousTestGraph0();
    case 1: return GetComplexHomomorphousTestGraph1();
    case 2: return GetComplexHomomorphousTestGraph2();
    case 3: return GetComplexHomomorphousTestGraph3();
    case 4: return GetComplexHomomorphousTestGraph4();
    case 5: return GetComplexHomomorphousTestGraph5();
    case 6: return GetComplexHomomorphousTestGraph6();
    case 7: return GetComplexHomomorphousTestGraph7();
    case 8: return GetComplexHomomorphousTestGraph8();
    case 9: return GetComplexHomomorphousTestGraph9();
    case 10: return GetComplexHomomorphousTestGraph10();
    case 11: return GetComplexHomomorphousTestGraph11();
    default: assert(!"Should not get here");
  }
  assert(!"Should not get here");
  return Graph();
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph0() const noexcept
{
  //[0] (note: same as heteromorphous[11])
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C"),
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));

  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,1.2,3.4));
  const auto node_g(NodeFactory().Create(concept_g,2.3,4.5));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(2),true),
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(3),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(1),true)
    };

  Graph conceptmap(
    GraphFactory().Create(nodes,edges)
  );

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph1() const noexcept
{
  //[1]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("B")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),false,nodes.at(3),true),
      EdgeFactory().Create(node_f,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_g,nodes.at(2),false,nodes.at(1),true)
    };

  assert(CanConstruct(nodes,edges));
  Graph conceptmap(
    GraphFactory().Create(nodes,edges)
  );

  assert(CanConstruct(conceptmap.GetNodes(),conceptmap.GetEdges()));
  assert(conceptmap.IsValid());
  TRACE(conceptmap);
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph2() const noexcept
{
  //[2]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_f,nodes.at(1),false,nodes.at(3),true),
      EdgeFactory().Create(node_g,nodes.at(3),false,nodes.at(2),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph3() const noexcept
{
  //[3]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("A")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(3),false,nodes.at(1),true),
      EdgeFactory().Create(node_f,nodes.at(1),false,nodes.at(2),true),
      EdgeFactory().Create(node_g,nodes.at(2),false,nodes.at(3),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph4() const noexcept
{
  //[4]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),false,nodes.at(3),true),
      EdgeFactory().Create(node_f,nodes.at(3),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(1),false,nodes.at(2),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph5() const noexcept
{
  //[5]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("A")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(3),false,nodes.at(2),true),
      EdgeFactory().Create(node_f,nodes.at(2),false,nodes.at(1),true),
      EdgeFactory().Create(node_g,nodes.at(1),false,nodes.at(3),true)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph6() const noexcept
{
  //[6] (note: same as heteromorphous[11], yet arrows reversed
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),true,nodes.at(1),false),
      EdgeFactory().Create(node_f,nodes.at(3),true,nodes.at(2),false),
      EdgeFactory().Create(node_g,nodes.at(1),true,nodes.at(3),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph7() const noexcept
{
  //[7]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("B")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(3),true,nodes.at(1),false),
      EdgeFactory().Create(node_f,nodes.at(2),true,nodes.at(3),false),
      EdgeFactory().Create(node_g,nodes.at(1),true,nodes.at(2),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph8() const noexcept
{
  //[8]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("C")
    };
  const auto concept_a(ConceptFactory().Create("X"));
  const auto concept_b(ConceptFactory().Create("B"));
  const auto concept_c(ConceptFactory().Create("A"));
  const auto concept_d(ConceptFactory().Create("C"));
  const Node node_a(NodeFactory().Create(concept_a));
  const Node node_b(NodeFactory().Create(concept_b));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),true,nodes.at(2),false),
      EdgeFactory().Create(node_f,nodes.at(3),true,nodes.at(1),false),
      EdgeFactory().Create(node_g,nodes.at(2),true,nodes.at(3),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph9() const noexcept
{
  //[9]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("A")
    };
  const auto concept_a(ConceptFactory().Create("X"));
  const auto concept_b(ConceptFactory().Create("B"));
  const auto concept_c(ConceptFactory().Create("C"));
  const auto concept_d(ConceptFactory().Create("A"));
  const Node node_a(NodeFactory().Create(concept_a));
  const Node node_b(NodeFactory().Create(concept_b));
  const Node node_c(NodeFactory().Create(concept_c));
  const Node node_d(NodeFactory().Create(concept_d));

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(1),true,nodes.at(3),false),
      EdgeFactory().Create(node_f,nodes.at(2),true,nodes.at(1),false),
      EdgeFactory().Create(node_g,nodes.at(3),true,nodes.at(2),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph10() const noexcept
{
  //[10]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("A"),
      NodeFactory().CreateFromStrings("B")
    };
  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(3),true,nodes.at(2),false),
      EdgeFactory().Create(node_f,nodes.at(1),true,nodes.at(3),false),
      EdgeFactory().Create(node_g,nodes.at(2),true,nodes.at(1),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}


ribi::cmap::Graph ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraph11() const noexcept
{
  //[11]
  Nodes nodes
    =
    {
      CenterNodeFactory().CreateFromStrings("X"),
      NodeFactory().CreateFromStrings("C"),
      NodeFactory().CreateFromStrings("B"),
      NodeFactory().CreateFromStrings("A")
    };

  const auto concept_e(ConceptFactory().Create("1"));
  const auto concept_f(ConceptFactory().Create("2"));
  const auto concept_g(ConceptFactory().Create("3"));
  const auto node_e(NodeFactory().Create(concept_e,1.2,3.4));
  const auto node_f(NodeFactory().Create(concept_f,2.3,4.5));
  const auto node_g(NodeFactory().Create(concept_g,3.4,5.6));

  Edges edges
    =
    {
      EdgeFactory().Create(node_e,nodes.at(2),true,nodes.at(3),false),
      EdgeFactory().Create(node_f,nodes.at(1),true,nodes.at(2),false),
      EdgeFactory().Create(node_g,nodes.at(3),true,nodes.at(1),false)
    };

  const Graph conceptmap(
    GraphFactory().Create(nodes,edges));

  assert(conceptmap.IsValid());
  return conceptmap;
}

std::vector<ribi::cmap::Graph > ribi::cmap::GraphFactory::GetComplexHomomorphousTestGraphs() const noexcept
{
  std::vector<Graph> v;
  for (int i=0; i!=12; ++i) { v.push_back(GetComplexHomomorphousTestGraph(i)); }
  return v;
}

std::vector<ribi::cmap::Graph > ribi::cmap::GraphFactory::GetSimpleHomomorphousTestGraphs() const noexcept
{
  std::vector<Graph> v;
  //[0]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("A"),
        NodeFactory().CreateFromStrings("B"),
        NodeFactory().CreateFromStrings("C")
      };
    Edges edges
      =
      {

      };

    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    assert(conceptmap.IsValid());
    v.push_back(conceptmap);
  }

  //[1]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("A"),
        NodeFactory().CreateFromStrings("C"),
        NodeFactory().CreateFromStrings("B")
      };
    Edges edges
      =
      {

      };
    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    v.push_back(conceptmap);
  }
  //[2]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("B"),
        NodeFactory().CreateFromStrings("A"),
        NodeFactory().CreateFromStrings("C")
      };

    Edges edges
      =
      {

      };

    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    assert(conceptmap.IsValid());
    v.push_back(conceptmap);
  }
  //[3]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("B"),
        NodeFactory().CreateFromStrings("C"),
        NodeFactory().CreateFromStrings("A")
      };

    Edges edges
      =
      {

      };
    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    v.push_back(conceptmap);
  }

  //[4]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("C"),
        NodeFactory().CreateFromStrings("A"),
        NodeFactory().CreateFromStrings("B")
      };

    Edges edges
      =
      {

      };

    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    assert(conceptmap.IsValid());
    v.push_back(conceptmap);
  }

  //[5]
  {
    Nodes nodes
      =
      {
        CenterNodeFactory().CreateFromStrings("X"),
        NodeFactory().CreateFromStrings("C"),
        NodeFactory().CreateFromStrings("B"),
        NodeFactory().CreateFromStrings("A")
      };

    Edges edges
      =
      {

      };

    const Graph conceptmap(
      GraphFactory().Create(nodes,edges));

    assert(conceptmap.IsValid());
    v.push_back(conceptmap);
  }
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
  if (verbose) { TRACE("Create from XML"); }
  {
    const auto conceptmap = f.FromXml("<concept_map><nodes><node><concept><name>X</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>1</is_center_node></node><node><concept><name>C</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node><node><concept><name>B</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node><node><concept><name>A</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept><x>0</x><y>0</y><is_center_node>0</is_center_node></node></nodes><edges></edges></concept_map>");
    assert(conceptmap.IsValid());
  }
  if (verbose) { TRACE("All the complex homomorphous concept maps to be valid"); }
  {
    const int n = f.GetNumberOfComplexHomomorphousTestGraphs();
    for (int i=0; i!=n; ++i) {
      const auto conceptmap = f.GetComplexHomomorphousTestGraph(i);
      assert(conceptmap.IsValid());
    }
  }
  assert(!"Green");
  GraphFactory().GetHeteromorphousTestGraph(0);
  if (verbose) { TRACE("All testing concept maps must be valid"); }
  {
    for (const auto& conceptmap: GraphFactory().GetAllTests())
    {
      assert(conceptmap.IsValid());
      for (const auto& edge: conceptmap.GetEdges())
      {
        assert(edge.GetTo());
        assert(edge.GetFrom());
        const auto nodes = conceptmap.GetNodes();
        assert(
          std::count(
            begin(nodes),end(nodes),*edge.GetTo()
          ) == 1
        );
        assert(
          std::count(
            begin(nodes),end(nodes),*edge.GetFrom()
          ) == 1
        );
      }
    }
  }
}
#endif // NDEBUG
