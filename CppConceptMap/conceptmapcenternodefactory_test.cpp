#include "conceptmapcenternodefactory.h"

#include <boost/test/unit_test.hpp>
#include "conceptmapconceptfactory.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_center_node_factory_must_construct_center_nodes_from_default)
{
  using namespace ribi::cmap;
  const auto node = CenterNodeFactory().Create();
  BOOST_CHECK(node.IsCenterNode());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_center_node_factory_must_construct_center_nodes_from_concept)
{
  using namespace ribi::cmap;
  const auto concept = ConceptFactory().GetTest(0);
  const double x{0.1};
  const double y{2.3};
  const auto node = CenterNodeFactory().Create(concept,x,y);
  BOOST_CHECK(node.IsCenterNode());
  BOOST_CHECK(concept == node.GetConcept());
  BOOST_CHECK(node.GetX() == x);
  BOOST_CHECK(node.GetY() == y);
}


BOOST_AUTO_TEST_CASE(ribi_concept_map_center_node_factory_must_construct_center_nodes_from_xml)
{
  using namespace ribi::cmap;
  const std::string xml = "<node><concept>"
    "<name>Concept without examples</name>"
    "<examples></examples><concept_is_complex>0</concept_is_complex>"
    "<complexity>0</complexity><concreteness>1</concreteness>"
    "<specificity>2</specificity></concept><x>0.1</x><y>2.3</y>"
    "<is_center_node>1</is_center_node></node>"
  ;
  const auto node = XmlToNode(xml);
  BOOST_CHECK(node.IsCenterNode());
}

BOOST_AUTO_TEST_CASE(ribi_concept_map_center_node_factory_test_sizes_must_match)
{
  using namespace ribi::cmap;
  const CenterNodeFactory f;
  {
    const auto tests_sz = f.GetTests().size();
    const auto n_tests = f.GetNumberOfTests();
    BOOST_CHECK_EQUAL(n_tests, static_cast<int>(tests_sz));
  }
  {
    const auto nasty_tests_sz = f.GetNastyTests().size();
    const auto n_nasty_tests = f.GetNumberOfNastyTests();
    BOOST_CHECK_EQUAL(n_nasty_tests, static_cast<int>(nasty_tests_sz));
  }
}

