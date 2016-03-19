//---------------------------------------------------------------------------
/*
XML functions
Copyright (C) 2014-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppXml.htm
//---------------------------------------------------------------------------
#include "xml.h"
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

BOOST_AUTO_TEST_CASE(ribi_xml_test)
{
  using namespace ribi::xml;
  //StripXmlTag
  {
    BOOST_CHECK(StripXmlTag("<my_tag>text</my_tag>") == "text");
    BOOST_CHECK(StripXmlTag("<mytag>text</mytag>") == "text");
    BOOST_CHECK(StripXmlTag("<tags>text</tags>") == "text");
    BOOST_CHECK(StripXmlTag("<tag>text</tag>") == "text");
    BOOST_CHECK(StripXmlTag("<tg>text</tg>") == "text");
    BOOST_CHECK(StripXmlTag("<t>text</t>") == "text");
    BOOST_CHECK(StripXmlTag("<x>y</x>") == "y");
    BOOST_CHECK(StripXmlTag("<x>y</x></x>") == "y</x>");
    BOOST_CHECK(StripXmlTag("<x><x>y</x>") == "<x>y");
    BOOST_CHECK(StripXmlTag("<x><x>y</x></x>") == "<x>y</x>");
    BOOST_CHECK(StripXmlTag("<x>y</z>") == "");
    BOOST_CHECK(StripXmlTag("<x>y<x>") == "");
    BOOST_CHECK(StripXmlTag("<>y<>") == "");
    BOOST_CHECK(StripXmlTag("<>y</>") == "");
    BOOST_CHECK(StripXmlTag("<x>y") == "");
    BOOST_CHECK(StripXmlTag("<x></x>") == "");
  }
  for (const std::string& s: { "A", "A B", "A<B", "A>B" } )
  {
    const std::string encoded{Encode2(s)};
    const std::string t{Decode2(encoded)};
    BOOST_CHECK(s == t);
  }
  //StrToXml and XmlToStr
  {
    const std::vector<std::string> v {
      "a", "ab", "abc", " ", "",
      //"A<B", "A>B", "A<>B"
    };
    const std::size_t sz = v.size();
    for (std::size_t i=0; i!=sz; ++i)
    {
      const std::string tag_name = v[i];
      for (std::size_t j=0; j!=sz; ++j)
      {
        const std::string content = v[j];
        const std::string xml = ToXml(tag_name,content);
        BOOST_CHECK(FromXml(xml).first  == tag_name);
        BOOST_CHECK(FromXml(xml).second == content);
      }
    }
  }
  //MapToXml
  {
    //Use int to std::string map
    {
      //Create a map
      typedef int KeyType;
      typedef std::string ValueType;
      std::map<KeyType,ValueType> m;
      m.insert( std::make_pair(1,"one") );
      m.insert( std::make_pair(2,"two") );
      m.insert( std::make_pair(4,"four") );

      const std::string tag_name = "integers";

      //Convert map to XML
      const std::string xml = MapToXml(tag_name,m);

      //Convert XML back to map
      const std::function<KeyType(const std::string&)>& str_to_key_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<KeyType>(s));
          return boost::lexical_cast<KeyType>(s);
        }
      };
      const std::function<ValueType(const std::string&)>& str_to_value_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<ValueType>(s));
          return boost::lexical_cast<ValueType>(s);
        }
      };
      const std::pair<std::string,std::map<KeyType,ValueType>> p {
        XmlToMap<KeyType,ValueType>(xml,str_to_key_function,str_to_value_function)
      };
      BOOST_CHECK(p.first == tag_name);
      BOOST_CHECK(p.second.size() == m.size());
      BOOST_CHECK(std::equal(std::begin(m),std::end(m),p.second.begin()));
      //Again convert pointers to XML
      std::stringstream s;
      s << MapToXml(p.first,p.second);
    }
    //Use std::string to int map
    {
      //Create a map
      typedef std::string KeyType;
      typedef int ValueType;
      std::map<KeyType,ValueType> m;
      m.insert( std::make_pair("one",1) );
      m.insert( std::make_pair("two",2) );
      m.insert( std::make_pair("four",4) );

      const std::string tag_name = "integers";

      //Convert map to XML
      const std::string xml = MapToXml(tag_name,m);

      //Convert XML back to map
      const std::function<KeyType(const std::string&)>& str_to_key_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<KeyType>(s));
          return boost::lexical_cast<KeyType>(s);
        }
      };
      const std::function<ValueType(const std::string&)>& str_to_value_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<ValueType>(s));
          return boost::lexical_cast<ValueType>(s);
        }
      };
      const std::pair<std::string,std::map<KeyType,ValueType>> p {
        XmlToMap<KeyType,ValueType>(xml,str_to_key_function,str_to_value_function)
      };
      BOOST_CHECK(p.first == tag_name);
      BOOST_CHECK(p.second.size() == m.size());
      BOOST_CHECK(std::equal(m.begin(),m.end(),p.second.begin()));
      //Again convert pointers to XML
      std::stringstream s;
      s << MapToXml(p.first,p.second) << '\n';
    }

    //Use int to boost::shared_ptr<const std::string> map
    {
      //Create a map
      typedef std::string TagType;
      typedef int KeyType;
      typedef boost::shared_ptr<const std::string> ValueType;
      const TagType tag_name { "integers again" };
      std::map<KeyType,ValueType> m;
      m.insert(
        std::make_pair(
          1,
          boost::make_shared<const std::string>("one" )
        )
      );
      m.insert( std::make_pair(4,boost::make_shared<const std::string>("four")) );
      m.insert( std::make_pair(9,boost::make_shared<const std::string>("nine")) );

      //Convert map to XML
      const std::function<std::string(const TagType&)> tag_to_str_function {
        [](const TagType& tag)
        {
          return tag;
        }
      };
      const std::function<std::string(const KeyType&  )> key_to_str_function {
        [](const KeyType& key)
        {
          BOOST_CHECK(CanLexicalCast<std::string>(key));
          return boost::lexical_cast<std::string>(key);
        }
      };
      const std::function<std::string(const ValueType&)> value_to_str_function {
        [](const ValueType& value)
        {
          return *value;
        }
      };

      const std::string xml {
        MapToXml(tag_name,m,tag_to_str_function,key_to_str_function,value_to_str_function)
      };

      //Convert XML back to map
      //const std::function<TagType(const std::string&)>& str_to_tag_function {
      //  [](const std::string& s)
      //  {
      //    return s;
      //  }
      //};
      const std::function<KeyType(const std::string&)>& str_to_key_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<KeyType>(s));
          return boost::lexical_cast<KeyType>(s);
        }
      };
      const std::function<ValueType(const std::string&)>& str_to_value_function {
        [](const std::string& s)
        {
          return boost::make_shared<const std::string>(s);
        }
      };
      const std::pair<std::string,std::map<KeyType,ValueType>> p {
        XmlToMap<KeyType,ValueType>(xml,str_to_key_function,str_to_value_function)
      };
      BOOST_CHECK(p.first == tag_name);
      BOOST_CHECK(p.second.size() == m.size());
      BOOST_CHECK(
        std::equal(m.begin(),m.end(),p.second.begin(),
          [key_to_str_function,value_to_str_function](
            const std::pair<KeyType,ValueType>& lhs, const std::pair<KeyType,ValueType>& rhs)
          {
            return key_to_str_function(lhs.first) == key_to_str_function(rhs.first)
              && value_to_str_function(lhs.second) == value_to_str_function(rhs.second);
          }
        )
      );
      //Again convert pointers to XML
      std::stringstream s;
      s << MapToXml(tag_name,m,tag_to_str_function,key_to_str_function,value_to_str_function);
    }

  }

  //SetToXml and XmlToSet
  {
    const std::set<std::string> content { "cats", "dog", "zebrafinch" };
    const std::string tag_name = "animals";
    const std::string xml = ToXml(tag_name,content.begin(),content.begin());
    //const std::pair<std::string,std::set<std::string>> p { XmlToSet(xml) };
    //BOOST_CHECK(p.first == tag_name);
    //BOOST_CHECK(p.second == content);
  }
  //ToXml and FromXml
  {
    //tag: std::string, content: std::string
    {
      typedef std::string TagType;
      typedef std::string ContentType;
      const TagType     tag_name { "name"  };
      const ContentType content  { "Kitty" };
      const std::string xml { ToXml(tag_name,content) };
      const std::pair<TagType,ContentType> p { FromXml<TagType,ContentType>(xml) };
      BOOST_CHECK(p.first  == tag_name);
      BOOST_CHECK(p.second == content);
    }
    //tag: int, content: std::string
    {
      typedef int TagType;
      typedef std::string ContentType;
      const TagType     tag_name { 42  };
      const ContentType content  { "The answer" };
      const std::string xml { ToXml(tag_name,content) };
      const std::pair<TagType,ContentType> p { FromXml<TagType,ContentType>(xml) };
      BOOST_CHECK(p.first  == tag_name);
      BOOST_CHECK(p.second == content);
    }
    //tag: int, content: std::string
    {
      typedef int ContentType;
      typedef std::string TagType;
      const TagType     tag_name { "The answer" };
      const ContentType content  { 42 };
      const std::string xml { ToXml(tag_name,content) };
      const std::pair<TagType,ContentType> p { FromXml<TagType,ContentType>(xml) };
      BOOST_CHECK(p.first  == tag_name);
      BOOST_CHECK(p.second == content);
    }
    //tag: int, content: int
    {
      typedef std::string TagType;
      typedef int ContentType;
      const TagType     tag_name { 123 };
      const ContentType content  { 456 };
      const std::string xml { ToXml(tag_name,content) };
      const std::pair<TagType,ContentType> p { FromXml<TagType,ContentType>(xml) };
      BOOST_CHECK(p.first  == tag_name);
      BOOST_CHECK(p.second == content);
    }
    //tag: std::string, content: boost::shared_ptr<const std::string>
    {
      typedef std::string TagType;
      typedef boost::shared_ptr<const std::string> ContentType;
      const TagType     tag_name { "name" };
      const ContentType content  { boost::make_shared<const std::string>("Kitty") };

      //Convert tag and content to XML
      const std::function<std::string(const TagType&)> tag_to_str_function {
        [](const TagType& t) { return t; }
      };
      const std::function<std::string(const ContentType&)> content_to_str_function {
        [](const ContentType& c) { return *c; }
      };

      const std::string xml {
        ToXml(tag_name,content,tag_to_str_function,content_to_str_function)
      };

      //Convert XML back to its tag and content
      //with custom functions
      const std::function<TagType(const std::string&)> str_to_tag_function {
        [](const std::string& s) { return s; }
      };
      const std::function<ContentType(const std::string&)> str_to_content_function {
        [](const std::string& s) { return boost::make_shared<const std::string>(s); }
      };

      //Check both conversion functions
      //Cannot simply compare to tag_name and content, as these may be of any type
      BOOST_CHECK(tag_to_str_function(str_to_tag_function(tag_to_str_function(tag_name)))
        ==   tag_to_str_function(                                        tag_name));
      BOOST_CHECK(content_to_str_function(str_to_content_function(content_to_str_function(content)))
        ==   content_to_str_function(                                                content));

      const std::pair<TagType,ContentType> p {
        FromXml<TagType,ContentType>(
          xml,
          str_to_tag_function,
          str_to_content_function
        )
      };

      //Cannot simply compare to tag_name and content, as these may be of any type
      BOOST_CHECK(tag_to_str_function(    p.first ) == tag_to_str_function(    tag_name));
      BOOST_CHECK(content_to_str_function(p.second) == content_to_str_function(content ));
    }

    //tag: int, content: boost::shared_ptr<const std::string>
    {
      typedef int TagType;
      typedef boost::shared_ptr<const std::string> ContentType;
      const TagType     tag_name { 123 };
      const ContentType content  { boost::make_shared<const std::string>("one-two-three") };

      //Convert tag and content to XML
      const std::function<std::string(const TagType&)> tag_to_str_function {
        [](const TagType& t)
        {
          BOOST_CHECK(CanLexicalCast<std::string>(t));
          return boost::lexical_cast<std::string>(t);
        }
      };
      const std::function<std::string(const ContentType&)> content_to_str_function {
        [](const ContentType& c) { return *c; }
      };

      const std::string xml {
        ToXml(tag_name,content,tag_to_str_function,content_to_str_function)
      };

      //Convert XML back to its tag and content
      //with custom functions
      const std::function<TagType(const std::string&)> str_to_tag_function {
        [](const std::string& s)
        {
          BOOST_CHECK(CanLexicalCast<TagType>(s));
          return boost::lexical_cast<TagType>(s);
        }
      };
      const std::function<ContentType(const std::string&)> str_to_content_function {
        [](const std::string& s) { return boost::make_shared<const std::string>(s); }
      };

      //Check both conversion functions
      //Cannot simply compare to tag_name and content, as these may be of any type
      BOOST_CHECK(tag_to_str_function(str_to_tag_function(tag_to_str_function(tag_name)))
        ==   tag_to_str_function(                                        tag_name));
      BOOST_CHECK(content_to_str_function(str_to_content_function(content_to_str_function(content)))
        ==   content_to_str_function(                                                content));

      const std::pair<TagType,ContentType> p {
        FromXml<TagType,ContentType>(
          xml,
          str_to_tag_function,
          str_to_content_function
        )
      };

      //Cannot simply compare to tag_name and content, as these may be of any type
      BOOST_CHECK(tag_to_str_function(    p.first ) == tag_to_str_function(    tag_name));
      BOOST_CHECK(content_to_str_function(p.second) == content_to_str_function(content ));
    }
  }
  //VectorToXml and XmlToVector
  {
    const std::vector<std::string> content { "cats", "dog", "zebrafinch" };
    const std::string tag_name = "animals";
    const std::string xml = VectorToXml(tag_name,content);
    BOOST_CHECK(xml == "<animals><0>cats</0><1>dog</1><2>zebrafinch</2></animals>");
    BOOST_CHECK(xml == ToXml(tag_name,content.begin(),content.end()));
    const std::pair<std::string,std::vector<std::string>> p { XmlToVector(xml) };
    BOOST_CHECK(p.first == tag_name);
    BOOST_CHECK(p.second == content);
  }
  //XmlToPretty
  {
    {
      const std::vector<std::string> result {
        XmlToPretty("<a>test</a>")
      };
      const std::vector<std::string> expected {
        "<a>",
        "test",
        "</a>"
      };
      //std::copy(result.begin(),result.end(),std::ostream_iterator<std::string>(std::cerr,"\n"));
      BOOST_CHECK(result == expected);
    }
    {
      const std::vector<std::string> result {
        XmlToPretty("<a><b>test</b></a>")
      };
      const std::vector<std::string> expected {
        "<a>",
        "  <b>",
        "  test",
        "  </b>",
        "</a>"
      };
      //std::copy(result.begin(),result.end(),std::ostream_iterator<std::string>(std::cerr,"\n"));
      BOOST_CHECK(result == expected);
    }
    {
      const std::vector<std::string> result {
        XmlToPretty("<a><b>this is</b><c>a test</c></a>")
      };
      const std::vector<std::string> expected {
        "<a>",
        "  <b>",
        "  this is",
        "  </b>",
        "  <c>",
        "  a test",
        "  </c>",
        "</a>"
      };
      //std::copy(result.begin(),result.end(),std::ostream_iterator<std::string>(std::cerr,"\n"));
      BOOST_CHECK(result == expected);
    }
    {
      const std::string s = "<a>A</a>";
      const std::vector<std::string> split = SplitXml(s);
      const std::vector<std::string> split_expected
        =
        {
          "<a>",
          "A",
          "</a>"
        };
      BOOST_CHECK(split == split_expected);
      const std::vector<std::string> pretty = XmlToPretty(s);
      const std::vector<std::string> pretty_expected
        =
        {
          "<a>",
          "A",
          "</a>"
        };
      BOOST_CHECK(pretty == pretty_expected);
    }
    {
      const std::string s = "<a>A<b>B</b></a>";
      const std::vector<std::string> split = SplitXml(s);
      const std::vector<std::string> split_expected
        =
        {
          "<a>",
          "A",
          "<b>",
          "B",
          "</b>",
          "</a>"
        };
      BOOST_CHECK(split == split_expected);
      const std::vector<std::string> pretty = XmlToPretty(s);
      const std::vector<std::string> pretty_expected
        =
        {
          "<a>",
          "A",
          "  <b>",
          "  B",
          "  </b>",
          "</a>"
        };
      BOOST_CHECK(pretty == pretty_expected);
    }
    {
      const std::string s = "<a>A<b>B1</b><b>B2</b></a>";
      const std::vector<std::string> split = SplitXml(s);
      const std::vector<std::string> split_expected
        =
        {
          "<a>",
          "A",
          "<b>",
          "B1",
          "</b>",
          "<b>",
          "B2",
          "</b>",
          "</a>"
        };
      BOOST_CHECK(split == split_expected);
      const std::vector<std::string> pretty = XmlToPretty(s);
      const std::vector<std::string> pretty_expected
        =
        {
          "<a>",
          "A",
          "  <b>",
          "  B1",
          "  </b>",
          "  <b>",
          "  B2",
          "  </b>",
          "</a>"
        };
      BOOST_CHECK(pretty == pretty_expected);
    }
  }
}
