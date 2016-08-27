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
#ifndef CONCEPTMAPEXAMPLE_H
#define CONCEPTMAPEXAMPLE_H

#include <string>
#include "conceptmapcompetency.h"

namespace ribi {
namespace cmap {

///A concept (on a node or an edge) can have examples
///Example is displayed by:
/// - QtExampleDialog
struct Example
{
  explicit Example(
    const std::string& text = "",
    const cmap::Competency competency = Competency::uninitialized,
    const bool is_complex = true,
    const bool is_concrete = true,
    const bool is_specific = true
  );

  ///When all text is GraphViz-friendly encoded, Decode will get the normal text back
  void Decode() noexcept;

  ///Get the competency, as might be judged by an assessor
  Competency GetCompetency() const noexcept { return m_competency; }

  ///Has an assessor rated this example as being an addition to the complexity?
  bool GetIsComplex() const noexcept { return m_is_complex; }

  ///Has an assessor rated this example as being an addition to the concreteness?
  bool GetIsConcrete() const noexcept { return m_is_concrete; }

  ///Has an assessor rated this example as being an addition to the specificity?
  bool GetIsSpecific() const noexcept { return m_is_specific; }

  ///Get the text of the example
  const std::string& GetText() const noexcept { return m_text; }

  ///Set the competency
  void SetCompetency(const Competency competency) noexcept;

  ///Has an assessor rated this example as being an addition to the complexity?
  void SetIsComplex(const bool is_complex) noexcept;

  ///Has an assessor rated this example as being an addition to the concreteness?
  void SetIsConcrete(const bool is_concrete) noexcept;

  ///Has an assessor rated this example as being an addition to the specificity?
  void SetIsSpecific(const bool is_specific) noexcept;

  ///Set the text
  void SetText(const std::string& text) noexcept;

  ///Convert Example to a short std::string
  std::string ToStr() const noexcept;

private:

  ///The competency, as might be judged by an assessor
  Competency m_competency;

  ///Has an assessor rated this example as being an addition to the complexity?
  bool m_is_complex;

  ///Has an assessor rated this example as being an addition to the concreteness?
  bool m_is_concrete;

  ///Has an assessor rated this example as being an addition to the specificity?
  bool m_is_specific;

  ///The text of the example
  ///For example: 'Plato', 'When I was a kid', 'As I did on holiday'
  std::string m_text;

  ///Set the competency with a string
  void SetCompetencyAsStr(const std::string& s) const;
};


std::string ToXml(const Example& example) noexcept;
Example XmlToExample(const std::string& s);

Competency ExtractExampleCompetencyFromXml(const std::string& s);
bool ExtractExampleIsComplexFromXml(const std::string& s);
bool ExtractExampleIsConcreteFromXml(const std::string& s);
bool ExtractExampleIsSpecificFromXml(const std::string& s);
std::string ExtractExampleTextFromXml(const std::string& s);

std::ostream& operator<<(std::ostream& os, const Example& example) noexcept;
std::istream& operator>>(std::istream& is, Example& example);

bool operator==(const Example& lhs, const Example& rhs) noexcept;
bool operator!=(const Example& lhs, const Example& rhs) noexcept;

bool operator<(const Example& lhs,const Example& rhs) noexcept;
bool operator>(const Example& lhs,const Example& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPEXAMPLE_H
