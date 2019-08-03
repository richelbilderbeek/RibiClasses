

#include "newick.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <functional>
#include <map>
#include <numeric>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>


#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "BigIntegerLibrary.hh"

#include "newickcpp98.h"



void ribi_Newick_Test()
{
  //#ifdef __GXX_EXPERIMENTAL_CXX0X__
  ///\note
  ///The tests below must be put back in again once
  #ifdef DEBUG_TEMP_REMOVE_2738236826438


  // Testing basic Newick functionality");
  //Check difference between C++98 and C++0x
  assert(Newick::CreateValidTrinaryNewicks() == NewickCpp98().CreateValidTrinaryNewicks());
  assert(Newick::GetKnownProbabilities() == NewickCpp98().GetKnownProbabilities());



  //Check conversions from std::string to std::vector #1
  {
    const std::vector<int> v = Newick::StringToNewick("(11,(22,33))");
    assert(v.size() == 7);
    assert(v[0]==Newick::bracket_open);
    assert(v[1]==11);
    assert(v[2]==Newick::bracket_open);
    assert(v[3]==22);
    assert(v[4]==33);
    assert(v[5]==Newick::bracket_close);
    assert(v[6]==Newick::bracket_close);
  }
  //Check if well-formed Newicks are accepted
  {
    const std::vector<std::string> v = Newick::CreateValidNewicks();
    for(const std::string& s: v)
    {
      assert(Newick::IsNewick(s));
      const std::vector<int> v = Newick::StringToNewick(s);
      assert(Newick::IsNewick(v));
    }
  }
  //Check if ill-formed Newicks are rejected
  {
    #ifndef NDEBUG
    const std::vector<std::string> v = Newick::CreateInvalidNewicks();
    for(const std::string& s: v)
    {
      assert(!Newick::IsNewick(s));
      //Cannot test if std::vector<int> versions are rejected,
      //because Newick::StringToNewick assumes a valid Newick
      //const std::vector<int> v = Newick::StringToNewick(s);
      //assert(!Newick::IsNewick(v));
    }
    #endif
  }
  //Check conversions from std::string to std::vector #2
  {
    const std::vector<int> v = Newick::StringToNewick("((11,22),33)");
    assert(v.size() == 7);
    assert(v[0]==Newick::bracket_open);
    assert(v[1]==Newick::bracket_open);
    assert(v[2]==11);
    assert(v[3]==22);
    assert(v[4]==Newick::bracket_close);
    assert(v[5]==33);
    assert(v[6]==Newick::bracket_close);
  }
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(1,(3,1))"))==0);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(3,(1,1))"))==1);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(1,((1,1),(1,1)))"))==3);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(1,((1,1),(2,2)))"))==2);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(1,(2,3))"))==0);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(99,99)"))==1);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(3,(2,2))"))==1);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(2,(2,2))"))==1);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("((3,3),(2,2))"))==2);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("((3,3),(3,3))"))==3);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("((3,3),(3,4))"))==1);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((3,3),(4,4)),5)"))==2);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((3,3),(5,5)),5)"))==2);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((5,5),(5,5)),5)"))==3);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((5,5),(5,5)),(4,4))"))==4);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((5,5),(4,4)),(4,4))"))==3);
  assert(Newick::CalcNumOfSymmetriesBinary(StringToNewick("(((4,4),(4,4)),(4,4))"))==4);
  assert(Newick::CalcNumOfCombinationsBinary(StringToNewick("(3,(1,1))"))==10);
  assert(Newick::CalcNumOfCombinationsBinary(StringToNewick("(1,(3,1))"))==20);
  assert(Newick::CalcNumOfCombinationsBinary(StringToNewick("(1,(1,(1,(1,1))))"))==60);
  assert(Newick::CalcNumOfCombinationsBinary(StringToNewick("(1,((1,1),(1,1)))"))==15);
  assert(bigIntegerToString(Newick::FactorialBigInt(1))=="1");
  assert(bigIntegerToString(Newick::FactorialBigInt(2))=="2");
  assert(bigIntegerToString(Newick::FactorialBigInt(3))=="6");
  assert(bigIntegerToString(Newick::FactorialBigInt(4))=="24");
  assert(bigIntegerToString(Newick::FactorialBigInt(5))=="120");
  assert(bigIntegerToString(Newick::FactorialBigInt(6))=="720");
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1)"))   == 1);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(12)"))  == 1);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(123)")) == 1);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,2)"))   == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(12,2)"))  == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(123,2)")) == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(1,2))"))   == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(12,2))"))  == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(123,2))")) == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((1,2),3)"))   == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((12,2),3)"))  == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((123,2),3)")) == 2);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,2,3)"))   == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(12,2,3)"))  == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(123,2,3)")) == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(1,2,3))"))   == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(12,2,3))"))  == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("(1,(123,2,3))")) == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((1,2,3),4)"))   == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((12,2,3),4)"))  == 3);
  assert(Newick::GetLeafMaxArity(Newick::StringToNewick("((123,2,3),4)")) == 3);

  assert(fuzzy_equal_to()(  2.0,Newick::CalcDenominator(Newick::StringToNewick("(1,1)"),10.0)));
  assert(fuzzy_equal_to()(  6.0,Newick::CalcDenominator(Newick::StringToNewick("((1,1),1)"),10.0)));
  assert(fuzzy_equal_to()( 26.0,Newick::CalcDenominator(Newick::StringToNewick("(1,2)"),10.0)));
  assert(fuzzy_equal_to()( 32.0,Newick::CalcDenominator(Newick::StringToNewick("((1,1),2)"),10.0)));
  assert(fuzzy_equal_to()( 32.0,Newick::CalcDenominator(Newick::StringToNewick("(2,(1,1))"),10.0)));
  assert(fuzzy_equal_to()( 50.0,Newick::CalcDenominator(Newick::StringToNewick("((1,1),3)"),10.0)));
  assert(fuzzy_equal_to()( 80.0,Newick::CalcDenominator(Newick::StringToNewick("((1,2),3)"),10.0)));
  assert(fuzzy_equal_to()( 80.0,Newick::CalcDenominator(Newick::StringToNewick("((3,1),2)"),10.0)));
  assert(fuzzy_equal_to()( 80.0,Newick::CalcDenominator(Newick::StringToNewick("((2,3),1)"),10.0)));
  assert(fuzzy_equal_to()(102.0,Newick::CalcDenominator(Newick::StringToNewick("((2,1),4)"),10.0)));
  assert(fuzzy_equal_to()(152.0,Newick::CalcDenominator(Newick::StringToNewick("(2,(1,(3,3)))"),10.0)));
  assert(fuzzy_equal_to()(162.0,Newick::CalcDenominator(Newick::StringToNewick("((2,3),4)"),10.0)));
  assert(fuzzy_equal_to()(180.0,Newick::CalcDenominator(Newick::StringToNewick("((1,2),(3,4))"),10.0)));
  assert(fuzzy_equal_to()(180.0,Newick::CalcDenominator(Newick::StringToNewick("((4,1),(2,3))"),10.0)));
  assert(fuzzy_equal_to()(180.0,Newick::CalcDenominator(Newick::StringToNewick("((3,4),(1,2))"),10.0)));
  assert(fuzzy_equal_to()(180.0,Newick::CalcDenominator(Newick::StringToNewick("((2,3),(4,1))"),10.0)));
  {
    const std::vector<int> v = { 0,1,2,3,4,5,6 };
    assert(Newick::FindPosAfter(v,3,2)==3);
    assert(Newick::FindPosAfter(v,4,2)==4);
    assert(Newick::FindPosAfter(v,5,2)==5);
    assert(Newick::FindPosAfter(v,6,2)==6);
    assert(Newick::FindPosBefore(v,3,4)==3);
    assert(Newick::FindPosBefore(v,2,4)==2);
    assert(Newick::FindPosBefore(v,1,4)==1);
    assert(Newick::FindPosBefore(v,0,4)==0);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("(1,(2,2))"));
    const std::vector<int> w = Newick::GetDepth(Newick::StringToNewick("(9,(9,9))"));
    const std::vector<int> x = { 0,0,1,1,1,1,0 };
    assert(v == x);
    assert(w == x);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("((2,2),1)"));
    const std::vector<int> w = { 0,1,1,1,1,0,0 };
    assert(v == w);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("(1,(2,2),1)"));
    const std::vector<int> w = { 0,0,1,1,1,1,0,0 };
    assert(v == w);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("(1,(2,3),4,(5,6))"));
    const std::vector<int> w = { 0,0,1,1,1,1,0,1,1,1,1,0 };
    assert(v == w);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("(1,(2,3),(5,6))"));
    const std::vector<int> w = { 0,0,1,1,1,1,1,1,1,1,0 };
    assert(v == w);
  }
  {
    const std::vector<int> v = Newick::GetDepth(Newick::StringToNewick("(1,(2,(3,4)),((5,6),7))"));
    const std::vector<int> w = { 0,0,1,1,2,2,2,2,1,1,2,2,2,2,1,1,0 };
    assert(v == w);
  }
  //Test GetRootBranches
  {
    const std::vector<std::vector<int> > v = Newick::GetRootBranches(Newick::StringToNewick("(1,2)"));
    assert(v.size() == 2);
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(1)")) != v.end());
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(2)")) != v.end());
  }
  {
    const std::vector<std::vector<int> > v = Newick::GetRootBranches(Newick::StringToNewick("(1,(2,3))"));
    assert(v.size() == 2);
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(1)")) != v.end());
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(2,3)")) != v.end());
  }
  {
    const std::vector<std::vector<int> > v = Newick::GetRootBranches(Newick::StringToNewick("(1,2,(3,4))"));
    assert(v.size() == 3);
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(1)")) != v.end());
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(2)")) != v.end());
    assert(std::find(v.begin(),v.end(),
      Newick::StringToNewick("(3,4)")) != v.end());
  }
  //Compare C++98 and C++0x version
  {
    const std::vector<std::string> v = Newick::CreateValidBinaryNewicks();
    for(const std::string& s: v)
    {
      const std::vector<int> n = Newick::StringToNewick(s);
      assert(Newick::GetRootBranches(n) == NewickCpp98().GetRootBranches(n));
    }
  }

  //Check if binary and trinary Newicks are detected correctly
  {
    const std::vector<std::string> v = Newick::CreateValidBinaryNewicks();
    for(const std::string& s: v)
    {
      const std::vector<int> n = Newick::StringToNewick(s);
      assert(Newick::IsBinaryNewick(n));
    }
  }
  //Check if unary Newicks are detected correctly
  {
    const std::vector<std::string> v = Newick::CreateValidUnaryNewicks();
    for(const std::string& s: v)
    {
      const std::vector<int> n = Newick::StringToNewick(s);
      assert( Newick::GetLeafMaxArity(n)<=1);
      assert( Newick::IsUnaryNewick(n));
      assert(!Newick::IsBinaryNewick(n));
      assert(!Newick::IsTrinaryNewick(n));
    }
  }
  //Check if binary Newicks are detected correctly
  {
    const std::vector<std::string> v = Newick::CreateValidBinaryNewicks();
    for(const std::string& s: v)
    {
      const std::vector<int> n = Newick::StringToNewick(s);
      assert( Newick::GetLeafMaxArity(n)<=2);
      assert(!Newick::IsUnaryNewick(n));
      assert( Newick::IsBinaryNewick(n));
      assert(!Newick::IsTrinaryNewick(n));
    }
  }
  //Check if trinary Newicks are detected correctly
  {
    const std::vector<std::string> v = Newick::CreateValidTrinaryNewicks();
    for(const std::string& s: v)
    {
      const std::vector<int> n = Newick::StringToNewick(s);
      assert( Newick::GetLeafMaxArity(n)<=3);
      assert(!Newick::IsUnaryNewick(n));
      assert(!Newick::IsBinaryNewick(n));
      assert( Newick::IsTrinaryNewick(n));
    }
  }
  //Test binary Newick
  {
    const std::string s("(1,(2,3))");
    const std::vector<std::vector<int> > n = GetSimplerNewicks(Newick::StringToNewick(s));
    assert(n.size() == 2);
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(1,3))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(2,2))"))
      != n.end());
  }
  {
    const std::string s("(1,(2,3,4))");
    const std::vector<std::vector<int> > n = GetSimplerNewicks(Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(1,3,4))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(2,2,4))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(2,3,3))"))
      != n.end());
  }
  {
    const std::string s("(1,(1,3,4))");
    const std::vector<std::vector<int> > n = GetSimplerNewicks(Newick::StringToNewick(s));
    assert(n.size() == 4);
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(4,4))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(3,5))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(1,2,4))"))
      != n.end());
    assert(std::find(n.begin(),n.end(),Newick::StringToNewick("(1,(1,3,3))"))
      != n.end());
  }
  {
    const std::string s("(1,(1,3,4))");
    const std::vector<std::pair<std::vector<int>,int> > n
      = GetSimplerNewicksFrequencyPairs(Newick::StringToNewick(s));
    typedef std::pair<std::vector<int>,int> Pair;
    assert(n.size() == 4);
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(1,(4,4))"),1))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(1,(3,5))"),1))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(1,(1,2,4))"),3))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(1,(1,3,3))"),4))
      != n.end());
  }
  {
    const std::string s("((1,1),2)");
    const std::vector<std::vector<int> > n = Newick::GetSimplerNewicks(
      Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("(2,2)"))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((1,1),1)"))
      != n.end());
  }
  {
    const std::string s("((1,1),2)");
    typedef std::pair<std::vector<int>,int> Pair;
    const std::vector<Pair> n
      = GetSimplerNewicksFrequencyPairs(Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(2,2)"),1))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((1,1),1)"),2))
      != n.end());
  }
  {
    const std::string s("((2,1),4)");
    const std::vector<std::vector<int> > n = Newick::GetSimplerNewicks(
      Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("(3,4)"))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((1,1),4)"))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((2,1),3)"))
      != n.end());
  }
  {
    const std::string s("((2,1),4)");
    typedef std::pair<std::vector<int>,int> Pair;
    const std::vector<Pair> n
      = GetSimplerNewicksFrequencyPairs(Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("(3,4)"),1))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((1,1),4)"),2))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((2,1),3)"),4))
      != n.end());
  }
  {
    const std::string s("((2,3),4)");
    const std::vector<std::vector<int> > n = Newick::GetSimplerNewicks(
      Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((1,3),4)"))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((2,2),4)"))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      Newick::StringToNewick("((2,3),3)"))
      != n.end());
  }
  {
    const std::string s("((2,3),4)");
    typedef std::pair<std::vector<int>,int> Pair;
    const std::vector<Pair> n
      = GetSimplerNewicksFrequencyPairs(Newick::StringToNewick(s));
    assert(n.size() == 3);
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((1,3),4)"),2))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((2,2),4)"),3))
      != n.end());
    assert(std::find(n.begin(),n.end(),
      std::make_pair(Newick::StringToNewick("((2,3),3)"),4))
      != n.end());
  }
  //Compare GetSimplerNewicks and
  //GetSimplerNewicksFrequencyPairs
  {
    const std::vector<std::string> newicks
      = Newick::CreateValidNewicks();
    for(const std::string& newick_str: newicks)
    {
      const std::vector<int> newick
        = Newick::StringToNewick(newick_str);
      const std::vector<std::vector<int> > v1
        = Newick::GetSimplerNewicks(newick);
      const std::vector<std::pair<std::vector<int>,int> > v2
        = Newick::GetSimplerNewicksFrequencyPairs(newick);
      assert(v1.size() == v2.size());
      const int size = boost::numeric_cast<int>(v1.size());
      for (int i=0; i!=size; ++i)
      {
        assert(v1[i] == v2[i].first);
      }
      assert(Newick::GetSimplerNewicksFrequencyPairs(newick)
        == NewickCpp98().GetSimplerNewicksFrequencyPairs(newick));
    }
  }
  #endif
}
