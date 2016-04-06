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
#include "conceptmapexamplesfactory.h"

#include <cassert>

#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapregex.h"

#include "trace.h"
#pragma GCC diagnostic pop

ribi::cmap::ExamplesFactory::ExamplesFactory() noexcept
{

}

ribi::cmap::Examples ribi::cmap::ExamplesFactory::Create(
  const std::vector<std::pair<std::string,Competency> >& v) const noexcept
{
  std::vector<Example> w;
  std::transform(v.begin(),v.end(),std::back_inserter(w),
    [](const std::pair<std::string,Competency>& p)
    {
      const Example q
       (p.first,p.second);
      return q;
    }
  );
  Examples q(w);
  return q;
}

ribi::cmap::Examples ribi::cmap::ExamplesFactory::GetTest(const int i) const noexcept
{
  assert(i >= 0);
  assert(i < GetNumberOfTests());
  return GetTests()[i];
}

std::vector<ribi::cmap::Examples> ribi::cmap::ExamplesFactory::GetNastyTests() const noexcept
{
  std::vector<Examples> v;
  v.push_back(Examples(ExampleFactory().GetNastyTests()));
  return v;
}

std::vector<ribi::cmap::Examples> ribi::cmap::ExamplesFactory::GetTests() const noexcept
{
  const std::vector<std::vector<int> > is = { {0}, {1}, {0,1,2,3}, {} };
  std::vector<Examples> v;
  std::transform(is.begin(),is.end(),std::back_inserter(v),
    [this](const std::vector<int>& js)
    {
      std::vector<Example> w;
      std::transform(js.begin(),js.end(),std::back_inserter(w),
        [](const int& j)
        {
          const Example p = ExampleFactory().GetTest(j);
          return p;
        }
      );
      const Examples q(w);
      return q;
    }
  );

  return v;

}

