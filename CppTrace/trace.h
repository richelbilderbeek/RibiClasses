#ifndef RIBI_TRACE_H
#define RIBI_TRACE_H

#include <string>
#include <vector>

struct Trace
{
  static std::string GetVersion()
  {
    return "2.6";
  }
  static std::vector<std::string> GetVersionHistory()
  {
    return {
      "2010-xx-xx: Version 1.0: initial version, write trace to std::cout",
      "2011-06-22: Version 2.0: added versioning, added boost::lexical_cast in //TRACE",
      "2011-08-15: Version 2.1: added all #includes",
      "2012-01-15: Version 2.2: Moved bracket in //TRACE to allow 'if (false) //TRACE(true);' without surprises",
      "2012-02-01: Version 2.3: put START_TRACE in its own scope",
      "2012-08-11: Version 2.4: put #ifdef _WIN32 on top for cross-compiling",
      "2013-02-24: Version 2.5: added FTRACE for tracing to file only",
      "2013-07-16: Version 2.6: transitioned to Qt5, GCC 4.8 and Boost 1.54.0, write trace to std::clog"
    };
  }
};

#ifdef NTRACE_BILDERBIKKEL
  #define START_TRACE() ((void)0)
  #define TRACE(x) ((void)0)
  #define FTRACE(x) ((void)0)
  #define TRACE_FUNC() ((void)0)
#else
  #include <fstream>
  #include <iostream>
  #include <stdexcept>

  
  
  
  #include <boost/lexical_cast.hpp>
  

  #define START_TRACE()        \
  {                            \
    std::ofstream f_trace1496( \
    "trace_out.txt");          \
    f_trace1496.close();       \
  }                            \


  #define TRACE(x)             \
  {                            \
    std::ofstream f_314_xyz(   \
      "trace_out.txt",         \
      std::ios::app);          \
    f_314_xyz                  \
      << "TRACE \'"            \
      <<  std::string(#x )     \
      <<  "\' line "           \
      <<  (__LINE__)           \
      <<  " in file '"         \
      <<  __FILE__             \
      <<  "': '"               \
      <<  boost::lexical_cast< \
          std::string>(x)      \
      <<  "'"                  \
      << std::endl;            \
    f_314_xyz.close();         \
    std::clog                  \
      << "TRACE \'"            \
      <<  std::string (#x)     \
      <<  "\' line "           \
      <<  (__LINE__)           \
      <<  " in file '"         \
      <<  __FILE__             \
      <<  "': '"               \
      <<  boost::lexical_cast< \
          std::string>(x)      \
      <<  "'"                  \
      << std::endl;            \
  }                            \

  //File-only trace
  #define FTRACE(x)            \
  {                            \
    std::ofstream f(           \
      "trace_out.txt",         \
      std::ios::app);          \
    f                          \
      << "TRACE \'"            \
      <<  std::string(#x )     \
      <<  "\' line "           \
      <<  (__LINE__)           \
      <<  " in file '"         \
      <<  __FILE__             \
      <<  "': '"               \
      <<  boost::lexical_cast< \
          std::string>(x)      \
      <<  "'"                  \
      << std::endl;            \
    f.close();                 \
  }                            \

  #define TRACE_FUNC()      \
  {                         \
    std::ofstream f(        \
      "trace_out.txt",      \
      std::ios::app);       \
    f                       \
      << "TRACE_FUNC "      \
      <<  (__func__)        \
      <<  " , "             \
      <<  (__LINE__)        \
      <<  " , "             \
      <<  __FILE__          \
      << std::endl;         \
    f.close();              \
    std::clog               \
      << "TRACE_FUNC "      \
      <<  (__func__)        \
      <<  " , "             \
      <<  (__LINE__)        \
      <<  " , "             \
      <<  __FILE__          \
      << std::endl;         \
  }                         \

#endif

#endif // RIBI_TRACE_H
