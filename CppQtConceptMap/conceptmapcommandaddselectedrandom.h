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
#ifndef CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H
#define CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H

#include "conceptmapcommand.h"
#include "conceptmapnode.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///Add another item to the selected pool
class CommandAddSelectedRandom final : public Command
{
  public:

  using ConstEdges = std::vector<Edge>;
  using ConstNodes = std::vector<Node>;
  using Edges = std::vector<Edge>;
  using Nodes = std::vector<Node>;
  using EdgesAndNodes = std::pair<Edges,Nodes>;
  using ConstEdgesAndNodes = std::pair<ConstEdges,ConstNodes>;

  CommandAddSelectedRandom(ConceptMap& conceptmap);
  CommandAddSelectedRandom(const CommandAddSelectedRandom&) = delete;
  CommandAddSelectedRandom& operator=(const CommandAddSelectedRandom&) = delete;
  ~CommandAddSelectedRandom() noexcept {}

  void undo() override;
  void redo() override;

  private:
  ConceptMap& m_conceptmap;
  EdgesAndNodes m_new_selected; //Selected after redo
  const EdgesAndNodes m_old_selected; //Selected before redo
};

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H