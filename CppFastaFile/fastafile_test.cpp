#ifndef NDEBUG
#include "fastafile.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "container.h"
#include "dnasequence.h"
#include "fileio.h"
#include "testtimer.h"
#include "trace.h"
#include <QFile>

void ribi::FastaFile::Test() noexcept
{
  {
    static bool is_tested {false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    fileio::FileIo();
    DnaSequence("no_description","ATGC");
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  if (verbose) { TRACE("One sequence"); }
  {
    const std::vector<DnaSequence> alignments{
      DnaSequence("my_a","A")
    };
    const FastaFile file(alignments);
    std::stringstream s;
    s << file;
    assert(s.str() == ">my_a\nA\n");
  }
  if (verbose) { TRACE("Two sequences"); }
  {
    std::vector<DnaSequence> sequences{
      DnaSequence("my_a","A"),DnaSequence("my_b","C")
    };
    FastaFile file(sequences);
    std::stringstream s;
    s << file;
    assert(s.str() == ">my_a\nA\n>my_b\nC\n");
  }
  if (verbose) { TRACE("File I/O"); }
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
    assert(file.GetSequences() == sequences);
    fileio::FileIo().DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Test case #0"); }
  {
    const std::string temp_filename{fileio::FileIo().GetTempFileName(".fasta")};
    QFile file(":/fasta_file/files/fasta_file_test_0.fasta");
    file.copy(temp_filename.c_str());
    fileio::FileIo().IsRegularFile(temp_filename);
    const FastaFile fasta_file(temp_filename);
    assert(fasta_file.GetSequences().size() == 8);
    const auto first_dna_sequence = fasta_file.GetSequences()[0];
    assert(first_dna_sequence.GetDescription() == "Outgroup");
    const std::string expected_first_dna_sequence{
      Container().ToUpper(
        "ctacgattcgcatcacaatttgtcaaatactttgacgacggcagcaatttggaaccggaaggcaaaagaactcgtagcgaccttgttgcgactacgcaag"
      )
    };
    assert(first_dna_sequence.GetSequence() == expected_first_dna_sequence);
    fileio::FileIo().DeleteFile(temp_filename);
  }
}
#endif

