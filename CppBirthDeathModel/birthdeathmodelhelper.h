#ifndef BIRTHDEATHMODELHELPER_H
#define BIRTHDEATHMODELHELPER_H

#include "rate.h"
#include <vector>

namespace ribi {
namespace bdm {

struct Parameters;

struct Helper
{
  using Rate = ribi::units::Rate;

  enum class Part { phylogeny, branch_lengths };

  Helper();

  ///Calculate the likelihood of the candidate parameters in generating the dataset
  double CalcLogLikelihood(
    const std::string& newick,
    const Rate& birth_rate,
    const Rate& death_rate
  ) const;

  ///Uses DDD package
  double CalcLogLikelihoodDdd(
    const std::string& newick,
    const Rate& birth_rate,
    const Rate& death_rate,
    const Part part
  ) const;

  ///Uses laser package
  double CalcLogLikelihoodLaser(
    const std::string& newick,
    const Rate& birth_rate,
    const Rate& death_rate
  ) const;

  ///Uses DDD package
  void CalcMaxLikelihood(
    const std::string& newick,
    Rate& birth_rate,
    Rate& death_rate,
    const Part part = Part::phylogeny
  ) const;

  std::string CreateSimulatedPhylogeny(
    const Parameters& parameters
  ) const;
};

} //~namespace bm
} //~namespace ribi

#endif // BIRTHDEATHMODELHELPER_H
