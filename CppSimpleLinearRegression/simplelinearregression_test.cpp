#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "simplelinearregression.h"

#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <sstream>

void ribi_SimpleLinearRegression_Test() noexcept
{
  {
    const std::vector<double> v { 75.0, 83.0, 96.0, 100.0, 121.0, 125.0 };
    const double variance { CalculateVariance(v) };
    const double expected { 332.666667 };
    assert(std::abs(variance - expected) < 0.0001);
  }
  {
    const std::vector<double> v { 0.23, 0.37, 0.45, 0.49, 0.56, 0.63, 0.63, 0.70, 0.72, 0.82 };
    const double variance { CalculateVariance(v) };
    const double expected { 0.02846 };
    assert(std::abs(variance - expected) < 0.0001);
  }
  for (int i=1; i!=5; ++i) //Human-based counting, following the Ansombe's Quartet indices
  {
    const std::vector<double> xs { GetAnscombesQuartetX(i) };
    const std::vector<double> ys { GetAnscombesQuartetY(i) };
    const std::pair<double,double> p { CalculateBestFit(xs,ys) };

    const double mean_x { CalculateMean(xs) };
    const double mean_y { CalculateMean(ys) };
    const double slope { p.first };
    const double intercept { p.second };

    //const double variance_x = CalculateVariance(xs);
    //const double variance_y = CalculateVariance(ys);


    const double expected_mean_x { 9.0 };
    const double expected_mean_y { 7.5 };     //to 2 decimal places
    const double expected_slope  { 0.500 };   //to 3 decimal places
    const double expected_intercept { 3.00 }; //to 2 decimal places

    const double e { 0.01 };
    assert(std::abs(expected_mean_x - mean_x) < e);
    assert(std::abs(expected_mean_y - mean_y) < e);

    //const double expected_variance_x = 11.0;
    //const double expected_variance_y = 4.125;        //4.122 or 4.127 (to 3 decimal places)
    //const double expected_correlation = 0.816;       //to 3 decimal places)
    //assert(std::abs(expected_variance_x - variance_x) < e);
    //assert(std::abs(expected_variance_y - variance_y) < e);
    assert(std::abs(expected_slope - slope) < e);
    assert(std::abs(expected_intercept - intercept) < e);
  }
}
