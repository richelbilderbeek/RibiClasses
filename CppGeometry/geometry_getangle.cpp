#include "geometry.h"

template <class T> T Atan2Impl(const T& a, const T& b) noexcept;
template <> double Atan2Impl(const double& a, const double& b) noexcept { return std::atan2(a,b); }

template <class T>
T GetAngleClockCartesianImpl(const T& dx, const T& dy) noexcept
{
  const T pi = boost::math::constants::pi<double>();
  const T tau = boost::math::constants::two_pi<double>();
  const T angle = ribi::Geometry().Fmod(pi - Atan2Impl(dx,-dy),tau);
  assert(angle >= 0.0 && "GetAngle must return a value between zero and two pi");
  assert(angle < tau  && "GetAngle must return a value between zero and two pi");
  return angle;
}

double ribi::Geometry::GetAngleClockCartesian(const double dx, const double dy) const noexcept
{
  return GetAngleClockCartesianImpl(dx,dy);
}

double ribi::Geometry::GetAngleClockCartesian(const Coordinat2D& p) const noexcept
{
  return GetAngleClockCartesianImpl(boost::geometry::get<0>(p),boost::geometry::get<1>(p));
}

template <class T>
T GetAngleClockScreenImpl(const T& dx, const T& dy) noexcept
{
  const T pi = boost::math::constants::pi<double>();
  const T tau = boost::math::constants::two_pi<double>();
  const T angle = ribi::Geometry().Fmod(pi - Atan2Impl(dx,dy),tau);
  assert(angle >= 0.0 && "GetAngle must return a value between zero and two pi");
  assert(angle < tau  && "GetAngle must return a value between zero and two pi");
  return angle;
}

double ribi::Geometry::GetAngleClockScreen(const double dx, const double dy) const noexcept
{
  return GetAngleClockScreenImpl(dx,dy);
}

double ribi::Geometry::GetAngleClockScreen(const Coordinat2D& p) const noexcept
{
  return GetAngleClockScreenImpl(boost::geometry::get<0>(p),boost::geometry::get<1>(p));
}
