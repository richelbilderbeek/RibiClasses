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
#include "qtconceptmapcommanddeleteselected.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

ribi::cmap::CommandDeleteSelected::CommandDeleteSelected(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene,
  QtTool * const tool_item
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after(conceptmap),
    m_conceptmap_before(conceptmap),
    m_qtedges_removed{},
    m_qtnodes_removed{},
    m_scene(scene),
    m_selected_before{scene->selectedItems()},
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item->GetBuddyItem()}
{
  setText("delete selected nodes and edges");

  //Count the number of vertices and edges selected
  {
    const auto vip = vertices(m_conceptmap_after);
    const int n_vertices_selected{
      static_cast<int>(
        std::count_if(vip.first, vip.second,
          [this](const VertexDescriptor vd)
          {
            const auto is_selected_map = get(boost::vertex_is_selected, m_conceptmap_after);
            return get(is_selected_map,vd);
          }
        )
      )
    };

    const auto eip = edges(m_conceptmap_after);
    const int n_edges_selected{
      static_cast<int>(
        std::count_if(eip.first, eip.second,
          [this](const EdgeDescriptor vd)
          {
            const auto is_selected_map = get(boost::edge_is_selected, m_conceptmap_after);
            return get(is_selected_map,vd);
          }
        )
      )
    };
    if (n_vertices_selected + n_edges_selected == 0) {
      std::stringstream msg;
      msg << __func__ << ": no edges nor vertices selected, cannot delete zero selected items";
      throw std::logic_error(msg.str());
    }
  }

  //Remove the vertices from the concept map
  while (1)
  {
    const auto vip = vertices(m_conceptmap_after);
    const auto i = std::find_if(vip.first, vip.second,
      [this](const VertexDescriptor vd)
      {
        const auto is_selected_map = get(boost::vertex_is_selected, m_conceptmap_after);
        return get(is_selected_map,vd);
      }
    );
    if (i == vip.second)
    {
      break;
    }
    else
    {
      boost::clear_vertex(*i,m_conceptmap_after);
      boost::remove_vertex(*i,m_conceptmap_after);
    }
  }

  //Remove the edges from the concept map
  while (1)
  {
    const auto eip = edges(m_conceptmap_after);
    const auto i = std::find_if(eip.first, eip.second,
      [this](const EdgeDescriptor vd)
      {
        const auto is_selected_map = get(boost::edge_is_selected, m_conceptmap_after);
        return get(is_selected_map,vd);
      }
    );
    if (i == eip.second)
    {
      break;
    }
    else
    {
      boost::remove_edge(*i,m_conceptmap_after);
    }
  }

  //Find the selected nodes to be deleted
  for (const auto i: m_scene->items())
  {
    if (QtNode* qtnode = dynamic_cast<QtNode*>(i))
    {
      if (qtnode->isSelected())
      {
        m_qtnodes_removed.emplace_back(qtnode);
      }
    }
  }
  //Find the edges connected to deleted nodes
  for (const auto i: m_scene->items())
  {
    if (QtEdge* qtedge = dynamic_cast<QtEdge*>(i))
    {
      //Is selected itself
      if (qtedge->isSelected())
      {
        m_qtedges_removed.emplace_back(qtedge);
        continue;
      }
      //Is connected to a deleted QtNode
      const auto j = std::find_if(
        std::begin(m_qtnodes_removed),std::end(m_qtnodes_removed),
        [qtedge](const QtNode* const qtnode) {
          return qtedge->GetFrom() == qtnode
            || qtedge->GetTo() == qtnode
          ;
        }
      );
      if (j != std::end(m_qtnodes_removed))
      {
        m_qtedges_removed.emplace_back(qtedge);
      }
    }
  }

}

void ribi::cmap::CommandDeleteSelected::redo()
{
  m_conceptmap = m_conceptmap_after;
  for (const auto qtedge: m_qtedges_removed) {
    m_scene->removeItem(qtedge);
  }
  for (const auto qtnode: m_qtnodes_removed) {
    m_scene->removeItem(qtnode);
  }
  for (auto item: m_selected_before) { item->setSelected(false); }
  m_tool_item->SetBuddyItem(nullptr);
}

void ribi::cmap::CommandDeleteSelected::undo()
{
  m_conceptmap = m_conceptmap_before;
  for (const auto qtnode: m_qtnodes_removed) {
    m_scene->addItem(qtnode);
  }
  for (const auto qtedge: m_qtedges_removed) {
    m_scene->addItem(qtedge);
  }
  for (auto item: m_selected_before) { item->setSelected(true); }

  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);
}