#include "conceptmapregex.h"

#include <boost/test/unit_test.hpp>
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_regex_test)
{
  const ::ribi::cmap::Regex r;
  const bool verbose{false};
  if (verbose) { TRACE("GetRegexExample"); }
  {
    const std::string s = "<concept><name>Concept with examples</name>"
      "<example>Example 1</example><example>Example 2</example>"
      "<example>Example 3</example></concept>"
    ;
    const std::vector<std::string> expected {
      "<example>Example 1</example>",
      "<example>Example 2</example>",
      "<example>Example 3</example>"
    };
    const std::string regex_str = r.GetRegexExample();
    BOOST_CHECK(r.GetRegexMatches(s,regex_str) == expected);
  }
}
