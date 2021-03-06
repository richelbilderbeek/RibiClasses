#include "openfoamraspropertiesfile.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>





#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

#include <QFile>

#include "fileio.h"

#include "openfoamheader.h"





ribi::foam::RasPropertiesFile::RasPropertiesFile(
  const Header& header
)
  : m_header{header}
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::foam::Header ribi::foam::RasPropertiesFile::GetDefaultHeader() noexcept
{
  return Header("dictionary","constant","","RASProperties");
}

#ifndef NDEBUG
void ribi::foam::RasPropertiesFile::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const RasPropertiesFile& f) noexcept
{
  os
    << f.GetHeader() << '\n'
    << "" << '\n'
    << "RASModel realizableKE;\n"
    << "\n"
    << "turbulence on;\n"
    << "\n"
    << "printCoeffs on;\n"
  ;
  return os;
}
