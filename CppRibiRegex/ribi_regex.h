#ifndef RIBI_REGEX_H
#define RIBI_REGEX_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <string>
#include <vector>
#pragma GCC diagnostic pop

namespace ribi {

///Regex functions
struct Regex
{
  Regex();

  //From http://www.richelbilderbeek.nl/CppGetRegexMatches.htm
  std::vector<std::string>
    GetRegexMatches(
    const std::string& s,
    const std::string& regex_str
  ) const { return GetRegexMatchesXpressive(s,regex_str); }

  ///Note: works differently than expected
  std::vector<std::string>
    GetRegexMatchesQRegExp(
    const std::string& s,
    const std::string& regex_str
  ) const noexcept;

  std::vector<std::string>
    GetRegexMatchesXpressive(
    const std::string& s,
    const std::string& regex_str
  ) const;

  std::string GetRegexDutchFloat() const noexcept;
  std::string GetRegexDutchPostalCode() const noexcept;
  std::string GetRegexPolygon() const noexcept;
  std::string GetRegexShapes() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  bool IsValidXpressive(const std::string& regex_str) const noexcept;
  bool IsValid(const std::string& regex_str) const noexcept { return IsValidXpressive(regex_str); }
};

} //~namespace ribi

#endif // RIBI_REGEX_H
