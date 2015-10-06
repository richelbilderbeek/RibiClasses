#include "beastparameterfile.h"

#include <cassert>
#include <sstream>

#include "beasttreeprior.h"
#include "container.h"
#include "dnasequence.h"
//#include "fileio.h"

ribi::BeastParameterFile::BeastParameterFile(
  std::vector<ribi::DnaSequence> sequences,
  const std::string& alignment_base_filename,
  const int mcmc_chainlength,
  const TreePrior tree_prior,
  const int date_iso8601
) :
    m_alignment_base_filename{alignment_base_filename},
    m_date_iso8601{date_iso8601},
    m_mcmc_chainlength{mcmc_chainlength},
    m_sequences{MakeDescriptionsUnique(sequences)},
    m_tree_prior{tree_prior}
{
  #ifndef NDEBUG
  Test();
  #endif
}

std::vector<ribi::DnaSequence> ribi::MakeDescriptionsUnique(std::vector<DnaSequence> sequences) noexcept
{
  //O(n^2) solution for today
  const int sz{static_cast<int>(sequences.size())};
  for (int i=0; i!=sz-1; ++i)
  {
    assert(i >= 0 && i < static_cast<int>(sequences.size()));
    auto& s = sequences[i];
    const std::string description{s.GetDescription()};
    int n_duplicates = 0;
    for (int j=i+1; j!=sz; ++j)
    {
      assert(j >= 0 && j < static_cast<int>(sequences.size()));
      auto& t = sequences[j];
      const std::string other_description{t.GetDescription()};
      if (other_description == description)
      {
        ++n_duplicates;
        t.SetDescription(description + "-" + std::to_string(n_duplicates));
      }
    }
    if (n_duplicates != 0)
    {
      s.SetDescription(description + "-0");
    }
  }

  return sequences;
}


std::vector<std::string> ribi::BeastParameterFile::ToBirthDeathXml() const noexcept
{
  if (m_date_iso8601  < 20151005) return ToBirthDeathXml20150101();
  return ToBirthDeathXml20151005();
}


std::ostream& ribi::operator<<(std::ostream& os, const BeastParameterFile file)
{
  const std::vector<std::string> xml{
    file.ToBirthDeathXml()
  };

  const std::string s{
    ribi::Container().ContainerToStr(xml,"\n")
  };
  os << s;
  return os;
}
