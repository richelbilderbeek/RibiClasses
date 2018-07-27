#ifndef CPPQTSCOPEDDISABLE_H
#define CPPQTSCOPEDDISABLE_H

#include <cassert>
#include <string>
#include <vector>

///QtScopedDisable is a template class that
///disables something upon construction, and enables it
///again when it goes out of scope.
///From http://www.richelbilderbeek.nl/CppQtScopedDisable.htm
template <class T>
struct QtScopedDisable final
{
  QtScopedDisable(T * const t)
    : m_t(t)
  {
    assert(m_t);
    m_t->setEnabled(false);
  }
  ~QtScopedDisable() noexcept
  {
    m_t->setEnabled(true);
  }

  ///Obtain the version of this class
  static std::string GetVersion()
  {
    return "1.2";
  }

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory()
  {
    std::vector<std::string> v;
    v.push_back("2012-11-22: version 1.0: initial version");
    return v;
  }


  private:
  T * const m_t;
};

#endif // CPPQTSCOPEDDISABLE_H
