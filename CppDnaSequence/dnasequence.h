#ifndef RIBI_DNASEQUENCE_H
#define RIBI_DNASEQUENCE_H

#include <string>

namespace ribi {

///Stores a DNA sequence as an upper-case string without spaces
struct DnaSequence
{
  explicit DnaSequence(const std::string& description, const std::string& sequence);

  const std::string& GetDescription() const noexcept { return m_description; }
  const std::string& GetSequence() const noexcept { return m_sequence; }

  void SetDescription(const std::string& description) noexcept { m_description = description; }

  private:
  std::string m_description;
  const std::string m_sequence;
};

///Will throw if there are unexpected characters
std::string CleanSequence(std::string sequence);

bool operator==(const DnaSequence& lhs, const DnaSequence& rhs) noexcept;
bool operator!=(const DnaSequence& lhs, const DnaSequence& rhs) noexcept;

} //~namespace ribi
#endif // RIBI_DNASEQUENCE_H
