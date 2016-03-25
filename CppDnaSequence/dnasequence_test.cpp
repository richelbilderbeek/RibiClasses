#include "dnasequence.h"
#include <boost/test/unit_test.hpp>


#include <stdexcept>

#include "container.h"
#include "testtimer.h"

BOOST_AUTO_TEST_CASE(dna_sequence_test)
{
  using DnaSequence = ribi::DnaSequence;
  //Recovery of parameters
  {
    const std::string description{"description"};
    const std::string sequence{"ACGT"};
    const DnaSequence s(description,sequence);
    BOOST_CHECK(s.GetDescription() == description);
    BOOST_CHECK(s.GetSequence() == sequence);
  }
  //operator==
  {
    const std::string description{"description"};
    const std::string sequence{"ACGT"};
    const DnaSequence s(description,sequence);
    const DnaSequence t(description,sequence);
    BOOST_CHECK(s == t);
  }
  //operator!= on different description
  {
    const std::string sequence{"ACGT"};
    const DnaSequence s("description1",sequence);
    const DnaSequence t("description2",sequence);
    BOOST_CHECK(s != t);
  }
  //operator!= on different sequence
  {
    const std::string description{"description"};
    const DnaSequence s(description,"ACGT");
    const DnaSequence t(description,"ACGTT");
    BOOST_CHECK(s != t);
  }
  {
    BOOST_CHECK_THROW(
      DnaSequence("description","incorrect_sequence"),
      std::invalid_argument
    );
  }
  {
    BOOST_CHECK(ribi::CleanSequence("a g c") == "AGC");
  }
}
