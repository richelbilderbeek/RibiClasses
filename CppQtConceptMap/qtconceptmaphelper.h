#ifndef QTCONCEPTMAPHELPER_H
#define QTCONCEPTMAPHELPER_H

#include <vector>
#include <QBrush>
#include "qtconceptmapfwd.h"
#include "qtconceptmapmode.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///Counts the QtNodes that are Nodes, i.e. are not on an edge
int CountQtNodes(const QGraphicsScene * const scene) noexcept;
int CountQtEdges(const QGraphicsScene * const scene) noexcept;
int CountSelectedQtNodes(const QGraphicsScene * const scene) noexcept;
int CountSelectedQtEdges(const QGraphicsScene * const scene) noexcept;

///Check is QtConceptMap and its ConceptMap have the requested number of edges and nodes
///Will throw if there is an internal inconsistency
bool DoubleCheckEdgesAndNodes(const QtConceptMap& qtconceptmap, const int n_edges, const int n_nodes);

///Check is QtConceptMap and its ConceptMap have the requested number of selected edges and nodes
///Will throw if there is an internal inconsistency
bool DoubleCheckSelectedEdgesAndNodes(const QtConceptMap& qtconceptmap, const int n_edges, const int n_nodes);

///Get the one selected Edge. Throws if there is not exactly one edge selected
Edge ExtractTheOneSelectedEdge(const ConceptMap& conceptmap, const QGraphicsScene& scene);

///Get the one selected QtEdge. Throws if there is not exactly one edge selected
QtEdge * ExtractTheOneSelectedQtEdge(const QGraphicsScene& scene);

QtEdge * FindQtEdge(const int edge_id, const QGraphicsScene * const scene) noexcept;

//Find the Qt edge with the same from and to
QtEdge * FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept;

///Find the edge this QtNode is in the center of
///Returns nullptr if the QtNode is not on a QtEdge
QtEdge * FindQtEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept;

QtNode * FindQtNode(const int node_id, const QGraphicsScene * const scene) noexcept;

std::vector<QtEdge *> GetQtEdges(const QGraphicsScene * const scene) noexcept;

///Get all the edges connected to the concept
std::vector<QtEdge*> GetQtEdges(
  const QtNode * const from,
  const QGraphicsScene * const scene
) noexcept;

///The function how a QtNode determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunction(const Mode mode);

///The function how a QtNode in Mode::Edit determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionEdit() noexcept;

///The function how a QtNode in Mode::Rate determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionRate() noexcept;

///The function how a QtNode in Mode::Unitialized determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionUninitialized() noexcept;

///Get all the QtNodes, also those that are part of a QtEdge
std::vector<QtNode *> GetQtNodes(const QGraphicsScene * const scene) noexcept;

///Get all the 'standalone' QtNodes; QtNodes that QtEdge can connect to;
std::vector<QtNode *> GetQtNodesNotOnEdge(const QGraphicsScene * const scene) noexcept;

std::vector<QtEdge *> GetSelectedQtEdges(const QGraphicsScene& scene) noexcept;
std::vector<QtNode *> GetSelectedQtNodes(const QGraphicsScene& scene) noexcept;

///Check if this item is the center node
bool IsQtCenterNode(const QGraphicsItem* const item);

///Is this QtNode in the center on a QtEdge?
bool IsOnEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPHELPER_H
