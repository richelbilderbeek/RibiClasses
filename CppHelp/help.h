#ifndef RIBI_HELP_H
#define RIBI_HELP_H

#include <iosfwd>
#include <string>
#include <vector>

namespace ribi {

///Help is used to manage a help screen its info
///Help always has some default options, as used by each MenuDialog
struct Help
{
  //Help options
  struct Option
  {
    explicit Option(
      const char option_short,
      const std::string& option_long,
      const std::string& option_description
    );
    char m_short;              // a
    std::string m_long;        // about
    std::string m_description; // displays the about information
  };
  explicit Help(
    const std::string& program_name,
    const std::string& program_description,
    const std::vector<Option>& options,
    const std::vector<std::string> example_uses
  );

  /// Add the default options
  static const std::vector<Option> AddDefaultOptions(const std::vector<Option>& options);

  const std::vector<std::string>& GetExampleUses() const noexcept { return m_example_uses; }
  const std::vector<Option>& GetOptions() const noexcept { return  m_options; }
  const std::string& GetProgramDescription() const noexcept { return  m_program_description; }
  const std::string& GetProgramName() const noexcept { return  m_program_name; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:

  // { "ProjectRichelBilderbeek --about", "ProjectRichelBilderbeek ToolHometrainer Exercise.txt" }
  const std::vector<std::string> m_example_uses;
  const std::vector<Option> m_options;

  //RichelBilderbeek's work
  const std::string m_program_description;

  //ProjectRichelBilderbeek
  const std::string m_program_name;

};

bool operator==(const Help::Option& lhs, const Help::Option& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Help& help);

} //~namespace ribi

#endif // RIBI_HELP_H
