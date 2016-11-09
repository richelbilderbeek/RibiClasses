#include "templocale.h"

void ribi::TempLocaleTest() noexcept
{
  {
    const double x = 1.5;
    const TempLocale temp_dutch_locale("nl_NL.UTF-8");
    assert(std::to_string(x)[1] == ',' && "Dutch");
  }
  {
    const double x = 1.5;
    const TempLocale temp_english_locale("en_US.UTF-8");
    assert(std::to_string(x)[1] == '.' && "English");
  }
  {
    const double x = 1.5;
    const TempLocale temp_dutch_locale("nl_NL.UTF-8");
    assert(std::to_string(x)[1] == ',' && "Dutch");
  }
  {
    const double x = 1.5;
    const TempLocale temp_english_locale("en_US.UTF-8");
    assert(std::to_string(x)[1] == '.' && "English");
  }
}
