#ifndef RIBI_BEAST_H
#define RIBI_BEAST_H

#include <string>
#include <vector>

namespace ribi {

///Class to call BEAST2
struct Beast
{
  ///Throws if BEAST2 cannot be found
  Beast();

  void AnalyzeBirthDeath(
    const std::string& log_file,
    const std::string png_filename
  ) const;

  void AnalyzeCoalescent(
    const std::string& log_file,
    const std::string png_filename_coalescent_constant,
    const std::string png_filename_popsize
  ) const;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  void Run(
    const std::string& xml_input_filename,
    const std::string& log_output_filename,
    const std::string& trees_output_filename
  ) const noexcept;

  ///Do not use
  void AnalyzeBirthDeathRinside(
    const std::string& log_file,
    const std::string png_filename
  ) const;

  ///Do not use
  void AnalyzeBirthDeathRscript(
    const std::string& log_file,
    const std::string png_filename
  ) const;

  ///Do not use
  void AnalyzeCoalescentRinside(
    const std::string& log_file,
    const std::string png_filename_coalescent_constant,
    const std::string png_filename_popsize
  ) const;

  ///Do not use
  void AnalyzeCoalescentRscript(
    const std::string& log_file,
    const std::string png_filename_coalescent_constant,
    const std::string png_filename_popsize
  ) const;

  private:
  ///Full path of the BEAST2 executable
  const std::string m_beast_path;
};

///Try to find the BEAST2 executable's path
std::string get_beast_path();

} //~namespace ribi

#endif //RIBI_BEAST_H
