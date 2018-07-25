#include "xml.h"

#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

std::string ribi::xml::Decode2(std::string s) noexcept
{
  boost::algorithm::replace_all(s,"&lt;","<");
  boost::algorithm::replace_all(s,"&gt;",">");
  return s;
}

std::string ribi::xml::Encode2(std::string s) noexcept
{
  boost::algorithm::replace_all(s,"<","&lt;");
  boost::algorithm::replace_all(s,">","&gt;");
  return s;
}

std::string ribi::xml::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::xml::GetVersionHistory() noexcept
{
  return {
    "201x-xx-xx: Version 1.0: initial version",
    "2014-02-27: Version 1.1: started versioning",
    "2015-12-30: Version 1.2: added encode and decode"
  };
}

std::vector<std::string> ribi::xml::SplitXml(const std::string& s)
{
  std::vector<std::string> v;
  std::string::const_iterator i = begin(s);
  std::string::const_iterator j = begin(s);
  const std::string::const_iterator end = s.end();
  while (j!=end)
  {
    ++j;
    if ((*j=='>' || *j == '<') && std::distance(i,j) > 1)
    {
      std::string t;
      std::copy(
        *i=='<' ? i   : i+1,
        *j=='>' ? j+1 : j,
        std::back_inserter(t));
      v.push_back(t);
      i = j;
    }
  }
  return v;
}

std::string ribi::xml::StripXmlTag(const std::string& s)
{
  if (s.empty()) return "";
  if (s[0]!='<') return "";
  if (s[s.size() - 1]!='>') return "";
  const int begin = s.find_first_of('>');
  if (begin == static_cast<int>(std::string::npos)) return "";
  const int end = s.find_last_of('<');
  if (end == static_cast<int>(std::string::npos)) return "";
  if (begin > end) return "";
  assert(begin < end);
  const std::string tag_left = s.substr(0,begin+1);
  assert(!tag_left.empty());
  assert(tag_left[0] == '<');
  assert(tag_left[tag_left.size() - 1] == '>');
  const std::string tag_left_text = tag_left.substr(1,tag_left.size() - 2);
  if (tag_left_text.empty()) return "";
  const std::string tag_right = s.substr(end,s.size() - end);
  if (tag_right.size() < 2) return "";
  assert(!tag_right.empty());
  assert(tag_right[0] == '<');
  assert(tag_right[tag_right.size() - 1] == '>');
  const std::string tag_right_text = tag_right.substr(2,tag_right.size() - 3);
  if (tag_right_text.empty()) return "";
  if (tag_left_text != tag_right_text) return "";
  const std::string text = s.substr(begin + 1,end - begin - 1);
  return text;
}

std::vector<std::string> ribi::xml::XmlToPretty(const std::string& xml_string) noexcept
{
  std::vector<std::string> v = SplitXml(xml_string);
  int n = -2;
  for (std::string& s: v)
  {
    assert(!s.empty());
    if (s[0] == '<' && s[1] != '/')
    {
      n+=2;
    }
    assert(n >= 0);
    s = std::string(n,' ') + s;
    if (s[n+0] == '<' && s[n+1] == '/')
    {
      n-=2;
    }
  }
  return v;
}

std::pair<std::string,std::vector<std::string>> ribi::xml::XmlToVector(
  const std::string& s)
{
  assert(!s.empty());
  assert(s[           0] == '<');
  assert(s[s.size() - 1] == '>');
  assert(s.find('>') != std::string::npos);

  //Read the name tag
  //<tag_name>...</tag_name>
  const std::pair<std::string,std::string> p = FromXml(s);
  const std::string tag_name = p.first;
  std::vector<std::string> content;

  //Remove the name tags
  //std::string t = s.substr(tag_name_sz + 2,s.size() - (2 * tag_name_sz) - 5);
  std::string t = p.second;
  for (int i=0; !t.empty(); ++i)
  {
    //Read the index tags and item
    //<index>item</index>
    assert(!t.empty());
    assert(t[0] == '<');
    assert(t[t.size() - 1] == '>');
    assert(t.find('>') != std::string::npos);
    const int index_tag_sz = static_cast<int>(t.find('>')) - 1;
    const std::string index_tag = t.substr(1,index_tag_sz);

    assert(CanLexicalCast<int>(index_tag));
    assert(i == boost::lexical_cast<int>(index_tag));
    assert(t.find('/') != std::string::npos);
    const int item_sz = static_cast<int>(t.find('/')) - index_tag_sz - 3;
    const std::string item = t.substr(index_tag.size() + 2,item_sz);

    const int total_sz = (2 * index_tag_sz) + item_sz + 5;
    t = t.substr(total_sz,t.size() - total_sz);

    content.push_back(item);
  }
  assert(VectorToXml(tag_name,content) == s);
  return std::make_pair(tag_name,content);
}

