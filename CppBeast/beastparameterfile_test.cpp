#ifndef NDEBUG
#include "beastparameterfile.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QFile>

//TODO: Move FastaFile check to FastaFile::Test
#include "fastafile.h"
#include "testtimer.h"
#include "fileio.h"
#include "trace.h"

void ribi::BeastParameterFile::Test() noexcept
{
  {
    static bool is_tested {false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    fileio::FileIo();
  }
  
  const bool verbose{false};

  const ribi::fileio::FileIo fileio;
  const std::string beast_filename_birth_death_0_20150101{"birth_death_0_20150101.xml"};
  const std::string beast_filename_birth_death_1_20150101{"birth_death_1_20150101.xml"};
  const std::string beast_filename_birth_death_0_20151005{"birth_death_0_20151005.xml"};
  const std::string beast_filename_birth_death_1_20151005{"birth_death_1_20151005.xml"};
  const std::string coalescent_constant_population_0{"coalescent_constant_population_0.xml"};
  const std::string coalescent_constant_population_1{"coalescent_constant_population_1.xml"};
  const std::string fasta_filename_0{"test_output_0.fas"};
  const std::string fasta_filename_1{"test_output_1.fas"};
  const std::string log_file_birthdeath{"CppBeastLogFileBirthDeath.log"};
  const std::string log_file_coalescent{"CppBeastLogFileCoalescent.log"};
  const std::vector<std::string> resource_filenames
    =
    {
      beast_filename_birth_death_0_20150101,
      beast_filename_birth_death_1_20150101,
      beast_filename_birth_death_0_20151005,
      beast_filename_birth_death_1_20151005,
      coalescent_constant_population_0,
      coalescent_constant_population_1,
      fasta_filename_0,
      fasta_filename_1,
      log_file_birthdeath,
      log_file_coalescent
    };
  if (verbose) { TRACE("Create resources"); }
  {
    for (const auto& s: resource_filenames)
    {
      if (!fileio.IsRegularFile(s))
      {
        QFile((std::string(":/CppBeast/") + s).c_str()).copy(s.c_str());
      }
      if (!fileio.IsRegularFile(s))
      {
        std::cerr << "File missing: :/CppBeast/" << s << std::endl;
      }
      assert(fileio.IsRegularFile(s));
    }
  }
  if (verbose) { TRACE("Compare fasta_filename_0 and beast_filename_birth_death_0_20150101"); }
  {
    const FastaFile fasta_file(fasta_filename_0);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_0), //alignment_base_filename,
      10000000,
      TreePrior::birth_death,
      20150101
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(beast_filename_birth_death_0_20150101))
    {
      std::stringstream cmd; cmd << "diff -w " << temp_filename << " " << beast_filename_birth_death_0_20150101;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }
    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(beast_filename_birth_death_0_20150101)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Compare fasta_filename_1 and beast_filename_birth_death_1_20150101"); }
  {
    const FastaFile fasta_file(fasta_filename_1);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_1),
      10000000,
      TreePrior::birth_death,
      20150101
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(beast_filename_birth_death_1_20150101))
    {
      std::stringstream cmd; cmd << "diff -w " << temp_filename << " " << beast_filename_birth_death_1_20150101;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }
    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(beast_filename_birth_death_1_20150101)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Compare fasta_filename_0 and beast_filename_birth_death_0_20151005"); }
  {
    const FastaFile fasta_file(fasta_filename_0);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_0), //alignment_base_filename,
      10000000,
      TreePrior::birth_death,
      20151005
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(beast_filename_birth_death_0_20151005))
    {
      std::stringstream cmd; cmd << "diff -w " << temp_filename << " " << beast_filename_birth_death_0_20151005;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }
    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(beast_filename_birth_death_0_20151005)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Compare fasta_filename_1 and beast_filename_birth_death_1_20151005"); }
  {
    const FastaFile fasta_file(fasta_filename_1);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_1),
      10000000,
      TreePrior::birth_death,
      20151005
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(beast_filename_birth_death_1_20151005))
    {
      std::stringstream cmd; cmd << "diff -w " << temp_filename << " " << beast_filename_birth_death_1_20151005;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }
    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(beast_filename_birth_death_1_20151005)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Compare fasta_filename_0 and coalescent_constant_population"); }
  {
    const FastaFile fasta_file(fasta_filename_0);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_0),
      10000000,
      TreePrior::coalescent_constant_population,
      20150101
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(coalescent_constant_population_0))
    {
      std::stringstream cmd; cmd << "diff " << temp_filename << " " << coalescent_constant_population_0;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }
    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(coalescent_constant_population_0)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("Compare fasta_filename_1 and coalescent_constant_population"); }
  {
    const FastaFile fasta_file(fasta_filename_1);
    const BeastParameterFile beast_file(
      fasta_file.GetSequences(),
      fileio.GetFileBasename(fasta_filename_1),
      10000000,
      TreePrior::coalescent_constant_population,
      20150101
    );
    const std::string temp_filename{fileio.GetTempFileName(".txt")};
    {
      std::ofstream f(temp_filename.c_str());
      f << beast_file;
    }
    if (fileio.FileToVector(temp_filename) != fileio.FileToVector(coalescent_constant_population_1))
    {
      std::stringstream cmd; cmd << "diff " << temp_filename << " " << coalescent_constant_population_1;
      const int error{std::system(cmd.str().c_str())};
      if (error)
      {
        std::clog << __FILE__ << ": error " << error << '\n';
      }

    }
    assert(
      fileio.FileToVector(temp_filename) == fileio.FileToVector(coalescent_constant_population_1)
    );
    fileio.DeleteFile(temp_filename);
  }
  if (verbose) { TRACE("MakeDescriptionsUnique"); }
  {
    const DnaSequence s("not_unique","ATGC");
    const DnaSequence t("not_unique","ATGC");
    const std::vector<DnaSequence> v( {s,t} );
    const std::vector<DnaSequence> w{MakeDescriptionsUnique(v)};
    assert(w[0].GetDescription() != w[1].GetDescription());
  }
  if (verbose) { TRACE("Allow BEAST to make sequences unique"); }
  {
    const DnaSequence s("not_unique","ATGC");
    const DnaSequence t("not_unique","ATGC");
    const std::vector<ribi::DnaSequence> sequences_in( {s,t} );
    const BeastParameterFile beast_file(
      sequences_in,
      "some_unused_filename",
      1,
      TreePrior::coalescent_constant_population,
      20150101
    );
    const auto sequences_out = beast_file.GetSequences();
    assert(sequences_out.size() == 2);
    assert(sequences_out[0].GetDescription() != sequences_out[1].GetDescription());
  }
}
#endif

