#include "openfoamkfile.h"

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





ribi::foam::KFile::KFile(
  const Header header
) : m_header(header)
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::foam::Header ribi::foam::KFile::GetDefaultHeader() noexcept
{
  return Header("dictionary","system","","K");
}

#ifndef NDEBUG
void ribi::foam::KFile::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const KFile& f) noexcept
{
  //No idea
  os
    << f.GetHeader() << '\n'
    << "" << '\n'
  ;
  return os;
}
