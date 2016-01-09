#include "qtconceptmapcommandselectrandomnode.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>

#include "qtconceptmap.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"

ribi::cmap::CommandSelectRandomNode::CommandSelectRandomNode(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene,
  QtTool * const tool_item
)
  : m_conceptmap(conceptmap),
    m_conceptmap_after{conceptmap},
    m_conceptmap_before{conceptmap},
    m_node_selected_after{},
    m_scene(scene),
    m_selected_before{scene->selectedItems()},
    m_tool_item{tool_item},
    m_tool_item_old_buddy{tool_item->GetBuddyItem()}
{
  this->setText("Select random node");

  //Unselect everything in the current concept map
  m_conceptmap_after = UnselectEverything(m_conceptmap_after);

  const int n_nodes{static_cast<int>(boost::num_vertices(m_conceptmap_after))};
  if (n_nodes == 0)
  {
    std::stringstream msg;
    msg << __func__ << ": cannot select a random node without nodes";
    throw std::logic_error(msg.str());
  }

  //Select the node
  const int random_node_index{std::rand() % n_nodes};
  const auto vip = vertices(m_conceptmap_after);
  auto vdi = vip.first;
  for (int j=0; j!=random_node_index; ++j, ++vdi) {}
  assert(vdi != vip.second);
  const auto vertex_is_selected_map = get(boost::vertex_is_selected, m_conceptmap_after);
  put(vertex_is_selected_map,*vdi,true);
  const auto vertex_custom_type_map = get(boost::vertex_custom_type, m_conceptmap_after);
  m_node_selected_after = get(vertex_custom_type_map,*vdi);
}

void ribi::cmap::CommandSelectRandomNode::redo()
{
  m_conceptmap = m_conceptmap_after;

  const auto qtnode = FindQtNode(m_node_selected_after,*m_scene);
  assert(qtnode);
  qtnode->SetSelected(true);
  qtnode->setFocus();
  m_tool_item->SetBuddyItem(qtnode);
}

void ribi::cmap::CommandSelectRandomNode::undo()
{
  m_conceptmap = m_conceptmap_before;

  const auto qtnode = FindQtNode(m_node_selected_after,*m_scene);
  assert(qtnode);
  qtnode->SetSelected(false);
  qtnode->clearFocus();

  for (const auto item: m_selected_before) {
    item->setSelected(true);
  }

  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);
}

