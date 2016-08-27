//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapconcept.h"

#include "conceptmaphelper.h"
#include "conceptmapconceptfactory.h"
#include "counter.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_concept_map_concept_test)
{
  using namespace ribi::cmap;
  const bool verbose{false};

  if (verbose) { TRACE("Copy constructor"); }
  {
    const Concept c = ConceptFactory().Create();
    Concept d(c);
    BOOST_CHECK(d == c);
  }
  if (verbose) { TRACE("Assignment operator"); }
  {
    const Concept c = ConceptFactory().GetTest(1);
    const Concept d = ConceptFactory().GetTest(2);
    BOOST_CHECK(c != d);
    Concept e(c);
    BOOST_CHECK(e == c);
    BOOST_CHECK(e != d);
    e = d;
    BOOST_CHECK(e != c);
    BOOST_CHECK(e == d);
  }
  if (verbose) { TRACE("Test operator== and operator!="); }
  {
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    BOOST_CHECK(sz > 0);
    for (int i=0; i!=sz; ++i)
    {
      const auto tmp_a = ConceptFactory().GetTests();
      const auto tmp_b = ConceptFactory().GetTests();
      const Concept a = tmp_a.at(i);
            Concept b = tmp_b.at(i);
      BOOST_CHECK(a == a);
      BOOST_CHECK(b == a);
      BOOST_CHECK(a == b);
      BOOST_CHECK(b == b);
      for (int j=0; j!=sz; ++j)
      {
        BOOST_CHECK(j < static_cast<int>(ConceptFactory().GetTests().size()));
        const Concept c = ConceptFactory().GetTests().at(j);
              Concept d = ConceptFactory().GetTests().at(j);
        BOOST_CHECK(c == c);
        BOOST_CHECK(d == c);
        BOOST_CHECK(c == d);
        BOOST_CHECK(d == d);
        if (i==j)
        {
          BOOST_CHECK(a == c); BOOST_CHECK(a == d);
          BOOST_CHECK(b == c); BOOST_CHECK(b == d);
          BOOST_CHECK(c == a); BOOST_CHECK(c == b);
          BOOST_CHECK(d == a); BOOST_CHECK(d == b);
        }
        else
        {
          BOOST_CHECK(a != c); BOOST_CHECK(a != d);
          BOOST_CHECK(b != c); BOOST_CHECK(b != d);
          BOOST_CHECK(c != a); BOOST_CHECK(c != b);
          BOOST_CHECK(d != a); BOOST_CHECK(d != b);
        }
      }
    }
  }
  if (verbose) { TRACE("Test operator<"); }
  {
    if (verbose) { TRACE("operator< must order by name"); }
    {
      const Concept a = ConceptFactory().Create("1");
            Concept b = ConceptFactory().Create("1");
      const Concept c = ConceptFactory().Create("2");
            Concept d = ConceptFactory().Create("2");
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    if (verbose) { TRACE("operator< must order by examples' size, sizes 0 versus 1"); }
    {
      const Concept a = ConceptFactory().Create("1");
            Concept b = ConceptFactory().Create("1");
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc} } );
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc} } );
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    if (verbose) { TRACE("operator< must order by examples' size, sizes 1 versus 2"); }
    {
      const Concept a = ConceptFactory().Create("1", { {"2",Competency::misc} } );
            Concept b = ConceptFactory().Create("1", { {"2",Competency::misc} } );
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
    if (verbose) { TRACE("Check correct ordering for equal examples' size, lexicographically in the 2nd text"); }
    {
      const Concept a = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
            Concept b = ConceptFactory().Create("1", { {"2",Competency::misc},{"3",Competency::misc} } ); //!OCLINT
      const Concept c = ConceptFactory().Create("1", { {"2",Competency::misc},{"4",Competency::misc} } ); //!OCLINT
            Concept d = ConceptFactory().Create("1", { {"2",Competency::misc},{"4",Competency::misc} } ); //!OCLINT
      BOOST_CHECK(a < c); BOOST_CHECK(a < d);
      BOOST_CHECK(b < c); BOOST_CHECK(b < d);
    }
  }
  {
    const std::string xml = "<concept><name>TEST</name><examples></examples><concept_is_complex>1</concept_is_complex><complexity>-1</complexity><concreteness>-1</concreteness><specificity>-1</specificity></concept>"; //!OCLINT
    const auto concept = XmlToConcept(xml);
    BOOST_CHECK(concept.GetName() == "TEST");
  }
  //Single stream
  {
    const Concept e = ConceptFactory().GetTest(1);
    const Concept f = ConceptFactory().GetTest(2);
    std::stringstream s;
    s << e << " " << f;
    Concept g;
    Concept h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(f == h);
  }
  //Nasty examples
  for (const Concept e: ConceptFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Concept f;
    BOOST_CHECK(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    BOOST_CHECK(e == f);
  }
  //Nasty examples
  for (const Concept e: ConceptFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Concept g;
    Concept h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(e == h);
  }
}
