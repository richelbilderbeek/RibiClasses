#include "birthdeathmodelsimulation.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "birthdeathmodelhelper.h"
#include <boost/math/constants/constants.hpp>

ribi::bdm::Simulation::Simulation(
  const Parameters& parameters
)
  : m_exponential_distribution(parameters.GetBirthRate().value() + parameters.GetDeathRate().value()),
    m_parameters{parameters},
    m_rng(parameters.GetRngSeed()),
    m_time{0.0 * boost::units::si::second},
    m_uniform_distribution(0.0,1.0)
{

}

void ribi::bdm::Simulation::CalcNext(const double x)
{
  const double r{
    CountExtant()
    * (m_parameters.GetBirthRate().value() + m_parameters.GetDeathRate().value())
  };
  if (r != m_exponential_distribution.lambda())
  {
    m_exponential_distribution = std::exponential_distribution<double>(r);
  }

  const double random_exponential{m_exponential_distribution(m_rng)};
  return CalcNext(x,random_exponential);
}

void ribi::bdm::Simulation::CalcNext(
  const double /* x */,
  const double random_exponential
)
{
  const auto dt = random_exponential * boost::units::si::second;
  m_time += dt;
  //Do something with a phylogeny here
  //Stub
}


int ribi::bdm::Simulation::CountExtant() const noexcept
{
  return 0; //Stub
}
