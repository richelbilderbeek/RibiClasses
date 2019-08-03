


#include "newickvector.h"

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

#include <boost/numeric/conversion/cast.hpp>

#include <boost/lexical_cast.hpp>

#include "BigIntegerLibrary.hh"

#include "fuzzy_equal_to.h"
#include "newick.h"




void ribi_NewickVector_Test() noexcept
{
  //Test simplification of trinary Newick
  {
    //Simple trinary Newick
    {
      const std::string s = "(2,3,4)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 3);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(1,3,4)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(2,2,4)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(2,3,3)")) != ns.end());
    }
    //Simple trinary Newick
    {
      const std::string s = "(1,2,3)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 4);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(3,3)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(2,4)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(1,1,3)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(1,2,2)")) != ns.end());
    }
    //Complex trinary Newick
    {
      const std::string s = "((2,3),(4,5),(6,7))";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 6);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,3),(4,5),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,2),(4,5),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(3,5),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(4,4),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(4,5),(5,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(4,5),(6,6))")) != ns.end());
    }
    //Another trinary Newick
    {
      const std::string s = "((2,3),(1,5),(6,7))";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 6);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,3),(1,5),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,2),(1,5),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),6,(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(1,4),(6,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(1,5),(5,7))")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,3),(1,5),(6,6))")) != ns.end());
    }
    //Another trinary Newick
    {
      const std::string s = "((1,2,3),(4,5,6),1)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 7);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((3,3),(4,5,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((2,4),(4,5,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,1,3),(4,5,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,2,2),(4,5,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,2,3),(3,5,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,2,3),(4,4,6),1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,2,3),(4,5,5),1)")) != ns.end());
    }
    //Another trinary Newick
    {
      const std::string s = "((1,(1,1,1)),1)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 6);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,(2,1)),1)")) != ns.end());
    }
    //Another trinary Newick
    {
      const std::string s = "(1,(1,1),1)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 4);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(1,2,1)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,1),2)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("(2,(1,1))")) != ns.end());
    }
    //Another trinary Newick
    {
      const std::string s = "((1,(2,3,4)),5)";
      const std::vector<int> n = Newick().StringToNewick(s);
      assert(Newick().IsTrinaryNewick(n));
      const std::vector<std::vector<int> > ns = Newick().GetSimplerNewicks(n);
      assert(ns.size() == 4);
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,(1,3,4)),5)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,(2,2,4)),5)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,(2,3,3)),5)")) != ns.end());
      assert(std::find(ns.begin(),ns.end(),
        Newick().StringToNewick("((1,(2,3,4)),4)")) != ns.end());
    }
  }

  //Check that well-formed Newicks are confirmed valid
  {
    const std::vector<std::string> v = Newick().CreateValidNewicks();
    for(const std::string& s: v)
    {
      //Check if valid newicks (as std::string) are marked as valid
      try
      {
        Newick().CheckNewick(s);
      }
      catch (std::exception& e)
      {
        std::cerr << "(" << __FILE__ << "," << __LINE__ << ") "
          << s << ": " << e.what() << '\n'
        ;
      }
      //Check if valid newicks (as std::vector) are marked as valid
      try
      {
        const std::vector<int> n = Newick().StringToNewick(s);
        Newick().CheckNewick(n);
        assert(Newick().IsNewick(n));
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (converted to std::vector<int>): "
          << e.what()
        ;
      }
      //Check std::string conversion (from NewickVector(std::string))
      try
      {
        NewickVector temp(s);
        assert(s == temp.ToStr());
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (NewickVector from std::string): "
          << e.what()
        ;
      }
      //Check std::string conversion (from NewickVector(std::vector<int>))
      try
      {
        const std::vector<int> n = Newick().StringToNewick(s);
        NewickVector temp(n);
        assert(s == temp.ToStr());
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (NewickVector from std::vector<int>): "
          << e.what()
        ;
      }
      assert(Newick().IsNewick(s));
      //Check the simpler Newicks
      {
        const std::vector<std::vector<int> > simpler
          = Newick().GetSimplerNewicks(
            Newick().StringToNewick(s));
        for(const std::vector<int> simple: simpler)
        {
          assert(Newick().IsNewick(simple));
          Newick().CheckNewick(simple);
        }
      }
      //Check the branches
      #ifndef NDEBUG
      if (!Newick().IsUnaryNewick(Newick().StringToNewick(s)))
      {
        const std::vector<std::vector<int> > b
            = Newick().GetRootBranches(
              Newick().StringToNewick(s));
        for(const std::vector<int>& c: b)
        {
          assert(Newick().IsNewick(c));
        }
      }
      #endif
    }
  }
  //Check if ill-formed Newicks are rejected
  {
    const std::vector<std::string> v = Newick().CreateInvalidNewicks();
    for(const std::string& s: v)
    {
      assert(!Newick().IsNewick(s));
    }
  }

  //Check that well-formed Newicks are confirmed valid
  {
    const std::vector<std::string> v = Newick().CreateValidNewicks();
    for(const std::string& s: v)
    {
      //Check if valid newicks (as std::string) are marked as valid
      try
      {
        Newick().CheckNewick(s);
      }
      catch (std::exception& e)
      {
        std::cerr << "(" << __FILE__ << "," << __LINE__ << ") "
          << s << ": " << e.what() << '\n';
      }
      //Check if valid newicks (as std::vector) are marked as valid
      try
      {
        const std::vector<int> n = Newick().StringToNewick(s);
        Newick().CheckNewick(n);
        assert(Newick().IsNewick(n));
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (converted to std::vector<int>): "
          << e.what();
      }
      //Check std::string conversion (from NewickVector(std::string))
      try
      {
        NewickVector temp(s);
        assert(s == temp.ToStr());
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (NewickVector from std::string): "
          << e.what();
      }
      //Check std::string conversion (from NewickVector(std::vector<int>))
      try
      {
        const std::vector<int> n = Newick().StringToNewick(s);
        NewickVector temp(n);
        assert(s == temp.ToStr());
      }
      catch (std::exception& e)
      {
        std::cerr << s
          << " (NewickVector from std::vector<int>): "
          << e.what();
      }
      assert(Newick().IsNewick(s));
      //Check the simpler Newicks
      if (Newick().IsBinaryNewick(Newick().StringToNewick(s)))
      {
        const std::vector<std::vector<int> > simpler
          = Newick().GetSimplerBinaryNewicks(
            Newick().StringToNewick(s));
        for(const std::vector<int> simple: simpler)
        {
          assert(Newick().IsNewick(simple));
          Newick().CheckNewick(simple);
        }
      }
      //Check the branches
      if (Newick().IsBinaryNewick(Newick().StringToNewick(s)))
      {
        const std::pair<std::vector<int>,std::vector<int> > b
            = Newick().GetRootBranchesBinary(Newick().StringToNewick(s));
        assert(Newick().IsNewick(b.first));
        assert(Newick().IsNewick(b.second));
      }
    }
  }
  //Calculate N1
  #ifndef NDEBUG
  {
    const double theta = 23.45;
    //Calculate probability the short way
    const std::string n1 = "((1,1),1,1)";
    const std::string n2 = "(1,(1,1),1)";
    const std::string n3 = "(1,1,(1,1))";
    const double p1 = ribi::NewickVector::CalculateProbability(n1,theta);
    const double p2 = ribi::NewickVector::CalculateProbability(n2,theta);
    const double p3 = ribi::NewickVector::CalculateProbability(n3,theta);
    //Calculate probability via testable binary Newicks
    const double p4
      = theta
      / Newick().CalcDenominator(Newick().StringToNewick(n1),theta)
      * ( (2.0 * ribi::NewickVector::CalculateProbability("(1,1,2)",theta) )
        + (2.0 * ribi::NewickVector::CalculateProbability("(2,(1,1))",theta) ) );
    const double p5
      = theta
      / Newick().CalcDenominator(Newick().StringToNewick(n1),theta)
      * ( (2.0 * ribi::NewickVector::CalculateProbability("(1,2,1)",theta) )
        + (2.0 * ribi::NewickVector::CalculateProbability("(2,(1,1))",theta) ) );
    const double p6
      = theta
      / Newick().CalcDenominator(Newick().StringToNewick(n1),theta)
      * ( (2.0 * ribi::NewickVector::CalculateProbability("(2,1,1)",theta) )
        + (2.0 * ribi::NewickVector::CalculateProbability("(2,(1,1))",theta) ) );
    assert(ribi::fuzzy_equal_to()(p1,p2));
    assert(ribi::fuzzy_equal_to(0.000000000000001)(p1,p3));
    assert(ribi::fuzzy_equal_to(0.000000000000001)(p1,p4));
    assert(ribi::fuzzy_equal_to(0.000000000000001)(p1,p5));
    assert(ribi::fuzzy_equal_to(0.000000000000001)(p1,p6));
  }
  //Calculate N2
  {
    const double theta = 12.34;
    //Calculate probability the short way
    const std::string n = "((1,1,1),1)";
    const double p1 = ribi::NewickVector::CalculateProbability(n,theta);
    //Calculate probability via testable binary Newicks
    const double p2
      = theta
      / Newick().CalcDenominator(Newick().StringToNewick(n),theta)
      * 6.0
      * ribi::NewickVector::CalculateProbability("((2,1),1)",theta);
    assert(ribi::fuzzy_equal_to()(p1,p2));
  }

  //Testing the known probabilities
  {
    const std::vector<boost::tuple<std::string,double,double> > v
      = Newick().GetKnownProbabilities();
    for(const auto& t:  v)
    {
      const std::string newick_str = boost::get<0>(t);
      const double theta = boost::get<1>(t);
      const double p1 = boost::get<2>(t);
      const double p2 = ribi::NewickVector::CalculateProbability(newick_str,theta);
      assert(ribi::fuzzy_equal_to(0.001)(p1,p2));
    }
  }
  #endif
}
