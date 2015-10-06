#ifndef BEASTPARAMETERFILE_H
#define BEASTPARAMETERFILE_H

#include <string>
#include <vector>
#include "beasttreeprior.h"

namespace ribi {

struct DnaSequence;

///Creates a BEAST XML parameter file
struct BeastParameterFile
{
  ///All sequences must have unique descriptions for BEAST2 to run.
  ///If these descriptions are not unique, this class will make them unique by adding an index
  /// alignment_base_filename: if the alignment was made
  ///   from e.g. 'MyData.fas', alignment_base_filename must be 'MyData'
  explicit BeastParameterFile(
    std::vector<DnaSequence> sequences,
    const std::string& alignment_base_filename,
    const int mcmc_chainlength,
    const TreePrior tree_prior,
    const int date_iso8601 // = 20151005
  );

  const std::vector<ribi::DnaSequence>& GetSequences() const noexcept { return m_sequences; }
  int GetMcmcChainlengths() const noexcept { return m_mcmc_chainlength; }
  TreePrior GetTreePrior() const noexcept { return m_tree_prior; }

  ///Convert an alignment
  std::vector<std::string> ToBirthDeathXml() const noexcept;

  private:
  const std::string m_alignment_base_filename;
  const int m_date_iso8601;
  const int m_mcmc_chainlength;
  const std::vector<ribi::DnaSequence> m_sequences;
  const TreePrior m_tree_prior;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  std::vector<std::string> ToBirthDeathXml20150101() const noexcept;
  std::vector<std::string> ToBirthDeathXml20151005() const noexcept;
};

//Make the descriptions of all sequences unique
std::vector<DnaSequence> MakeDescriptionsUnique(std::vector<DnaSequence> sequences) noexcept;

std::ostream& operator<<(std::ostream& os, const BeastParameterFile file);

} //~namespace ribi

#endif // BEASTPARAMETERFILE_H
