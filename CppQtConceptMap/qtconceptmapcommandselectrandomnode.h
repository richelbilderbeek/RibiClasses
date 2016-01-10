#ifndef QTCONCEPTMAPCOMMANDSELECTRANDOMNODE_H
#define QTCONCEPTMAPCOMMANDSELECTRANDOMNODE_H

/*

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapqtnode.h"
#include <QGraphicsScene>
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandSelectRandomNode final : public Command
{
  public:

  CommandSelectRandomNode(
    ConceptMap& conceptmap,
    QGraphicsScene * const scene,
    QtTool * const tool_item
  );
  CommandSelectRandomNode(const CommandSelectRandomNode&) = delete;
  CommandSelectRandomNode& operator=(const CommandSelectRandomNode&) = delete;
  ~CommandSelectRandomNode() noexcept {}

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap;
  ConceptMap m_conceptmap_after;
  const ConceptMap m_conceptmap_before;
  Node m_node_selected_after;
  QGraphicsScene * const m_scene;
  const QList<QGraphicsItem*> m_selected_before;
  QtTool * const m_tool_item;
  QtNode * const m_tool_item_old_buddy;
};

} //~namespace cmap
} //~namespace ribi

*/

#endif // QTCONCEPTMAPCOMMANDSELECTRANDOMNODE_H
