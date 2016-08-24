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
#include "conceptmapedgefactory.h"

#include <cassert>
#include <boost/lexical_cast.hpp>

#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"

#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::EdgeFactory::EdgeFactory() noexcept
{

}


ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);

  const double x{(from.GetX() + to.GetX()) / 2.0};
  const double y{(from.GetY() + to.GetY()) / 2.0};
  const auto concept = ConceptFactory().Create();
  const Node node(concept,x,y);
  Edge p(
    node
  );
  return p;
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& node
) const noexcept
{
  Edge p(node);
  return p;
}

int ribi::cmap::EdgeFactory::GetNumberOfTests() const noexcept
{
  return ConceptFactory().GetNumberOfTests();
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::GetTest(
  const int index
) const noexcept
{
  //Nodes may be similar, but not the same
  const auto v = GetTests();
  assert(index >= 0);
  assert(index < static_cast<int>(v.size()));
  return v[index];
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::GetNastyTest(
  const int index
) const noexcept
{
  const auto v = GetNastyTests();
  assert(index >= 0);
  assert(index < static_cast<int>(v.size()));
  return v[index];
}

std::vector<ribi::cmap::Edge> ribi::cmap::EdgeFactory::GetNastyTests() const noexcept
{
  std::vector<Edge> result;

  for (const auto node: NodeFactory().GetNastyTests())
  {
    Edge edge(node);
    edge.SetHeadArrow(true);
    result.emplace_back(edge);
  }
  return result;
}


std::vector<ribi::cmap::Edge> ribi::cmap::EdgeFactory::GetTests() const noexcept
{
  const int n{NodeFactory().GetNumberOfTests()};
  std::vector<Edge> result;

  for(int i{0}; i!=n; ++i)
  {
    {
      const auto node = NodeFactory().GetTest(i);
      Edge edge(node);
      edge.SetHeadArrow(true);
      result.emplace_back(edge);
    }
    /*
    {
      const EdgePtr edge(new Edge(concept,0,2,true));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,0,3,true));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,1,2,false));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,1,3,false));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,2,3,false));
      result.push_back(edge);
    }
    */
  }
  assert(GetNumberOfTests() == static_cast<int>(result.size()));
  return result;
}


