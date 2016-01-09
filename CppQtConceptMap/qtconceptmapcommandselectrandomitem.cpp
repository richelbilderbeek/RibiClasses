#include "qtconceptmapcommandselectrandomitem.h"

/*

#include <cassert>
#include <boost/graph/isomorphism.hpp>

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"

ribi::cmap::CommandSelectRandomItem::CommandSelectRandomItem(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene,
  QtTool * const tool_item
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after{conceptmap},
    m_conceptmap_before{conceptmap},
    m_qtedge{nullptr},
    m_qtnode{nullptr},
    m_scene(scene),
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item->GetBuddyItem()}
{
  this->setText("Select random item");

  //Unselect everything in the current concept map
  m_conceptmap_after = UnselectEverything(m_conceptmap_after);

  const int n_edges{boost::num_edges(m_conceptmap_after)};
  const int n_nodes{boost::num_vertices(m_conceptmap_after)};
  const int n_items{n_edges + n_nodes};
  if (n_items == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": cannot select a random item without items";
    throw std::logic_error(msg.str());
  }
  const int random_item = std::rand() % n_items;

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
}

void ribi::cmap::CommandSelectRandomItem::redo()
{
  m_conceptmap = m_conceptmap_after;
  m_scene->addItem(m_qtnode);
  m_qtnode->SetSelected(true); //Additively select node
  m_qtnode->setFocus();
  m_tool_item->SetBuddyItem(m_qtnode);
}

void ribi::cmap::CommandSelectRandomItem::undo()
{
  m_conceptmap = m_conceptmap_before;
  m_scene->removeItem(m_qtnode);
  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);
}

*/
