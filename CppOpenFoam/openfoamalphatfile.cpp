#include "openfoamalphatfile.h"

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





ribi::foam::AlphatFile::AlphatFile(
  const Header header
) noexcept
  : m_boundary_field{},
    m_dimensions{ {} },
    m_header{header},
    m_internal_field{}
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::foam::Header ribi::foam::AlphatFile::GetDefaultHeader() noexcept
{
  return Header("volScalarField","0","","alphat");
}

#ifndef NDEBUG
void ribi::foam::AlphatFile::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const AlphatFile& f) noexcept
{
  os
    << f.GetHeader() << '\n'
    << "" << '\n'
    << "" << '\n'
    << "dimensions ["
      << f.m_dimensions[0] << " "
      << f.m_dimensions[1] << " "
      << f.m_dimensions[2] << " "
      << f.m_dimensions[3] << " "
      << f.m_dimensions[4] << " "
      << f.m_dimensions[5] << " "
      << f.m_dimensions[6]
      << "];" << '\n'
    << "" << '\n'
    << "internalField " << f.m_internal_field << ";\n"
    << "" << '\n'
    << "boundaryField" << '\n'
    << "{" << '\n'
    << f.m_boundary_field << '\n'
    << "}" << '\n'
  ;
  return os;
}
