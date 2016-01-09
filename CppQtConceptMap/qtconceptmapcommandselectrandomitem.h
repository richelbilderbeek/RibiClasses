#ifndef QTCONCEPTMAPCOMMANDSELECTRANDOMITEM_H
#define QTCONCEPTMAPCOMMANDSELECTRANDOMITEM_H

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
class CommandSelectRandomItem final : public Command
{
  public:

  CommandSelectRandomItem(
    ConceptMap& conceptmap,
    QGraphicsScene * const scene,
    QtTool * const tool_item
  );
  CommandSelectRandomItem(const CommandSelectRandomItem&) = delete;
  CommandSelectRandomItem& operator=(const CommandSelectRandomItem&) = delete;
  ~CommandSelectRandomItem() noexcept {}

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap;
  ConceptMap m_conceptmap_after;
  const ConceptMap m_conceptmap_before;
  QtNode * m_qtnode;
  QGraphicsScene * const m_scene;
  QtTool * const m_tool_item;
  QtNode * const m_tool_item_old_buddy;
};

} //~namespace cmap
} //~namespace ribi

*/

#endif // QTCONCEPTMAPCOMMANDSELECTRANDOMITEM_H
