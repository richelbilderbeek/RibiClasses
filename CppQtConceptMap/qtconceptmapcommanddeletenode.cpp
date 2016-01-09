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
#include "qtconceptmapcommanddeletenode.h"

#include <cassert>
#include <iostream>

#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene,
  QtTool * const tool_item
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after{conceptmap},
    m_conceptmap_before{conceptmap},
    m_qtnode{nullptr},
    m_scene(scene),
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item->GetBuddyItem()}
{
  setText("delete selected nodes and edges");
  /*

  //Add the vertex to the concept map
  VertexDescriptor vd = boost::add_vertex(m_conceptmap_after);

  //Create the node
  Node node;
  node.SetX(m_x);
  node.SetY(m_y);

  //Set the node
  {
    const auto pmap = get(boost::vertex_custom_type, m_conceptmap_after);
    put(pmap, vd, node);
  }
  //Additively select node
  {
    const auto pmap = get(boost::vertex_is_selected, m_conceptmap_after);
    put(pmap, vd, true);
  }

  //Modify the QGraphicsScene
  m_qtnode = new QtNode(node);
  assert(m_qtnode);
  assert(m_qtnode->GetCenterX() == node.GetX());
  assert(m_qtnode->GetCenterY() == node.GetY());
  */
}

void ribi::cmap::CommandDeleteSelected::redo()
{
  /*
  m_conceptmap = m_conceptmap_after;
  m_scene->addItem(m_qtnode);
  m_qtnode->SetSelected(true); //Additively select node
  m_qtnode->setFocus();
  m_tool_item->SetBuddyItem(m_qtnode);
  */
}

void ribi::cmap::CommandDeleteSelected::undo()
{
  /*
  m_conceptmap = m_conceptmap_before;
  m_scene->removeItem(m_qtnode);
  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);
  */
}
