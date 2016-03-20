//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

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
#ifndef CONCEPTMAPEDGEFACTORY_H
#define CONCEPTMAPEDGEFACTORY_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapfwd.h"
#include "conceptmapnode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

struct EdgeFactory
{
  EdgeFactory() noexcept;

  Edge Create(
    const Node& from,
    const Node& to
  ) const noexcept;

  Edge Create(
    const Node& node
  ) const noexcept;

  int GetNumberOfTests() const noexcept;
  Edge GetTest(const int index) const noexcept;
  std::vector<Edge> GetTests() const noexcept;

  int GetNumberOfNastyTests() const noexcept;
  Edge GetNastyTest(const int index) const noexcept;
  std::vector<Edge> GetNastyTests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEDGEFACTORY_H
