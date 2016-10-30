#include "conceptmaphelper.h"
#include <boost/test/unit_test.hpp>

#include "ribi_regex.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_helper_test)
{
  using Regex = ribi::Regex;
  using namespace ribi::cmap;
  //GetRegexMatches
  {
    const std::string s = "In the Netherlands, 1234 AB and 2345 BC are valid zip codes";
    std::vector<std::string> expected;
    expected.push_back("1234 AB");
    expected.push_back("2345 BC");
    {
      const std::string r = "(\\d{4} [A-Z]{2})";
      BOOST_CHECK(Regex().GetRegexMatches(s,(r.c_str())) == expected);
    }
  }
  {
    const std::string s = "<concept><name>Concept with examples</name><example>Example 1</example><example>Example 2</example><example>Example 3</example></concept>";
    BOOST_CHECK(std::count(s.begin(),s.end(),'\b') == 0);
    std::vector<std::string> expected;
    expected.push_back("<example>Example 1</example>");
    expected.push_back("<example>Example 2</example>");
    expected.push_back("<example>Example 3</example>");
    {
      const std::string r = "(<example>.*?</example>)";
      BOOST_CHECK(Regex().GetRegexMatches(s,(r.c_str())) == expected);
    }
  }
  //GetCombinations
  //Assume the number of elements is correct
  BOOST_CHECK(GetCombinations(std::vector<int>( {         } ) ).size() ==  1);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1        } ) ).size() ==  2);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2      } ) ).size() ==  4);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3    } ) ).size() ==  8);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3,4  } ) ).size() == 16);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3,4,5} ) ).size() == 32);
  //Assume the elements are correct
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {1,2};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2,3 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {3};
    const std::vector<int> expected_4 = {1,2};
    const std::vector<int> expected_5 = {1,3};
    const std::vector<int> expected_6 = {2,3};
    const std::vector<int> expected_7 = {1,2,3};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_4));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_5));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_6));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_7));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2,3,4 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {3};
    const std::vector<int> expected_4 = {4};
    const std::vector<int> expected_5 = {1,2};
    const std::vector<int> expected_6 = {1,3};
    const std::vector<int> expected_7 = {1,4};
    const std::vector<int> expected_8 = {2,3};
    const std::vector<int> expected_9 = {2,4};
    const std::vector<int> expected_10 = {3,4};
    const std::vector<int> expected_11 = {1,2,3};
    const std::vector<int> expected_12 = {1,2,4};
    const std::vector<int> expected_13 = {1,3,4};
    const std::vector<int> expected_14 = {2,3,4};
    const std::vector<int> expected_15 = {1,2,3,4};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_4));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_5));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_6));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_7));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_8));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_9));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_10));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_11));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_12));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_13));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_14));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_15));
  }
  //Wordwrap
  {
    const auto v = {
      "",
      "1",
      "12",
      "1234",
      "12345678",
      "1234567890123456",
      "12345678901234567890123456789012",
      "123456789012345678901234567890123",
      "1234567890123456789012345678901234",
      "12345678901234567890123456789012345",
      "123456789012345678901234567890123456",
      "1234567890123456789012345678901234567",
      "12345678901234567890123456789012345678",
      "123456789012345678901234567890123456789",
      "1234567890123456789012345678901234567890",
      "1 1",
      "12 12",
      "123 123",
      "1234 1234",
      "12345 12345",
      "123456 123456",
      "1234567 1234567",
      "12345678 8",
      "123456789 9",
      "1234567890 0",
      "1234567890 1234567890",
      "1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
      " 1",
      "  1",
      "  1 ",
      "  1  ",
      "  1 2 ",
      "  1 23 ",
      "  12 34  ",
      "  12  34  ",
      "   12   34   ",
      "   12   34   5",
      "   12   34   5 ",
      "   12   34   5 6",
      "0   12   34   5 6",
      "0   12   34   5 6  ",
      "                    ",
      "                      ",
      "                        ",
      "                            ",
      "                                    ",
      "                                                    ",
      "                                                                                     "
    };
    for (int len=1; len!=200; ++len)
    {
      for (const std::string& s: v)
      {
        //Wordwrap calls Unwordwrap
        Wordwrap(s,len);
      }
    }
  }
}
