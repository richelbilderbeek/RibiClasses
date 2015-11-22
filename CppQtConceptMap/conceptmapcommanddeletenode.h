//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPCOMMANDDELETENODE_H
#define CONCEPTMAPCOMMANDDELETENODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapcommand.h"
#include "conceptmapnode.h"
#include "conceptmap.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Delete an existing node
class CommandDeleteNode final : public Command
{
  public:

  using EdgePtr = Edge;
  using Edges = std::vector<EdgePtr>;
  using Nodes = std::vector<Node>;
  using EdgesAndNodes = std::pair<Edges,Nodes>;

  CommandDeleteNode(
    ConceptMap& conceptmap,
    const Node& node
  );
  CommandDeleteNode(const CommandDeleteNode&) = delete;
  CommandDeleteNode& operator=(const CommandDeleteNode&) = delete;
  ~CommandDeleteNode() noexcept {}

  void undo() override;
  void redo() override;
  void SetVerbosity(const bool verbose) noexcept { m_verbose = verbose; }

  private:
  ConceptMap& m_conceptmap;
  const Edges m_deleted_edges;
  const Node m_node;
  const EdgesAndNodes m_old_selected; //Selected before command
  bool m_verbose;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDDELETENODE_H