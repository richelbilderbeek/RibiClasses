#ifndef CONCEPTMAPCOMMANDDELETEEDGE_H
#define CONCEPTMAPCOMMANDDELETEEDGE_H

#include "conceptmapcommand.h"
#include "conceptmapedge.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///Delete an existing edge
class CommandDeleteEdge final : public Command
{
  public:

  CommandDeleteEdge(
    ConceptMap& conceptmap,
    const Edge& edge
  );
  CommandDeleteEdge(const CommandDeleteEdge&) = delete;
  CommandDeleteEdge& operator=(const CommandDeleteEdge&) = delete;
  ~CommandDeleteEdge() noexcept {}

  void undo() override;
  void redo() override;

  private:
  ConceptMap& m_conceptmap;
  const Edge m_edge;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDDELETEEDGE_H
