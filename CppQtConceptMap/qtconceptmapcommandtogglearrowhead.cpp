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
    m_cmap_after{conceptmap},
    m_cmap_before{conceptmap},
    m_edge_after{ExtractEdge(conceptmap,*scene)},
    m_edge_before{ExtractEdge(conceptmap,*scene)},
    m_scene{scene},
    m_qtedge{ExtractQtEdge(*scene)}
{
  this->setText("Toggle arrow head");

  //Create 'm_edge_after'
  {
    const auto old_has_head = m_edge_after.HasHeadArrow();
    const auto new_has_head = !old_has_head;
    m_edge_after.SetHeadArrow(new_has_head);
  }

  //Create concept map 'm_after'
  {
    const auto ed = find_first_custom_edge_with_my_edge(m_edge_before, m_cmap_after);
    set_my_custom_edge(m_edge_after, ed, m_cmap_after);
  }
}

ribi::cmap::Edge ribi::cmap::CommandToggleArrowHead::ExtractEdge(
  const ConceptMap& conceptmap, const QGraphicsScene& scene
)
{
  const auto qtedge = ExtractQtEdge(scene);
  const auto ed = find_first_custom_edge_with_my_edge(qtedge->GetEdge(), conceptmap);
  const Edge edge = get_my_custom_edge(ed, conceptmap);
  assert(edge == qtedge->GetEdge());
  return edge;
}

ribi::cmap::QtEdge * ribi::cmap::CommandToggleArrowHead::ExtractQtEdge(const QGraphicsScene& scene)
{
  if (scene.selectedItems().size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have one selected item, instead of "
      << scene.selectedItems().size() << " items"
    ;
    throw std::invalid_argument(msg.str());
  }
  auto item = scene.selectedItems().front();

  //Is it an edge?
  if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
  {
    return qtedge;
  }

  //Or is it the node on an edge?
  else if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
  {
    QtEdge * const qtedge = FindQtEdge(qtnode, &scene);
    if (qtedge) return qtedge;
  }

  //Nope, it cannot be found
  std::stringstream msg;
  msg << __func__ << ": "
    << "The selected item must be a QtEdge, "
    << "or a QtNode on a QtEdge"
  ;
  throw std::invalid_argument(msg.str());
}

void ribi::cmap::CommandToggleArrowHead::redo()
{
  m_conceptmap = m_cmap_after;
  m_qtedge->SetEdge(m_edge_after);
}

void ribi::cmap::CommandToggleArrowHead::undo()
{
  m_conceptmap = m_cmap_before;
  m_qtedge->SetEdge(m_edge_before);
}
