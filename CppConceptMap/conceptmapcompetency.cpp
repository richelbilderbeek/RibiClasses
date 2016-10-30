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
#include "conceptmapcompetency.h"
#include "conceptmapcompetencies.h"

std::string ribi::cmap::CompetencyToStr(const Competency competency) noexcept
{
  return Competencies().ToStr(competency);
}

ribi::cmap::Competency ribi::cmap::StrToCompetency(const std::string s)
{
  return Competencies().ToType(s);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Competency competency)
{
  os << Competencies().ToStr(competency);
  return os;
}
