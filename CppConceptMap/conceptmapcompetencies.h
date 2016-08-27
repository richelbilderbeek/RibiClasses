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
#ifndef CONCEPTMAPCOMPETENCIES_H
#define CONCEPTMAPCOMPETENCIES_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/bimap.hpp>
#include "conceptmapcompetency.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Functions to work on with the Competency enumeration
struct Competencies
{
  Competencies();
  std::vector<Competency> GetAllCompetencies() const noexcept;
  int ToIndex(const Competency competency) const noexcept;
  std::string ToStrDutch(const Competency competency) const noexcept;
  std::string ToStr(const Competency competency) const noexcept;
  Competency ToTypeFromDutch(const std::string& dutch_string) const;
  Competency ToType(const std::string& s) const;

  private:

  mutable boost::bimap<Competency,std::string> m_map_dutch; //Lazy initialization
  mutable boost::bimap<Competency,std::string> m_map_english; //Lazy initialization
};

boost::bimap<Competency,std::string> CreateMapDutch() noexcept;
boost::bimap<Competency,std::string> CreateMapEnglish() noexcept;

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMPETENCIES_H
