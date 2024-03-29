#ifndef BIRTHDEATHMODELPROCESS_H
#define BIRTHDEATHMODELPROCESS_H

#include "birthdeathmodelparameters.h"
#include <random>

namespace ribi {
namespace bdm {

///Performs a Brownian motion
struct Simulation
{
  using BirthRate = ribi::units::Rate;
  using DeathRate = ribi::units::Rate;
  using Time = ribi::units::Time;

  Simulation(
    const Parameters& parameters
  );

  ///Calculate the next x. This class will supply the random numbers,
  ///thus these member functions are not const
  void CalcNext(
    const double x
  );

  ///Calculate the next x+dt, supplying the random numbers yourself
  void CalcNext(
    const double x,
    const double random_exponential
  );

  int CountExtant() const noexcept;

  private:
  std::exponential_distribution<double> m_exponential_distribution;
  const Parameters m_parameters;
  std::mt19937 m_rng;
  Time m_time;
  std::uniform_real_distribution<double> m_uniform_distribution;
};

} //~namespace bm
} //~namespace ribi

#endif // BIRTHDEATHMODELPROCESS_H
