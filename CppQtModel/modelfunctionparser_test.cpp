#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "modelfunctionparser.h"

#include <cassert>
#include <stdexcept>

#include <boost/math/constants/constants.hpp>

//Warp's function parser
#include "fparser.hh"

#include "ribi_random.h"
#include "templocale.h"


#pragma GCC diagnostic pop

void ribi::ModelFunctionParserTest() noexcept
{
  //Set std::locale to English
  TempLocale temp_english_locale("en_US.UTF-8");

  //In case the decimal digits need to be non-Dutch
  {
    const double pi = boost::math::constants::pi<double>();
    assert(boost::lexical_cast<std::string>(pi)[1] == '.' && "No Dutch please");
  }

  {
    const ModelFunctionParser p("x * x * sin(x) * rand(x)","x");
    p.Evaluate(0.0);
  }
  try
  {
    std::string zero{boost::lexical_cast<std::string>(0.0)};
    const ModelFunctionParser p(zero,"x");
    assert(std::abs(p.Evaluate(0.0) - 0.0) < 0.0001);
  }
  catch (std::runtime_error&)
  {
    assert(!"Should not get here, '0.0' is a correct function");
  }
}
