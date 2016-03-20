#include "conceptmapexamplesfactory.h"

#include <boost/test/unit_test.hpp>
#include "conceptmapexamples.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_factory_test)
{
  using namespace ribi::cmap;
  const bool verbose{false};
  const ribi::cmap::ExamplesFactory f;
  if (verbose) { TRACE("Create must return a valid Examples"); }
  if (verbose) { TRACE("Examples -> XML -> Examples "); }
  {
    const auto examples = f.GetTest(2);
    const auto xml = ToXml(examples);
    const auto new_examples = XmlToExamples(xml);
    const auto new_xml = ToXml(new_examples);
    BOOST_CHECK(xml == new_xml);
  }
}

