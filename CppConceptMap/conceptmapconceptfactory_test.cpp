#include "conceptmapconceptfactory.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapconcept.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_test)
{
  using namespace ribi::cmap;
  const bool verbose{false};
  if (verbose) { TRACE("Concept -> XML -> Concept "); }
  {
    const auto concept = ConceptFactory().GetTest(2);
    const auto xml = ToXml(concept);
    const auto new_concept = XmlToConcept(xml);
    const auto new_xml = ToXml(new_concept);
    BOOST_CHECK(xml == new_xml);
  }
}
