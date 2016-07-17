#include "qtconceptmaphelper.h"
#include <QGraphicsScene>
#include "conceptmap.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapcenternode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapcollect.h"
#include "count_vertices_with_selectedness.h"
#include "get_my_custom_edge.h"
#include "count_edges_with_selectedness.h"

int ribi::cmap::CountQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (const QtNode* const qtnode = dynamic_cast<QtNode*>(item)) {
      if (!IsOnEdge(qtnode, scene)) ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)
      && dynamic_cast<QtEdge*>(item)->isSelected()
    ) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtNode*>(item)
      && dynamic_cast<QtNode*>(item)->isSelected()
      && !IsOnEdge(dynamic_cast<QtNode*>(item), scene)
    ) {
      ++cnt;
    }
  }
  return cnt;
}

bool ribi::cmap::DoubleCheckEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_nodes = static_cast<int>(boost::num_vertices(g));
  const auto n_edges = static_cast<int>(boost::num_edges(g));
  const auto n_qtnodes = CountQtNodes(qtconceptmap.GetScene());
  const auto n_qtedges = CountQtEdges(qtconceptmap.GetScene());
  if (n_nodes != n_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_nodes (" << n_nodes << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_edges != n_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_edges (" << n_edges << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_nodes != n_nodes_desired) return false;
  if (n_edges != n_edges_desired) return false;
  return true;
}

bool ribi::cmap::DoubleCheckSelectedEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_selected_nodes = count_vertices_with_selectedness(true,g);
  const auto n_selected_edges = count_edges_with_selectedness(true,g);
  const auto n_selected_qtnodes = CountSelectedQtNodes(qtconceptmap.GetScene());
  const auto n_selected_qtedges = CountSelectedQtEdges(qtconceptmap.GetScene());

  if (n_selected_nodes != n_selected_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_nodes (" << n_selected_nodes << ") != n_selected_qtnodes ("
      << n_selected_qtnodes << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_edges != n_selected_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_edges (" << n_selected_edges << ") != n_selected_qtedges ("
      << n_selected_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_nodes != n_nodes_desired) return false;
  if (n_selected_edges != n_edges_desired) return false;
  return true;
}

ribi::cmap::Edge ribi::cmap::ExtractTheOneSelectedEdge(
  const ConceptMap& conceptmap, const QGraphicsScene& scene
)
{
  const auto qtedge = ExtractTheOneSelectedQtEdge(scene);
  const auto ed = find_first_custom_edge_with_my_edge(qtedge->GetEdge(), conceptmap);
  const Edge edge = get_my_custom_edge(ed, conceptmap);
  assert(edge == qtedge->GetEdge());
  return edge;
}

ribi::cmap::QtEdge * ribi::cmap::ExtractTheOneSelectedQtEdge(const QGraphicsScene& scene)
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
  else if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
  {
    //Or is it the node on an edge?
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

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const int edge_id,
  const QGraphicsScene * const scene
) noexcept
{
  for (auto item: scene->items())
  {
    if (QtEdge * qtedge = dynamic_cast<QtEdge*>(item))
    {
      if (qtedge->GetEdge().GetId() == edge_id) { return qtedge; }
    }
  }
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept
{

  assert(from);
  assert(to);
  assert(from != to);
  const std::vector<QtEdge*> edge_concepts = Collect<QtEdge>(scene);
  const auto iter = std::find_if(edge_concepts.begin(),edge_concepts.end(),
    [from,to](const QtEdge* const qtedge)
    {
      return
        (*qtedge->GetFrom() == *from && *qtedge->GetTo() == *to)
     || (*qtedge->GetFrom() == *to && *qtedge->GetTo() == *from);
    }
  );
  if (iter == edge_concepts.end()) return nullptr;
  return * iter;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept
{
  for (const auto item: scene->items())
  {
    if (QtEdge* const qtedge = dynamic_cast<QtEdge*>(item))
    {
      if (qtedge->GetQtNode() == qtnode) return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::FindQtNode(
  const int node_id, const QGraphicsScene * const scene) noexcept
{
  for (auto item: scene->items())
  {
    if (QtNode * qtnode = dynamic_cast<QtNode*>(item))
    {
      if (qtnode->GetNode().GetId() == node_id) { return qtnode; }
    }
  }
  return nullptr;
}

std::vector<ribi::cmap::QtEdge*> ribi::cmap::GetQtEdges(
  const QtNode* const from,
  const QGraphicsScene * const scene
) noexcept
{
  assert(from);
  const std::vector<QtEdge*> v = GetQtEdges(scene);
  std::vector<QtEdge*> w;
  std::copy_if(v.begin(),v.end(),std::back_inserter(w),
    [from](const QtEdge* const qtedge)
    {
      return *qtedge->GetFrom() == *from || *qtedge->GetTo() == *from;
    }
  );
  return w;
}


std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtEdge>(scene);
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtNode>(scene);
}

std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtEdge *> selected;
  const auto qtedges = GetQtEdges(&scene);
  std::copy_if(
    std::begin(qtedges),
    std::end(qtedges),
    std::back_inserter(selected),
    [](QtEdge* const qtedge) { return qtedge->isSelected() || qtedge->GetQtNode()->isSelected(); }
  );
  return selected;
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetSelectedQtNodes(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtNode *> selected;
  const auto qtnodes = GetQtNodes(&scene);
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [](QtNode* const qtnode) { return qtnode->isSelected(); }
  );
  return selected;
}

bool ribi::cmap::IsQtCenterNode(const QGraphicsItem* const item)
{
  const QtCenterNode * const qtnode = dynamic_cast<const QtCenterNode*>(item);
  assert(!qtnode || IsCenterNode(qtnode->GetNode()));
  return qtnode;
}

bool ribi::cmap::IsOnEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept
{
  return FindQtEdge(qtnode, scene);
}