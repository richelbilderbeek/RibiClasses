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
#include "qtconceptmapqtnode.h"
#include "trace.h"
#include "count_vertices_with_selectedness.h"
#include "add_edge_between_two_selected_vertices.h"
#include <boost/graph/isomorphism.hpp>
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::CommandCreateNewEdgeBetweenTwoSelectedNodes(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene,
  QtTool * const tool_item
) : m_conceptmap(conceptmap),
    m_after{conceptmap},
    m_before{conceptmap},
    m_scene{scene},
    m_tool_item{tool_item},
    m_qtedge{nullptr}
{
  this->setText("Create new edge");

  //Update concept map
  assert(count_vertices_with_selectedness(true, m_after) == 2);
  const auto ed = add_edge_between_two_selected_vertices(m_after);
  assert(!boost::isomorphism(m_before,m_after));

  const VertexDescriptor vd_from = boost::source(ed, m_after);
  const VertexDescriptor vd_to = boost::target(ed, m_after);
  assert(vd_from != vd_to);
  const auto vertex_map = get(boost::vertex_custom_type, m_after);
  const Node from = get(vertex_map, vd_from);
  const Node to = get(vertex_map, vd_to);
  assert(from.GetId() != to.GetId());
  QtNode * const qtfrom = FindQtNode(from.GetId(), scene);
  QtNode * const qtto = FindQtNode(to.GetId(), scene);
  const auto edge_map = get(boost::edge_custom_type, m_after);
  const Edge edge = get(edge_map, ed);
  m_qtedge = new QtEdge(edge, qtfrom,qtto);

}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::redo()
{
  m_conceptmap = m_after;
  m_scene->addItem(m_qtedge);
}

void ribi::cmap::CommandCreateNewEdgeBetweenTwoSelectedNodes::undo()
{
  m_conceptmap = m_before;
  m_scene->removeItem(m_qtedge);
}
