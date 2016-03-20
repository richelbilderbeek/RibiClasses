#include "conceptmapfactory.h"

#include <boost/test/unit_test.hpp>
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_map_factory_test)
{
  const bool verbose{false};
  const ribi::cmap::ConceptMapFactory f;
  {
    if (f.GetNumberOfAllTests() != static_cast<int>(f.GetAllTests().size()))
    {
      TRACE(f.GetNumberOfAllTests());
      TRACE(f.GetAllTests().size());
    }
    BOOST_CHECK(f.GetNumberOfAllTests() == static_cast<int>(f.GetAllTests().size()));
  }
  if (verbose) { TRACE("All the testing concept maps to be valid"); }
  {
    const int n{f.GetNumberOfAllTests()};
    for (int i=0; i!=n; ++i) {
      f.GetTest(i);
    }
  }
}
