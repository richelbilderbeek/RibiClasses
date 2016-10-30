#include "conceptmapexamplefactory.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_concept_map_example_factory_test)
{
  ribi::cmap::ExampleFactory f;
  BOOST_CHECK(!f.GetNastyTests().empty());
}
