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

#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::CenterNodeFactory::CenterNodeFactory()
{
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

int ribi::cmap::CenterNodeFactory::GetNumberOfTests() const noexcept
{
  return NodeFactory().GetNumberOfTests();
}

int ribi::cmap::CenterNodeFactory::GetNumberOfNastyTests() const noexcept
{
  return NodeFactory().GetNumberOfNastyTests();
}

std::vector<ribi::cmap::Node> ribi::cmap::CenterNodeFactory::GetTests() const noexcept
{
  auto v = NodeFactory().GetTests();
  for (auto& node: v) { node.SetIsCenterNode(true); }
  return v;
}

std::vector<ribi::cmap::Node> ribi::cmap::CenterNodeFactory::GetNastyTests() const noexcept
{
  auto v = NodeFactory().GetNastyTests();
  for (auto& node: v) { node.SetIsCenterNode(true); }
  return v;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetTest(test_index);
  node.SetIsCenterNode(true);
  return node;
}

ribi::cmap::Node ribi::cmap::CenterNodeFactory::GetNastyTest(const int test_index) const noexcept
{
  auto node = NodeFactory().GetNastyTest(test_index);
  node.SetIsCenterNode(true);
  return node;
}

