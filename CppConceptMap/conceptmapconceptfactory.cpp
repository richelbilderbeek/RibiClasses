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

ribi::cmap::Concept ribi::cmap::ConceptFactory::Get0() const noexcept
{
  const Examples examples;
  return Concept("Concept without examples", examples, false, 0, 1, 2);
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::Get1() const noexcept
{
  return Create(
    "Concept with one example",
    {
      { "Only example", cmap::Competency::profession }
    },
    1,
    2,
    0
  );
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::Get2() const noexcept
{
  return Create(
    "Very long multi-line concept with four Roman examples that "
    "also each span multiple lines, that is, eighty characters",
    {
      {
        "Example I/IV, spanning multiple lines (that is, having at least "
        "eighty characters) and is rated as Competency::misc",
        Competency::misc
      },
      {
        "Example II/IV, spanning multiple lines (that is, having at least "
        "eighty characters) and is rated as Competency::uninitialized",
        Competency::uninitialized
      },
      {
        "Example III/IV, spanning multiple lines (that is, having at least "
        "eighty characters) and is rated as Competency::profession",
        Competency::profession },
      {
        "Example III/IV, spanning multiple lines (that is, having at least "
        "eighty ycharacters) and is rated as "
        "cmap::Competency::social_surroundings",
        Competency::social_surroundings
      }
    }, 1, 2, 0
  );
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::GetNasty0() const noexcept
{
  return Concept(
    " Concept with (n < 1) examples", Examples(), false, 0, 1, 2
  );
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::GetNasty1() const noexcept
{
  return Create(
    "Concept (n > 0 && n < 2) examples ",
    {
      { " Only one of (n > 2) examples", cmap::Competency::profession }
    },
    1,
    2,
    0
  );
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::GetNasty2() const noexcept
{
  return Create(
    " Very>long<multi-line concept with four Roman examples "
    "that also each span multiple lines, that is, eighty characters",
    {
      {
        "Example I/IV,spanning (n > 1) lines  (that is, having at least "
        "eighty characters) and is rated as cmap::Competency::misc ",
        cmap::Competency::misc
      },
      { " Example II/IV,spanning (n >= (256 >> 9) lines (that is, "
        "having at least eighty characters) and is rated as "
        "cmap::Competency::uninitialized",
         cmap::Competency::uninitialized
      },
      {
        "Example III/IV, spanning <<<multiple>>> lines (that is, "
        "having at least eight characters) and is rated as "
        "cmap::Competency::profession ",
        cmap::Competency::profession
      },
      {
        " Example III/IV, spanning multiple lines (n >= (1 << 1)) (that is, "
        "having at least eight characters) and is rated as "
        "cmap::Competency::social_surroundings",
        cmap::Competency::social_surroundings
      }
    },
    1,
    2,
    0
  );
}

std::vector<ribi::cmap::Concept> ribi::cmap::ConceptFactory::GetNastyTests() const noexcept
{
  std::vector<Concept> v;
  v.push_back(GetNasty0());
  v.push_back(GetNasty1());
  v.push_back(GetNasty2());
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

std::vector<ribi::cmap::Concept> ribi::cmap::ConceptFactory::GetTests() const noexcept
{
  std::vector<Concept> v;
  v.reserve(3);
  v.push_back(Get0());
  v.push_back(Get1());
  v.push_back(Get2());
  return v;
}

