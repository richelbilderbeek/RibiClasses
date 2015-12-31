//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2015 Richel Bilderbeek

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
#include "conceptmapexamplefactory.h"

#include <cassert>

#include "conceptmapcompetencies.h"
#include "conceptmaphelper.h"
#include "counter.h"
#include "conceptmapexample.h"
#include "conceptmapregex.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::ExampleFactory::ExampleFactory() noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::Example ribi::cmap::ExampleFactory::GetTest(const int i) const noexcept
{
  assert(i < GetNumberOfTests());
  return GetTests()[i];
}

std::vector<ribi::cmap::Example> ribi::cmap::ExampleFactory::GetNastyTests() const noexcept
{
  return
  {
    Example(" Test example 0",Competency::profession,true,false,false),
    Example("Test>example 1",Competency::organisations,false,true,false),
    Example("Test example<2",Competency::social_surroundings,false,false,true)
  };
}

std::vector<ribi::cmap::Example> ribi::cmap::ExampleFactory::GetTests() const noexcept
{
  return
  {
    Example("Test example 0",Competency::profession,true,false,false),
    Example("Test example 1",Competency::organisations,false,true,false),
    Example("Test example 2",Competency::social_surroundings,false,false,true),
    Example("Test example 3",Competency::target_audience,true,true,false),
    Example("Test example 4",Competency::ti_knowledge,false,true,true),
    Example("Test example 5",Competency::prof_growth,true,false,true),
    Example("Test example 6",Competency::misc,true,true,true),
    Example("",Competency::uninitialized,true,false,true)
  };
}

#ifndef NDEBUG
void ribi::cmap::ExampleFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Example("Test example 0",Competency::profession,true,false,false);
    Counter();
    ExampleFactory().GetTest(0);
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
}
#endif // NDEBUG
