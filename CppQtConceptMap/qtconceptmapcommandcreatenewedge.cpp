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
#include "qtconceptmapcommandcreatenewedge.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"

#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "trace.h"

#include <boost/graph/isomorphism.hpp>

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  ConceptMap& conceptmap
)
 :
    m_conceptmap(conceptmap),
    m_before{conceptmap},
    m_after{CreateNewEdgeFromTwoSelectedNodes(conceptmap)}
{
  assert(!boost::isomorphism(m_before,m_after));
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::redo()
{
  m_conceptmap = m_after;
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::undo()
{
  m_conceptmap = m_before;
}

ribi::cmap::ConceptMap ribi::cmap::CreateNewEdgeFromTwoSelectedNodes(ConceptMap g)
{
  std::vector<VertexDescriptor> v;
  const auto vip = vertices(g);

  std::copy_if(
    vip.first,
    vip.second,
    std::back_inserter(v),
    [g](const VertexDescriptor& vd)
    {
      const auto is_selected_map
        = get(boost::vertex_is_selected, g);
      return get(is_selected_map, vd);
    }
  );
  if (v.size() == 2)
  {
    std::stringstream s;
    s << __func__ << ": need two selected nodes to create an edge between";
    throw std::logic_error(s.str());
  }

  const auto aer = boost::add_edge(v[0],v[1],g);
  if (!aer.second)
  {
    std::stringstream s;
    s << __func__ << ": edge insertion failed";
    throw std::logic_error(s.str());
  }
  return g;
}

