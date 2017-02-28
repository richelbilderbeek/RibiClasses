#include "fastafile.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "container.h"
#include "dnasequence.h"
#include "fileio.h"
#include <QFile>

#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(fasta)
{
  // "One sequence"); }
  {
    const std::vector<DnaSequence> alignments{
      DnaSequence("my_a","A")
    };
    const FastaFile file(alignments);
    std::stringstream s;
    s << file;
    BOOST_CHECK(s.str() == ">my_a\nA\n");
  }
  // "Two sequences"); }
  {
    std::vector<DnaSequence> sequences{
      DnaSequence("my_a","A"),DnaSequence("my_b","C")
    };
    FastaFile file(sequences);
    std::stringstream s;
    s << file;
    BOOST_CHECK(s.str() == ">my_a\nA\n>my_b\nC\n");
  }
  // "File I/O"); }
  {
    std::vector<DnaSequence> sequences{
      DnaSequence("my_a","A"),DnaSequence("my_b","C")
    };
    //Write to disc
    const std::string temp_filename{fileio::FileIo().GetTempFileName(".fasta")};
    {
      const FastaFile fasta_file(sequences);
      std::ofstream f(temp_filename.c_str());
      f << fasta_file;
    }
    //Read from disc
    const FastaFile file(temp_filename);
    BOOST_CHECK(file.GetSequences() == sequences);
    fileio::FileIo().DeleteFile(temp_filename);
  }
  // "Test case #0"); }
  {
    const std::string temp_filename{fileio::FileIo().GetTempFileName(".fasta")};
    QFile file(":/fasta_file/files/fasta_file_test_0.fasta");
    file.copy(temp_filename.c_str());
    fileio::FileIo().IsRegularFile(temp_filename);
    const FastaFile fasta_file(temp_filename);
    assert(fasta_file.GetSequences().size() == 8);
    const auto first_dna_sequence = fasta_file.GetSequences()[0];
    BOOST_CHECK(first_dna_sequence.GetDescription() == "Outgroup");
    const std::string expected_first_dna_sequence{
      Container().ToUpper(
        "ctacgattcgcatcacaatttgtcaaatactttgacgacggcagcaatttggaaccggaaggcaaaagaactcgtagcgaccttgttgcgactacgcaag"
      )
    };
    BOOST_CHECK(first_dna_sequence.GetSequence() == expected_first_dna_sequence);
    fileio::FileIo().DeleteFile(temp_filename);
  }
}
