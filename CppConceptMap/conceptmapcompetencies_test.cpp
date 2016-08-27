#include "conceptmapcompetencies.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_concept_map_competencies_test)
{
  using namespace ribi::cmap;
  static_assert(static_cast<int>(Competency::uninitialized) == 0,
    "Start the uninitialized value at zero, "
    "so that GetAllCompetencies can check against n_competencies"
  );
  Competencies c;
  {
    const std::vector<Competency> v = c.GetAllCompetencies();
    const std::size_t sz = v.size();
    for (std::size_t i=0; i!=sz; ++i)
    {
      BOOST_CHECK(i < v.size());
      const Competency t = v[i];
      const std::string s = c.ToStr(t);
      BOOST_CHECK(!s.empty());
      const Competency u = c.ToType(s);
      BOOST_CHECK(u == t);
    }
    for (std::size_t i=0; i!=sz; ++i)
    {
      BOOST_CHECK(i < v.size());
      const Competency t = v[i];
      const std::string s = c.ToStrDutch(t);
      BOOST_CHECK(!s.empty());
      const Competency u = c.ToTypeFromDutch(s);
      BOOST_CHECK(u == t);
    }
  }
  //ToIndex
  {
    BOOST_CHECK(c.ToIndex(Competency::uninitialized) == 0);
  }
}
