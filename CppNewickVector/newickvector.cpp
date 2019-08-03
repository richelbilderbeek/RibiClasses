


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




ribi::NewickVector::NewickVector(const std::string& s)
  : m_v{Newick().StringToNewick(s)}
{
  assert(Newick().IsNewick(s));
  //Can this be added?
  //assert(m_v.empty() || Newick().IsNewick(m_v));
}

ribi::NewickVector::NewickVector(const std::vector<int>& v)
  : m_v{v}
{
  assert(m_v.empty() || Newick().IsNewick(m_v));
}

const BigInteger ribi::NewickVector::CalcComplexity() const
{
  return Newick().CalcComplexity(m_v);
}

double ribi::NewickVector::CalcDenominator(const double theta) const
{
  return Newick().CalcDenominator(Peek(),theta);
}

const BigInteger ribi::NewickVector::CalcNumOfCombinations() const
{
  assert(Newick().IsNewick(m_v));
  return Newick().CalcNumOfCombinationsBinary(m_v);
}

const BigInteger ribi::NewickVector::CalcNumOfSymmetries() const
{
  assert(Newick().IsNewick(m_v));
  assert(Newick().IsBinaryNewick(m_v));
  return Newick().CalcNumOfSymmetriesBinary(m_v);
}

double ribi::NewickVector::CalculateProbability(
  const std::string& newick_str,
  const double theta)
{
  assert(Newick().IsNewick(newick_str));
  assert(theta > 0.0);
  NewickVector newick(newick_str);
  NewickStorage<NewickVector> storage(newick);
  return CalculateProbabilityInternal(
    newick,
    theta,
    storage);
}

double ribi::NewickVector::CalculateProbabilityInternal(
  const NewickVector& n,
  const double theta,
  NewickStorage<NewickVector>& storage)
{
  while(1)
  {
    //Is n already known?
    {
      const double p = storage.Find(n);
      if (p!=0.0)
      {
        return p;
      }
    }

    //Check for simple phylogeny
    if (n.IsSimple())
    {
      const double p = n.CalcProbabilitySimpleNewick(theta);
      storage.Store(n,p);
      return p;
    }
    //Complex
    //Generate other Newicks and their coefficients
    std::vector<double> coefficients;
    std::vector<NewickVector> newicks;
    {
      const double d = n.CalcDenominator(theta);
      typedef std::pair<std::vector<int>,int> NewickFrequencyPair;
      const std::vector<NewickFrequencyPair> newick_freqs
        = Newick().GetSimplerNewicksFrequencyPairs(n.Peek());
      for(const NewickFrequencyPair& p: newick_freqs)
      {
        const int frequency = p.second;
        assert(frequency > 0);
        if (frequency == 1)
        {
          newicks.push_back(p.first);
          coefficients.push_back(theta / d);
        }
        else
        {
          const double f_d = static_cast<double>(frequency);
          newicks.push_back(p.first);
          coefficients.push_back( (f_d*(f_d-1.0)) / d);
        }
      }
    }
    //Ask help about these new Newicks
    {
      const int sz = newicks.size();
      assert(newicks.size() == coefficients.size() );
      double p = 0.0;
      for (int i=0; i!=sz; ++i)
      {
        //Recursive function call
        p+=(coefficients[i] * CalculateProbabilityInternal(newicks[i],theta,storage));
      }
      storage.Store(n,p);
      return p;
    }
  }
}

double ribi::NewickVector::CalcProbabilitySimpleNewick(const double theta) const
{
  assert(Newick().IsSimple(m_v));
  assert(theta > 0.0);
  return Newick().CalcProbabilitySimpleNewick(m_v,theta);
}

int ribi::NewickVector::FindPosAfter(const std::vector<int>& v,const int x, const int index) const
{
  const int sz = v.size();
  for (int i=index; i!=sz; ++i)
  {
    assert(i >= 0);
    assert(i < sz);
    if (v[i]==x) return i;
  }
  return sz;
}

int ribi::NewickVector::FindPosBefore(const std::vector<int>& v,const int x, const int index) const
{

  for (int i=index; i!=-1; --i)
  {
    #ifndef NDEBUG
    const int sz = static_cast<int>(v.size());
    assert(i >= 0);
    assert(i < sz);
    #endif
    if (v[i]==x) return i;
  }
  return -1;
}

const std::vector<ribi::NewickVector> ribi::NewickVector::GetSimplerNewicks() const
{
  assert(Newick().IsNewick(m_v));
  const std::vector<std::vector<int> > v
    = Newick().GetSimplerBinaryNewicks(m_v);
  std::vector<NewickVector> w(std::begin(v),std::end(v));
  return w;
}

const std::pair<ribi::NewickVector,ribi::NewickVector> ribi::NewickVector::GetRootBranches() const
{
  assert(Newick().IsNewick(m_v));
  std::pair<std::vector<int>,std::vector<int> > p
    = Newick().GetRootBranchesBinary(m_v);
  return p;
}

std::string ribi::NewickVector::GetVersion() noexcept
{
  return "2.1";
}

std::vector<std::string> ribi::NewickVector::GetVersionHistory() noexcept
{
  return {
    "2009-06-01: Version 1.0: Initial version",
    "2010-08-10: Version 1.1: Major architectural revision",
    "2011-02-20: Version 1.2: Removed helper functions from global namespace",
    "2011-02-22: Version 2.0: Changed file management",
    "2011-04-08: Version 2.1: fixed error forgiven by G++, but fatal for i686-pc-mingw32-qmake"
  };
}

 bool ribi::NewickVector::IsCloseBracketRight(const int pos) const
{
  const int sz = m_v.size();

  assert(pos >= 0);
  assert(pos < sz);
  assert(m_v[pos]==1);

  for (int i=pos+1; i!=sz; ++i) //+1 because v[pos]==1
  {
    const int x = m_v[i];
    if (x == Newick::bracket_close) return true;
    if (x == Newick::bracket_open) return false;
  }
  //There will always be a final closing bracket at the right
  // that is not stored in a SortedNewickVector's std::vector
  return true;
}

bool ribi::NewickVector::IsOpenBracketLeft(const int pos) const
{
  assert(pos >= 0);
  assert(pos < static_cast<int>(m_v.size()));
  assert(m_v[pos]==1);

  for (int i=pos-1; i!=-1; --i) //-1, because v[pos]==1
  {
    const int x = m_v[i];
    if (x == Newick::bracket_open) return true;
    if (x == Newick::bracket_close) return false;
  }
  //There will always be a trailing opening bracket at the left
  // that is not stored in a SortedNewickVector's std::vector
  return true;
}

bool ribi::NewickVector::IsSimple() const
{
  return Newick().IsSimple(m_v);
}

//Does the following conversions:
// (5,(5,1)) -> (5,6)
// (4,(5,1)) -> (4,6)
// (4,(3,1)) -> (4,4)
// (4,(1,1)) -> (4,2)
// string_pos points at an index in the string current.newick after the '1'
// For example, for (4,(3,1)) the string_pos equals 7
// num is the other value between brackets
// For example, for (4,(3,1)) num will equal 3
// (5,(5,1)) -> (5,6)
// -> sz = 9
// -> bracket_open_pos  = 3
// -> bracket_close_pos = 7
// -> sz_loss = 4 = 7 - 3 = bracket_close_pos - bracket_open_pos
// -> new_sz = 5
const ribi::NewickVector ribi::NewickVector::LoseBrackets(const int x, const int i) const
{
  assert(i >= 0);
  assert(i < Size());
  assert(m_v[i] == 1);
  assert(x>0);
  std::vector<int> v_copy = m_v;

  const int bracket_open_pos
    = FindPosBefore(m_v,Newick::bracket_open,i);
  assert(bracket_open_pos > -1);
  const int bracket_close_pos
    = FindPosAfter(m_v,Newick::bracket_close,i);
  assert(bracket_close_pos < Size());
  const int sz = Size();
  const int sz_lose = bracket_close_pos - bracket_open_pos;
  const int sz_new = sz - sz_lose;
  v_copy[bracket_open_pos] = x+1;
  const std::vector<int>::iterator begin_iter(&v_copy[bracket_close_pos+1]);
  const std::vector<int>::iterator output_iter(&v_copy[bracket_open_pos+1]);
  std::copy(begin_iter,v_copy.end(),output_iter);
  v_copy.resize(sz_new);

  return NewickVector(v_copy);
}

bool ribi::NewickVector::NewickCompare(
  const std::vector<int>& lhs,
  const std::vector<int>& rhs) noexcept
{
  const int l_sz = lhs.size();
  const int r_sz = rhs.size();
  if (l_sz < r_sz) return true;
  if (l_sz > r_sz) return false;

  typedef std::vector<int>::const_iterator Iter;
  Iter lhs_iter = lhs.begin();
  const Iter lhs_end = lhs.end();
  Iter rhs_iter = rhs.begin();

  for ( ; lhs_iter != lhs_end; ++lhs_iter, ++rhs_iter)
  {
    const int x_l = *lhs_iter;
    const int x_r = *rhs_iter;
    if (x_l < x_r) return true;
    if (x_l > x_r) return false;
  }
  return false;
}

int ribi::NewickVector::Size() const noexcept
{
  return boost::numeric_cast<int>(m_v.size());
}

const ribi::NewickVector ribi::NewickVector::TermIsNotOne(const int i) const
{
  assert(m_v[i]>1);
  std::vector<int> v(m_v);
  --v[i];
  return NewickVector(v);
}

//TermIsOne is called whenever a '1' is found in a newick structure
//string_pos has the index of the character after this '1'
// (when a string has multiple 1's, TermIsOne is called for each '1',
//  with each time a different string_pos)
//If this '1' is between two brackets, with one other number,
//  these two numbers are added and the brackets are removed
//If this '1' is not between two brackets,
//  the newick string returned is empty
//Conversion examples
// (3,(15,1)), string_pos 8 -> (3,16)
//         ^   EXIT1
// (2,(23,1)), string_pos 8 -> (2,24)
//         ^   EXIT1
// (1,(20,5)), string_pos 2 -> [empty]
//   ^         EXIT-2
// (1,(1,1)), string_pos 2 -> [empty]
//   ^         EXIT-2
// (1,(1,1)), string_pos 5 -> (1,2)
//      ^      EXIT-2
// (1,(1,1)), string_pos 7 -> (1,2)
//        ^    EXIT-1
// ((1,2,3),3), string_pos 3 -> (3,3) //Might be incorrect: algorithm holds for two numbers between brackets
//    ^
const ribi::NewickVector ribi::NewickVector::TermIsOne(const int i) const
{
  const int sz = m_v.size();

  //assert(new_newick.empty());
  assert(i < sz);
  assert(m_v[i] == 1); //Must be a 1

  const bool open_bracket_left
    = IsOpenBracketLeft(i);
  const bool close_bracket_right
    = IsCloseBracketRight(i);

  if (open_bracket_left == true
    && close_bracket_right == true)
  {
    //Find other_value
    int other_value = 0;
    //If adjecent to the left is a comma
    // and subsequently a value,
    if (i > 0
      && m_v[i-1]  > 0)
    {
      other_value = m_v[i-1];
    }
    else if (i + 1 < sz
      && m_v[i+1]  > 0)
    {
      other_value = m_v[i+1];
    }

    assert(other_value >= 1);
    return LoseBrackets(other_value,i);
  }

  //Return an empty SortedNewickVector
  return NewickVector(std::vector<int>());
}

std::string ribi::NewickVector::ToStr() const
{
  assert(Newick().IsNewick(m_v));
  return Newick().NewickToString(m_v);
}

bool ribi::operator<(const NewickVector& lhs, const NewickVector& rhs)
{
  return ribi::NewickVector::NewickCompare(lhs.Peek(),rhs.Peek());
}
