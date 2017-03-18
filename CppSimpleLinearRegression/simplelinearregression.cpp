#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "simplelinearregression.h"

#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <sstream>

#pragma GCC diagnostic pop

double ribi::SimpleLinearRegression::CalculateCovariance(
  const std::vector<double>& xs,
  const std::vector<double>& ys)
{
  assert(xs.size() == ys.size());
  const std::size_t sz { xs.size() };
  std::vector<double> ps(sz);
  for (std::size_t i=0; i!=sz; ++i)
  {
    ps[i] = xs[i] * ys[i];
  }

  const double mean_x { CalculateMean(xs) };
  const double mean_y { CalculateMean(ys) };
  const double mean_p { CalculateMean(ps) };
  return mean_p - (mean_x * mean_y);
}

double ribi::SimpleLinearRegression::CalculateVariance(
  const std::vector<double>& v)
{
  assert(!v.empty());
  const double mean { CalculateMean(v) };


  const double mean_of_squares {
    std::accumulate(v.begin(),v.end(),0.0,
      [](const double init, const double x)
      {
        return init + (x * x);
      }
    ) / static_cast<double>(v.size())
  };

  return mean_of_squares - (mean * mean);
}

std::vector<double> ribi::SimpleLinearRegression::GetAnscombesQuartetX(const int index)
{
  switch (index)
  {
    case 1:
    case 2:
    case 3:
      return { 10.0,8.0,13.0,9.0,11.0,14.0,6.0,4.0,12.0,7.0,5.0 };
    case 4:
      return { 8.0,8.0,8.0,8.0,8.0,8.0,8.0,19.0,8.0,8.0,8.0 };
    default:
    {
      std::stringstream s;
      s << "ERROR ("
        << __func__
        << "): Anscombe's Quartet has index 1 to and including 4, given index is: "
        << index;
      std::cerr << s.str() << std::endl;
      assert(!"Should not get here: ");
      throw std::logic_error(s.str());
    }
  }
}

std::vector<double>
  ribi::SimpleLinearRegression::GetAnscombesQuartetY(const int index)
{
  switch (index)
  {
    case 1:
      return { 8.04,6.95,7.58,8.81,8.33,9.96,7.24,4.26,10.84,4.82,5.68 };
    case 2:
      return { 9.14,8.14,8.74,8.77,9.26,8.1,6.13,3.1,9.13,7.26,4.74 };
    case 3:
      return { 7.46,6.77,12.74,7.11,7.81,8.84,6.08,5.39,8.15,6.42,5.73 };
    case 4:
      return { 6.58,5.76,7.71,8.84,8.47,7.04,5.25,12.5,5.56,7.91,6.89 };
    default:
    {
      std::stringstream s;
      s << "ERROR ("
        << __func__
        << "): Anscombe's Quartet has index 1 to and including 4, given index is: "
        << index;
      std::cerr << s.str() << std::endl;
      assert(!"Should not get here: ");
      throw std::logic_error(s.str());
    }
  }
}

std::string ribi::SimpleLinearRegression::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::SimpleLinearRegression::GetVersionHistory() noexcept
{
  std::vector<std::string> v {
    "2013-08-27: version 1.0: initial version, as ToolTestSimpleLinearRegressionMainDialog",
    "2013-08-28: version 1.1: renamed to SimpleLinearRegression, templated CalculateBestFit",
  };
  return v;
}
