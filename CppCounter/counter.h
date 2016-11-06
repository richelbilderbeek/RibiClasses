#ifndef COUNTER_H
#define COUNTER_H

#include <string>
#include <vector>

namespace ribi {

///Counter is a counter
///It may appear to be a rather trivial class, but I use it mostly to test signals:
///  MyClass my_class;
///  Counter c{0}; //For receiving the signal
///  c.SetVerbosity(true);
///  my_class.m_signal.connect(
///    boost::bind(&MyClass::AnyMemberFunction,&c)
///  );
///  my_class.DoSomethingThatEmitsAsignal();
///  assert(c.Get() == 1);
struct Counter
{
  ///Counter is constructed with an initial count, which is initialized to
  ///zero by default
  explicit Counter(const int initial_count = 0) noexcept;

  ///Increments count
  void Inc() noexcept;

  //Prefix
  Counter& operator++()
  {
    Inc();
    return *this;
  }

  ///Get the count
  int Get() const noexcept { return m_count; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetVerbosity(const bool verbosity) noexcept { m_verbosity = verbosity; }

  private:
  ///The count
  int m_count;

  bool m_verbosity;
};

bool operator==(const Counter& lhs, const Counter& rhs) noexcept;
bool operator!=(const Counter& lhs, const Counter& rhs) noexcept;

} //~namespace ribi

#endif // COUNTER_H
