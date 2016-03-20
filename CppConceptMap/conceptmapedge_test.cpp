#include "conceptmapedge.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapedgefactory.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_test)
{
  using namespace ribi::cmap;
  const bool verbose{false};
  const std::vector<Node> nodes{NodeFactory().GetTest(0),NodeFactory().GetTest(1)};
  if (verbose) { TRACE("Copy constructor"); }
  {
    const auto edge1 = EdgeFactory().GetTest(1);
    const auto edge2(edge1);
    assert(edge1 == edge2);
    assert(edge1.GetNode().GetX() == edge2.GetNode().GetX());
    assert(edge1.GetNode().GetX() == edge2.GetNode().GetX());
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
    assert(edge1.GetNode().GetX() == edge2.GetNode().GetX());
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
    if (ToXml(edge_before) != ToXml(edge_after))
    {
      TRACE(ToXml(edge_before));
      TRACE(ToXml(edge_after ));
      TRACE(edge_before.GetNode().GetX());
      TRACE(edge_before.GetNode().GetY());
      TRACE(edge_after.GetNode().GetX());
      TRACE(edge_after.GetNode().GetY());
    }
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
