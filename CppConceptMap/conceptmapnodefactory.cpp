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
#include "conceptmapnodefactory.h"


#include "conceptmapcenternodefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapregex.h"
#include "container.h"

#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::NodeFactory::NodeFactory()
{

}

ribi::cmap::Node ribi::cmap::NodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  const bool is_center_node{false};
  Node node(
    ConceptFactory().Create(name,examples),
    is_center_node,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

int ribi::cmap::NodeFactory::GetNumberOfTests() const noexcept
{
  return static_cast<int>(GetTests().size());
}

int ribi::cmap::NodeFactory::GetNumberOfNastyTests() const noexcept
{
  return static_cast<int>(GetNastyTests().size());
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetTest(const int i) const noexcept
{
  const auto tests = GetTests();
  assert(i >= 0);
  assert(i < static_cast<int>(tests.size()));
  return tests[i];
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty0() const noexcept
{
  return Node(
    ConceptFactory().GetNasty0(),
    false,
    1.2,
    3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty1() const noexcept
{
  return Node(
    ConceptFactory().GetNasty1(),
    false,
    -1.2,
    3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNasty2() const noexcept
{
  return Node(
    ConceptFactory().GetNasty2(),
    false,
    -1.2,
    -3.4
  );
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetNastyTest(const int i) const noexcept
{
  const auto tests = GetNastyTests();
  assert(i >= 0);
  assert(i < static_cast<int>(tests.size()));
  return tests[i];
}

std::vector<ribi::cmap::Node> ribi::cmap::NodeFactory::GetNastyTests() const noexcept
{
  std::vector<Node> nodes;
  const auto v = ConceptFactory().GetNastyTests();
  std::transform(v.begin(),v.end(),std::back_inserter(nodes),
    [](const Concept& c)
    {
      const double x{1.2};
      const double y{3.4};
      const Node p{c,false,x,y};
      return p;
    }
  );
  return nodes;
}

std::vector<ribi::cmap::Node> ribi::cmap::NodeFactory::GetTests() const noexcept
{
  std::vector<Node> nodes;
  const auto v = ConceptFactory().GetTests();
  std::transform(v.begin(),v.end(),std::back_inserter(nodes),
    [](const Concept& c)
    {
      const double x{1.2};
      const double y{3.4};
      const Node p{c,false,x,y};
      return p;
    }
  );
  return nodes;
}

