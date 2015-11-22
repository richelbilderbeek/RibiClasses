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
#ifndef CONCEPTMAPCOMMANDCREATENEWNODE_H
#define CONCEPTMAPCOMMANDCREATENEWNODE_H

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

///Start a new node
///-Can be used only when there is an existing concept map
class CommandCreateNewNode final : public Command
{
  public:

  CommandCreateNewNode(ConceptMap& conceptmap);
  CommandCreateNewNode(const CommandCreateNewNode&) = delete;
  CommandCreateNewNode& operator=(const CommandCreateNewNode&) = delete;
  ~CommandCreateNewNode() noexcept {}

  const auto& GetAddedQtNode() const noexcept { return m_node; }

  void redo() override;
  void undo() override;

  private:
  ///There can be one or zero Nodes
  std::vector<Node> m_node;
  ConceptMap& m_conceptmap;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWNODE_H