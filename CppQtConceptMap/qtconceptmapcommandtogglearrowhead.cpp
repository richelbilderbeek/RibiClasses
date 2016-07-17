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
#include "qtconceptmaphelper.h"

ribi::cmap::CommandToggleArrowHead::CommandToggleArrowHead(
  ConceptMap& conceptmap,
  QGraphicsScene * const scene
) : m_conceptmap(conceptmap),
    m_cmap_after{conceptmap},
    m_cmap_before{conceptmap},
    m_edge_after{ExtractTheOneSelectedEdge(conceptmap,*scene)},
    m_edge_before{ExtractTheOneSelectedEdge(conceptmap,*scene)},
    m_scene{scene},
    m_qtedge{ExtractTheOneSelectedQtEdge(*scene)}
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
