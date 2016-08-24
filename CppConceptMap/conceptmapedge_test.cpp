#include "conceptmapedge.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapedgefactory.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_copy_constructor)
{
  using namespace ribi::cmap;
  const auto edge1 = EdgeFactory().GetTest(1);
  const auto edge2(edge1);
  BOOST_CHECK_EQUAL(edge1, edge2);
  BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
  BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_assignment_operator)
{
  using namespace ribi::cmap;
  const auto edge1 = EdgeFactory().GetTest(0);
  auto edge2 = EdgeFactory().GetTest(1);
  BOOST_CHECK_NE(edge1, edge2);
  edge2 = edge1;
  BOOST_CHECK_EQUAL(edge1, edge2);

}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_operator_is_equal)
{
  using namespace ribi::cmap;
  {
    const auto edge1 = EdgeFactory().GetTest(0);
    const auto edge2 = EdgeFactory().GetTest(0);
    BOOST_CHECK_EQUAL(edge1, edge2);
  }
  {
    const auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK_EQUAL(edge1.GetNode().GetX(), edge2.GetNode().GetX());
    BOOST_CHECK_EQUAL(edge1, edge2);
  }
  //Toggle head arrow
  {
    auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK_EQUAL(edge1, edge2);
    edge1.SetHeadArrow(!edge1.HasHeadArrow());
    BOOST_CHECK_NE(edge1, edge2);
    edge1.SetHeadArrow(!edge1.HasHeadArrow());
    BOOST_CHECK_EQUAL(edge1, edge2);
  }
  //Toggle tail arrow
  {
    auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2 = EdgeFactory().GetTest(1);
    BOOST_CHECK_EQUAL(edge1, edge2);
    edge1.SetTailArrow(!edge1.HasTailArrow());
    BOOST_CHECK_NE(edge1, edge2);
    edge1.SetTailArrow(!edge1.HasTailArrow());
    BOOST_CHECK_EQUAL(edge1, edge2);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_operator_is_not_equal)
{
  using namespace ribi::cmap;
  const auto edge1 = EdgeFactory().GetTest(0);
  const auto edge2 = EdgeFactory().GetTest(1);
  BOOST_CHECK_NE(edge1, edge2);
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_to_xml_to_edge_easy)
{
  //Can all edges be converted to XML and back?
  using namespace ribi::cmap;
  const auto edges = EdgeFactory().GetTests();
  for (const auto edge_before: edges)
  {
    const std::string s{ToXml(edge_before)};
    const Edge edge_after{XmlToEdge(s)};
    BOOST_CHECK_EQUAL(ToXml(edge_before), ToXml(edge_after));
    BOOST_CHECK_EQUAL(edge_before, edge_after);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_to_xml_to_edge_nasty)
{
  //Can all edges be converted to XML and back?
  using namespace ribi::cmap;
  const auto edges = EdgeFactory().GetNastyTests();
  for (const auto edge_before: edges)
  {
    const std::string s{ToXml(edge_before)};
    const Edge edge_after{XmlToEdge(s)};
    BOOST_CHECK_EQUAL(ToXml(edge_before), ToXml(edge_after));
    BOOST_CHECK_EQUAL(edge_before, edge_after);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operator)
{
  using namespace ribi::cmap;
  const auto edge1 = EdgeFactory().GetTest(0);
  std::stringstream s;
  s << edge1;
  Edge edge2;
  s >> edge2;
  BOOST_CHECK_EQUAL(edge1, edge2);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operators)
{
  using namespace ribi::cmap;
  const Edge e = EdgeFactory().GetTest(1);
  const Edge f = EdgeFactory().GetTest(2);
  std::stringstream s;
  s << e << f;
  Edge g;
  Edge h;
  s >> g >> h;
  BOOST_CHECK_EQUAL(e, g);
  BOOST_CHECK_EQUAL(f, h);
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operator_nasty)
{
  using namespace ribi::cmap;
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Edge f;
    BOOST_CHECK_NE(e, f);
    s >> f;
    BOOST_CHECK_EQUAL(e, f);
  }
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_edge_stream_operators_nasty)
{
  using namespace ribi::cmap;
  for (const Edge e: EdgeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << e;
    Edge g;
    Edge h;
    s >> g >> h;
    BOOST_CHECK_EQUAL(e, g);
    BOOST_CHECK_EQUAL(e, h);
  }
}
