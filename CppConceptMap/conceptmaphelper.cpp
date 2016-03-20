//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmaphelper.h"

#include <cassert>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <QFile>
#include <QRegExp>

#include "fileio.h"
#include "conceptmapregex.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

double ribi::cmap::GetDistance(const double delta_x, const double delta_y)
{
  return std::sqrt( (delta_x * delta_x) + (delta_y * delta_y) );
}

double ribi::cmap::GetDistance(const double x1, const double y1, const double x2, const double y2)
{
  return GetDistance(x1-x2,y1-y2);
}

std::vector<std::string> ribi::cmap::SafeFileToVector(const std::string& filename) noexcept
{
  std::vector<std::string> v = ribi::FileIo().FileToVector(filename);
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}

std::string ribi::cmap::Unwordwrap(const std::vector<std::string>& v) noexcept
{
  //Simply concatenate
  std::string t;
  for (const std::string& s: v) { t += s; }
  return t;
}

std::vector<std::string> ribi::cmap::Wordwrap(
  const std::string& s_original, const std::size_t max_len) noexcept
{
  if (max_len == 0)
  {
    throw std::logic_error("Cannot wordwrap for a max_len of zero");
  }
  //std::clog << "Wordwrap \'" << s_original << '\'' << std::endl;
  std::string s{s_original};
  assert(s.size() == s_original.size());

  ///Replace multiple spaces with '\b ', where x is a char not in the string
  std::string::value_type x = '\b'; //Bell
  {
    const std::size_t sz = s.size();

    ///Replace spaces at beginning
    for (std::size_t i=0; i!=sz; ++i)
    {
      if (s[i] == ' ')
        s[i] = x;
      else
        break;
    }
    //Replace spaces at end
    if (sz > 0)
    {
      //i!=0, because if s[0] is a space, it is already converted to bell
      for (std::size_t i=sz-1; i!=0; ++i)
      {
        if (s[i] == ' ')
          s[i] = x;
        else
          break;
      }
    }
    ///Replace "  " by "\b "
    for (std::size_t i=0; i!=sz-1; ++i)
    {
      if (s[i] == ' ' && s[i+1] == ' ')
        s[i] = x;
      else
        break;
    }
  }

  std::vector<std::string> v;

  //Start the actual wordwrapping
  while (!s.empty())
  {
    //Is the word short enough?
    if (s.size() < max_len)
    {
      //Copy entire word
      v.push_back(s);
      s = {};
      assert(s.empty());
      continue;
    }
    //No spaces, or space beyond max_len: cut word
    if (s.find(' ') == std::string::npos || s.find(' ') > max_len)
    {
      v.push_back(s.substr(0,max_len));
      s = s.substr(max_len,s.size() - max_len);
      continue;
    }
    //Find last space before max_len
    std::size_t len = s.find(' ');
    assert(len != std::string::npos);
    assert(len < s.size());
    while (1)
    {
      const std::size_t new_len = s.find(' ',len + 1);
      if (new_len > max_len || new_len == std::string::npos) break;
      len = new_len;
    }
    assert(len + 0 < s.size());
    assert(len + 1 < s.size());
    //cut s, put cut part in vector
    const std::string line = s.substr(0,len+1); //Keep space
    assert(!line.empty());
    v.push_back(line);
    const std::size_t new_index = len+1; //After the space found
    assert(new_index < s.size());
    const std::string new_s = s.substr(new_index,s.size() - new_index);
    assert(s != new_s);
    s = new_s;
  }

  ///Replace bell characters by spaces again
  for (std::string& s: v)
  {
    assert(x != ' ');
    std::size_t pos = s.find(x);
    while (pos != std::string::npos)
    {
      assert(pos != std::string::npos);
      assert(pos < s.size());
      assert(pos == s.find(x)); //To prevent infinite while loop
      s[pos] = ' ';
      assert(s[pos] == ' ');
      assert(pos != s.find(x)); //To prevent infinite while loop
      pos = s.find(x);
    }
    assert(s.find(x) == std::string::npos);
  }

  #ifndef NDEBUG
  //Test if Unwordwrap the result produces the original input
  if (Unwordwrap(v) != s_original)
  {
    std::cerr << v.size() << '\n';
    std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::cerr,"\n"));
    std::cerr << Unwordwrap(v) << '\n';
    std::cerr << s_original << std::endl;

  }
  #endif
  assert(Unwordwrap(v) == s_original);
  return v;
}
