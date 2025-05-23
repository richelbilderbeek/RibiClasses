//---------------------------------------------------------------------------
/*
RibiRandom, class for working with random numbers
Copyright (C) 2014-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppRibiRandom.htm
//---------------------------------------------------------------------------
#ifndef RIBI_RANDOM_H
#define RIBI_RANDOM_H





#include <memory>
#include <string>
#include <vector>


namespace ribi {

///Random functions
struct Random final
{
  ///Random seed
  Random();
  ///Use a given seed
  Random(const int seed);
  ~Random() noexcept;

  ///Obtain a random boolean
  bool GetBool() noexcept;

  ///Obtain a random lowercase character
  char GetChar() noexcept;

  ///Obtain a random number from zero to (and not including) one
  double GetFraction() noexcept;

  ///Obtain a random integer in range 'min' to and including 'max'
  int GetInt(const int min, const int max) noexcept;

  ///Obtain a random number from a normal distribution
  ///From http://www.richelbilderbeek.nl/CppGetRandomNormal.htm
  double GetNormal(const double mean = 0.0, const double sigma = 1.0) noexcept;

  ///Return a random string
  std::string GetString(const int length) noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:
  struct RandomImpl;
  const std::unique_ptr<RandomImpl> m_impl;
};

} //~namespace ribi

#endif // RIBI_RANDOM_H
