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
#include "conceptmapconceptfactory.h"

#include "conceptmapcompetency.h"
#include "conceptmapconcept.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapregex.h"

#include "counter.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic push

ribi::cmap::ConceptFactory::ConceptFactory() noexcept
{
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::Create(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& v,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity
) const noexcept
{
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);

  std::vector<Example> w;
  std::transform(v.begin(),v.end(),std::back_inserter(w),
    [](const std::pair<std::string,Competency>& p)
    {
      Example q(
        p.first,
        p.second
      );
      return q;
    }
  );

  const Examples examples(w);

  Concept concept{
    name,
    examples,
    is_complex,
    rating_complexity,
    rating_concreteness,
    rating_specificity
  };
  return concept;
}

std::vector<ribi::cmap::Concept> ribi::cmap::ConceptFactory::GetNastyTests() const noexcept
{
  std::vector<Concept> v;
  {
    const Examples examples;
    const Concept p(" Concept with (n < 1) examples", examples, false, 0, 1, 2);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept (n > 0 && n < 2) examples ", { { " Only one of (n > 2) examples", cmap::Competency::profession } }, 1, 2, 0);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create(
      " Very>long<multi-line concept with four Roman examples that also each span multiple lines, that is, eighty characters",
      {
        { "Example I/IV,spanning (n > 1) lines  (that is, having at least eight characters) and is rated as cmap::Competency::misc ", cmap::Competency::misc },
        { " Example II/IV,spanning (n >= (256 >> 9) lines (that is, having at least eight characters) and is rated as cmap::Competency::uninitialized", cmap::Competency::uninitialized },
        { "Example III/IV, spanning <<<multiple>>> lines (that is, having at least eight characters) and is rated as cmap::Competency::profession ", cmap::Competency::profession },
        { " Example III/IV, spanning multiple lines (n >= (1 << 1)) (that is, having at least eight characters) and is rated as cmap::Competency::social_surroundings", cmap::Competency::social_surroundings }
      }, 1, 2, 0
    );
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  return v;
}

int ribi::cmap::ConceptFactory::GetNumberOfTests() const noexcept
{
  const int sz{3};
  assert(GetTests().size() == sz);
  return sz;
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::GetTest(const int i) const noexcept
{
  const auto v = GetTests();
  assert(i >= 0);
  assert(i < static_cast<int>(v.size()));
  return v[i];
}

std::array<ribi::cmap::Concept, 3> ribi::cmap::ConceptFactory::GetTests() const noexcept
{
  const int sz{3};
  std::array<Concept, 3> v;
  assert(sz == static_cast<int>(v.size()));
  int i{0};
  {
    const Examples examples;
    const Concept p("Concept without examples", examples, false, 0, 1, 2);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    assert(i < sz);
    v[i++] = p;
  }
  {
    const Concept p = Create("Concept with one example", { { "Only example", cmap::Competency::profession } }, 1, 2, 0);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    assert(i < sz);
    v[i++] = p;
  }
  /*
  {
    const Concept p = Create("Concept with two examples", { { "First example", cmap::Competency::organisations }, { "Second example", cmap::Competency::social_surroundings } }, 2, 0, 1);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept with three examples", { { "Example 1 of 3", cmap::Competency::target_audience }, { "Example 2 of 3", cmap::Competency::ti_knowledge }, { "Example 3 of 3", cmap::Competency::prof_growth } }, 0, 1, 2);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept with four Roman examples", { { "Example I/IV", cmap::Competency::misc }, { "Example II/IV", cmap::Competency::uninitialized }, { "Example III/IV", cmap::Competency::profession }, { "Example III/IV", cmap::Competency::social_surroundings } }, 1, 2, 0);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  */
  {
    const Concept p = Create(
      "Very long multi-line concept with four Roman examples that also each span multiple lines, that is, eighty characters",
      {
        { "Example I/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::misc", cmap::Competency::misc },
        { "Example II/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::uninitialized", cmap::Competency::uninitialized },
        { "Example III/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::profession", cmap::Competency::profession },
        { "Example III/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::social_surroundings", cmap::Competency::social_surroundings }
      }, 1, 2, 0
    );
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    assert(i < sz);
    v[i++] = p;
  }
  return v;
}

