#include "openfoamdecomposepardictfile.h"

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





ribi::foam::DecomposeParDictFile::DecomposeParDictFile(
  const Header header
)
  : m_header{header}
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::foam::Header ribi::foam::DecomposeParDictFile::GetDefaultHeader() noexcept
{
  return Header("dictionary","system","","decomposeParDict");
}

#ifndef NDEBUG
void ribi::foam::DecomposeParDictFile::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const DecomposeParDictFile& f) noexcept
{
  os
    << f.GetHeader() << '\n'
    << "\n"
    << "numberOfSubdomains 4;\n"
    << "\n"
    << "method          	scotch;\n"
    << "\n"
    << "simpleCoeffs\n"
    << "{\n"
    << "    n			( 4 1 1 );\n"
    << "    delta		0.001;\n"
    << "}\n"
    << "\n"
    << "hierarchicalCoeffs\n"
    << "{\n"
    << "    n			( 1 1 1 );\n"
    << "    delta		0.001;\n"
    << "    order		xyz;\n"
    << "}\n"
    << "\n"
    << "metisCoeffs\n"
    << "{\n"
    << "    processorWeights	( 1 1 1 1 );\n"
    << "}\n"
    << "\n"
    << "manualCoeffs\n"
    << "{\n"
    << "    dataFile		"";\n"
    << "}\n"
    << "\n"
    << "distributed		no;\n"
    << "\n"
    << "roots			( );\n"
  ;
  return os;
}
