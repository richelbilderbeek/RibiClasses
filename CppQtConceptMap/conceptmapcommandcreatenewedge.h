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
#ifndef CONCEPTMAPCOMMANDCREATENEWEDGE_H
#define CONCEPTMAPCOMMANDCREATENEWEDGE_H

#include "conceptmap.h"
#include "conceptmapcommand.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandCreateNewEdgeBetweenTwoSelectedNodes final : public Command
{
  public:

  CommandCreateNewEdgeBetweenTwoSelectedNodes(ConceptMap& conceptmap);
  CommandCreateNewEdgeBetweenTwoSelectedNodes(const CommandCreateNewEdgeBetweenTwoSelectedNodes&) = delete;
  CommandCreateNewEdgeBetweenTwoSelectedNodes& operator=(const CommandCreateNewEdgeBetweenTwoSelectedNodes&) = delete;
  ~CommandCreateNewEdgeBetweenTwoSelectedNodes() noexcept {}

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap;
  const ConceptMap m_before;
  const ConceptMap m_after;

};

//Throws if there are no two nodes selected
ConceptMap CreateNewEdgeFromTwoSelectedNodes(ConceptMap g);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWEDGE_H
