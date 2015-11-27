//---------------------------------------------------------------------------
/*
Container, class with container class helper functions
Copyright (C) 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppContainer.htm
//---------------------------------------------------------------------------
#ifndef RIBI_CONTAINER_H
#define RIBI_CONTAINER_H

#include <algorithm>
#include <cassert>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic pop

namespace ribi {

///Class with container class helper functions
struct Container
{
  Container();

  bool AllAboutEqual(
    const std::vector<double>& v,
    const double tolerance
  ) const noexcept;

  template <class T>
  bool AllUnique(const T& v) const noexcept
  {
    typedef typename T::value_type ValueType;
    std::set<ValueType> s;
    std::copy(std::begin(v),std::end(v),std::inserter(s,std::end(s)));

    //Thanks to Qt for making me do these casts, as QList::size returns an int
    return static_cast<int>(s.size()) == static_cast<int>(v.size());
  }

  ///Concatenate concatenates the strings, with a certain seperator
  std::string Concatenate(const std::vector<std::string>& v, const std::string& seperator = "") const noexcept;

  ///NOTE: May be merged with ToStr
  std::string ContainerToStr(const std::vector<std::string>& s, const std::string& seperator = " ") const noexcept;

  ///Shorthand for std::count(std::begin(t),std::end(t),u)
  template <class T, class U>
  int Count(const T& t, const U& u) const noexcept
  {
    return std::count(std::begin(t),std::end(t),u);
  }

  template <class T, class Predicate>
  int CountIf(const T& t, const Predicate& p) const noexcept
  {
    return std::count_if(std::begin(t),std::end(t),p);
  }

#if __cplusplus >= 201402L //C++17
  template <class T, class U>
  decltype(auto) Find(const T& t, const U& p) const noexcept
  {
    return std::find(std::begin(t),std::end(t),p);
  }
#else
  template <class T, class U>
  typename T::const_iterator Find(const T& t, const U& p) const noexcept
  {
    return std::find(std::begin(t),std::end(t),p);
  }
#endif // __cplusplus >= 201402L

#if __cplusplus >= 201402L //C++17
  template <class T, class Predicate>
  decltype(auto) FindIf(const T& t, const Predicate& p) const noexcept
  {
    return std::find_if(std::begin(t),std::end(t),p);
  }
#else
  template <class T, class Predicate>
  typename T::const_iterator FindIf(const T& t, const Predicate& p) const noexcept
  {
    return std::find_if(std::begin(t),std::end(t),p);
  }
#endif // __cplusplus >= 201402L

  ///Obtain the version
  std::string GetVersion() const noexcept;

  ///Obtain the version history
  std::vector<std::string> GetVersionHistory() const noexcept;

  template <class T, class U>
  bool HasNoOverlap(const T& lhs,const U& rhs) const noexcept
  {
    assert(AllUnique(lhs));
    assert(AllUnique(rhs));
    typedef typename T::value_type ValueTypeT;
    typedef typename U::value_type ValueTypeU;
    static_assert(std::is_same<ValueTypeT,ValueTypeU>::value,"Containers must have the same value type");
    std::set<ValueTypeT> s;
    std::copy(std::begin(lhs),std::end(lhs),std::inserter(s,std::end(s)));
    std::copy(std::begin(rhs),std::end(rhs),std::inserter(s,std::end(s)));

    //Thanks to Qt for making me do these casts, as QList::size returns an int
    return static_cast<int>(s.size()) == static_cast<int>(lhs.size()) + static_cast<int>(rhs.size());
  }


  std::string RemoveWhitespace(std::string s) const noexcept;

  std::vector<std::string> SeperateString(
    const std::string& input,
    const char seperator) const noexcept;

  ///Shorthand for std::sort(std::begin(t),std::end(t))
  template <class T>
  void Sort(T& t) const noexcept
  {
    return std::sort(std::begin(t),std::end(t));
  }

  template <class T>
  static std::string ToStr(const std::set<T>& set) noexcept
  {
    std::stringstream s;
    for (const auto& t: set) { s << t << ","; }
    std::string str{s.str()};
    if (!str.empty()) { str.pop_back(); }
    str = "{" + str + "}";
    return str;
  }


  template <class T>
  static std::string ToStr(const std::vector<T>& v) noexcept
  {
    std::stringstream s;
    for (const auto& t: v) { s << t << ","; }
    std::string str{s.str()};
    if (!str.empty()) { str.pop_back(); }
    str = "{" + str + "}";
    return str;
  }

  ///Convert a std::string to uppercase
  std::string ToUpper(std::string s) const;

  private:
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

using container = Container;

} //~namespace ribi

#endif // RIBI_CONTAINER_H
