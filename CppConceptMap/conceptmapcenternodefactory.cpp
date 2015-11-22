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
#include "conceptmapcenternodefactory.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>


#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "counter.h"
#include "ribi_regex.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::CenterNodeFactory::CenterNodeFactory()
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::Create() const noexcept
{
  Node node(
    ConceptFactory().Create(),
    true, //is_center_node
    0.0, //x
    0.0  //y
  );
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::Create(
  const Concept& concept,
  const double x,
  const double y
) const noexcept
{
  Node node(
    concept,
    true, //is_center_node
    x,
    y
  );
  assert(concept == node.GetConcept());
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.IsCenterNode());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  Node node(
    ConceptFactory().Create(name,examples),
    true,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  assert(node.IsCenterNode());
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::FromXml(
  const std::string& s
) const
{
  Node node = NodeFactory().FromXml(s);
  assert(ToXml(node) == s);
  assert(node.IsCenterNode());
  return node;
}

#ifndef NDEBUG
void ribi::cmap::CenterNodeFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    NodeFactory();
    ConceptFactory();
    Counter();
    ExampleFactory();
    ExamplesFactory();
    ::ribi::Regex();
    ::ribi::cmap::TestHelperFunctions();
    ::ribi::cmap::Regex();
  }
  const TestTimer test_timer{__func__,__FILE__,0.1};
  //Create from Concept constructor
  {
    const auto concept = ConceptFactory().GetTest(0);
    const double x{0.1};
    const double y{2.3};
    const auto node = CenterNodeFactory().Create(concept,x,y);
    assert(node.IsCenterNode());
    assert(concept == node.GetConcept());
    assert(node.GetX() == x);
    assert(node.GetY() == y);
  }
  {
    const std::string xml = "<node><concept><name>Concept without examples</name><examples></examples><concept_is_complex>0</concept_is_complex><complexity>0</complexity><concreteness>1</concreteness><specificity>2</specificity></concept><x>0.1</x><y>2.3</y><is_center_node>1</is_center_node></node>";
    const auto node = CenterNodeFactory().FromXml(xml);
    assert(node.IsCenterNode());
  }
}
#endif
