#include "fastafile.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <fstream>
#include <sstream>

#include "dnasequence.h"
#include "fileio.h"
//#include "helper.h"

ribi::FastaFile::FastaFile(
  const std::vector<ribi::DnaSequence>& sequences
) : m_sequences{sequences}
{

}

std::string ribi::FastaFile::GetVersion() const noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::FastaFile::GetVersionHistory() const noexcept
{
  return {
    "2015-0?-??: Version 1.0: initial version",
    "2015-10-06: Version 1.1: started versioning, FastaFile can read multi-line fasta files"
  };
}

std::vector<ribi::DnaSequence> ribi::FastaFile::ReadSequencesFromFile(const std::string& filename)
{
  std::vector<std::string> text{
    fileio::FileIo().FileToVector(filename)
  };
  std::vector<DnaSequence> sequences;
  std::string current_description = "";
  std::string current_sequence = "";

  for (const auto& line: text)
  {
    if (line.empty()) continue;
    //Description starting?
    if (line[0] == '>') {
      if (current_description != "") {
        //Store old descriptions, start a new one
        sequences.push_back(DnaSequence(current_description,current_sequence));
      }
      current_description = line.substr(1,line.size()-1);
      current_sequence = "";
      continue;
    }
    else
    {
      //DnaSequence will clean up the whitespace
      current_sequence += line;
    }
  }
  sequences.push_back(DnaSequence(current_description,current_sequence));
  return sequences;
}


std::ostream& ribi::operator<<(std::ostream& os, const FastaFile file)
{
  for (const auto& p: file.GetSequences())
  {
    os
      << '>' << p.GetDescription() << '\n'
      << p.GetSequence() << '\n'
    ;
  }
  return os;
}
