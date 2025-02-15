



#include "ribi_regex.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <boost/xpressive/xpressive.hpp>

//Older version:
//#include <QRegExp>
#include <QRegularExpression>

ribi::Regex::Regex()
{
}

std::vector<std::string>
  ribi::Regex::GetRegexMatchesQRegExp(
  const std::string& s,
  const std::string& regex_str
) const noexcept
{
  assert(!s.empty());
  assert(!regex_str.empty());
  std::vector<std::string> v;
  assert(!"Not used");
  /*
  QRegularExpression r(regex_str.c_str());
  r.setMinimal(true); //QRegExp must be non-greedy
  int pos = 0;
  while ((pos = r.indexIn(s.c_str(), pos)) != -1)
  {
    const QString q = r.cap(1);
    if (q.isEmpty()) break;
    v.push_back(q.toStdString());
    pos += r.matchedLength();
  }
  */
  return v;
}

std::vector<std::string>
  ribi::Regex::GetRegexMatchesXpressive(
  const std::string& s,
  const std::string& regex_str
) const
{
  if (!IsValidXpressive(regex_str))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "invalid regex '" << regex_str
      << "' supplied"
    ;
    throw std::invalid_argument(msg.str());
  }

  const boost::xpressive::sregex r
    = boost::xpressive::sregex::compile(regex_str)
  ;

  std::vector<std::string> v;
  boost::xpressive::sregex_iterator cur(
    std::begin(s),
    std::end(s),
    r
  );
  boost::xpressive::sregex_iterator end;
  for( ; cur != end; ++cur )
  {
    const boost::xpressive::smatch& what = *cur;
    v.push_back(what[0]);
  }
  return v;
}

std::string ribi::Regex::GetRegexDutchFloat() const noexcept
{
  return R"((\d*,\d*))";
}

std::string ribi::Regex::GetRegexDutchPostalCode() const noexcept
{
  return R"((\d{4})( )([A-Z]{2}))";
}

std::string ribi::Regex::GetRegexPolygon() const noexcept
{
  return
    "(POLYGON\\(\\(.*?\\)\\))"
  ;
}

std::string ribi::Regex::GetRegexShapes() const noexcept
{
  return R"((POLYGON\(\(.*?\)\))|(LINESTRING\(.*?\)))";
}

std::string ribi::Regex::GetVersion() noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Regex::GetVersionHistory() noexcept
{
  return {
    "2014-06-19: Version 1.0: initial version",
    "2014-01-02: Version 1.1: added GetRegexDutchFloat",
    "2016-03-19: Version 1.2: use of Boost.Test"
  };
}

bool ribi::Regex::IsValidXpressive(const std::string& regex_str) const noexcept
{
  try
  {
    boost::xpressive::sregex::compile(regex_str);
  }
  catch (boost::xpressive::regex_error&)
  {
    return false;
  }
  return true;
}
