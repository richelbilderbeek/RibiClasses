#include "qtconceptmapcommandtogglearrowhead.h"

#include <cassert>
#include <sstream>

#include "conceptmap.h"

#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "get_my_custom_edge.h"
#include "set_my_custom_edge.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "trace.h"
#include "count_vertices_with_selectedness.h"
#include "add_edge_between_selected_vertices.h"
#include <boost/graph/isomorphism.hpp>
#include <QGraphicsScene>
#include "qtconceptmap.h"
#include "qtconceptmaptoolsitem.h"

ribi::cmap::CommandToggleArrowHead::CommandToggleArrowHead(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene
) : m_conceptmap(conceptmap),
    m_after{conceptmap},
    m_before{conceptmap},
    m_scene{scene},
    m_qtedge{nullptr}
{
  if (m_scene->selectedItems().size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have one selected item, instead of "
      << m_scene->selectedItems().size() << " items"
    ;
    throw std::invalid_argument(msg.str());
  }
  auto item = m_scene->selectedItems().front();
  //Is it an edge?
  if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item)) { m_qtedge = qtedge; }
  //Or is it the node on an edge?
  else if (QtNode * const qtnode = dynamic_cast<QtNode*>(item)) { m_qtedge = FindQtEdge(qtnode, scene); }

  if (!m_qtedge)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "The selected item must be a QtEdge, "
      << "or a QtNode on a QtEdge"
    ;
    throw std::invalid_argument(msg.str());
  }

  this->setText("Toggle arrow head");


}

void ribi::cmap::CommandToggleArrowHead::redo()
{
  const auto ed = find_first_custom_edge_with_my_edge(m_qtedge->GetEdge(), m_conceptmap);
  Edge edge = get_my_custom_edge(ed, m_conceptmap);
  const auto old_has_head = edge.HasHeadArrow();
  const auto new_has_head = !old_has_head;
  edge.SetHeadArrow(new_has_head);
  assert(edge.HasHeadArrow() == new_has_head);

  set_my_custom_edge(edge, ed, m_conceptmap);
  m_qtedge->SetEdge(edge);

  assert(new_has_head == get_my_custom_edge(find_first_custom_edge_with_my_edge(m_qtedge->GetEdge(), m_conceptmap), m_conceptmap).HasHeadArrow());
  assert(m_qtedge->GetEdge().HasHeadArrow() == new_has_head);
}

void ribi::cmap::CommandToggleArrowHead::undo()
{
  // A toggle after a toggle equals an undo :-)
  redo();
}
