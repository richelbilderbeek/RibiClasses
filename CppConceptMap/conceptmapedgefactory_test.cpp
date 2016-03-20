#include "conceptmapedgefactory.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapedge.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_edge_factory_test)
{
  const ribi::cmap::EdgeFactory f;
  BOOST_CHECK(!f.GetNastyTests().empty());
}
