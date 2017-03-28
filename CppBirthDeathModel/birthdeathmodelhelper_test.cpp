#include "birthdeathmodelhelper.h"

void ribi::bdm::Helper::Test() noexcept
{
  //CreateSimulatedPhylogeny must create a newick
  {
    const Parameters parameters(
      0.1 / boost::units::si::second,
      0.0 / boost::units::si::second,
      10,
      42
    );
    assert(!h.CreateSimulatedPhylogeny(parameters).empty());
  }
  //CalcLogLikelihood
  {
    const std::string newick{"((F:2,G:2):1,H:3);"};
    const Rate& birth_rate{0.20 / boost::units::si::second};
    const Rate& death_rate{0.01 / boost::units::si::second};
    const double log_likelihood{
      h.CalcLogLikelihood(
        newick,
        birth_rate,
        death_rate
      )
    };
    assert(log_likelihood <= 0.0);
  }
  //CalcLogLikelihoodDdd and CalcLogLikelihoodLaser should give same results
  {
    const std::string newick{"((F:2,G:2):1,H:3);"};
    const Rate& birth_rate{0.20 / boost::units::si::second};
    const Rate& death_rate{0.01 / boost::units::si::second};
    const double log_likelihood_ddd{
      h.CalcLogLikelihoodDdd(
        newick,
        birth_rate,
        death_rate,
        Part::branch_lengths
      )
    };
    const double log_likelihood_laser{
      h.CalcLogLikelihoodLaser(
        newick,
        birth_rate,
        death_rate
      )
    };
    std::cout
      << "log_likelihood_laser: " << log_likelihood_laser << '\n'
      << "log_likelihood_ddd: " << log_likelihood_ddd << '\n'
    ;

    assert(abs(log_likelihood_laser - log_likelihood_ddd) < 0.0001);
  }
  //CalcMaxLikelihood
  {
    const std::string newick{"((F:2,G:2):1,H:3);"};
    Rate birth_rate{0.0 / boost::units::si::second};
    Rate death_rate{0.0 / boost::units::si::second};
    h.CalcMaxLikelihood(
      newick,
      birth_rate,
      death_rate,
      Part::phylogeny
    );
    assert(birth_rate > 0.0 / boost::units::si::second);
    assert(death_rate > 0.0 / boost::units::si::second);
  }
}
