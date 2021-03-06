//---------------------------------------------------------------------------
/*
TempLocale, class for a temporary std::locale
Copyright (C) 2015-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppTempLocale.htm
//---------------------------------------------------------------------------
#include "templocale.h"

#include <cassert>
#include <locale>

struct ribi::TempLocale::TempLocaleImpl
{
  explicit TempLocaleImpl(
    const std::string& temp_locale_name
  )
    : m_prev_locale_name{std::locale().name()},
      m_temp_locale_name{temp_locale_name}
  {
    //Set the new locale
    std::locale::global(std::locale(m_temp_locale_name.c_str()));
  }
  ~TempLocaleImpl() noexcept
  {
    //Restore the old locale
    std::locale::global(std::locale(m_prev_locale_name.c_str()));

  }
  ///The previous std::locale its name (before TempLocale became active)
  const std::string m_prev_locale_name;
  ///The current/temporary std::locale its name
  const std::string m_temp_locale_name;
};

ribi::TempLocale::TempLocale(
    const std::string& temp_locale_name
)
#if __cplusplus >= 201402L //C++17
  : m_impl(std::make_unique<TempLocaleImpl>(temp_locale_name))
#else
  : m_impl{new TempLocaleImpl(temp_locale_name)}
#endif
{

}

ribi::TempLocale::~TempLocale() noexcept
{

}

std::string ribi::TempLocale::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::TempLocale::GetVersionHistory() noexcept
{
  return {
    "2015-01-04: version 1.0: initial version"
  };
}

