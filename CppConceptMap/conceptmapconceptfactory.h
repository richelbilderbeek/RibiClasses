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
#ifndef CONCEPTMAPCONCEPTFACTORY_H
#define CONCEPTMAPCONCEPTFACTORY_H

#include <array>
#include <string>
#include <vector>
#include "conceptmapcompetency.h"
#include "conceptmapfwd.h"

namespace ribi {
namespace cmap {

///Creates Concepts
struct ConceptFactory
{
  ConceptFactory() noexcept;

  Concept Create(
    const std::string& name = "...",
    const std::vector<std::pair<std::string,Competency> >& examples = {},
    const bool is_complex = true,
    const int rating_complexity = -1,
    const int rating_concreteness = -1,
    const int rating_specificity = -1
  ) const noexcept;

  Concept GetTest(const int i) const noexcept;
  std::vector<Concept> GetNastyTests() const noexcept;
  std::array<Concept, 3> GetTests() const noexcept;
  //int GetNumberOfTests() const noexcept { return static_cast<int>(GetTests().size()); }
  int GetNumberOfTests() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCONCEPTFACTORY_H
