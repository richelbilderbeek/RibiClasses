#include "conceptmapconceptfactory.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapconcept.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_test)
{
  BOOST_CHECK_EQUAL(
    ribi::cmap::ConceptFactory().GetTests().size(),
    ribi::cmap::ConceptFactory().GetNumberOfTests()
  );
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_factory_concept_to_xml_to_concept)
{
  const auto concept = ribi::cmap::ConceptFactory().GetTest(2);
  const auto xml = ribi::cmap::ToXml(concept);
  const auto new_concept = ribi::cmap::XmlToConcept(xml);
  const auto new_xml = ribi::cmap::ToXml(new_concept);
  BOOST_CHECK(xml == new_xml);
}
