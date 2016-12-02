#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "planey.h"

#include <cassert>

#include "geometry.h"
#include "planez.h"
// 
#pragma GCC diagnostic pop

ribi::PlaneY::PlaneY() noexcept
  : PlaneY(
    Coordinat3D(0.0,0.0,0.0),
    Coordinat3D(1.0,0.0,0.0),
    Coordinat3D(0.0,0.0,1.0)
  )
{

}

ribi::PlaneY::PlaneY(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
)
  : m_plane_z{Create(p1,p2,p3)}
{

}

double ribi::PlaneY::CalcError(const Coordinat3D& coordinat) const noexcept
{
  const double x = boost::geometry::get<0>(coordinat);
  const double y = boost::geometry::get<1>(coordinat);
  const double z = boost::geometry::get<2>(coordinat);
  const auto expected = y;
  const auto calculated = CalcY(x,z);
  const double error{std::abs(calculated - expected)};
  return error;
}

ribi::PlaneY::Double ribi::PlaneY::CalcMinErrorPerC() noexcept
{
  //min_error_per_c will be about 0.000000001
  //stub_value increases this jut a little, by a 0.000001%
  const double stub_value = 0.000000001 * 1.00000001;
  #define USE_STUB
  #ifdef USE_STUB
  return stub_value;
  #else //USE_STUB
  //PlaneX calculates its own tolerance for errors, by measuring it
  static Double min_error_per_c = 0.0;
  if (min_error_per_c > 0.0) return min_error_per_c;

  //const double low = std::numeric_limits<double>::denorm_min();
  //const double high = std::numeric_limits<double>::max();
  const double low  = 1.0e-16;
  const double high = 1.0e+16;
  const double min_x = low;
  const double max_x = high;
  const double min_y = low;
  const double max_y = high;
  const double min_z = low;
  const double max_z = high;
  const double zero(0.0);

  for (double z = min_z; z < max_z; z*=10.0)
  {
    for (double y = min_y; y < max_y; y*=10.0)
    {
      for (double x = min_x; x < max_x; x*=10.0)
      {
        const Coordinat3D p1(0.0,  y,0.0);
        const Coordinat3D p2(  x,  y,0.0);
        const Coordinat3D p3(0.0,  y,z);
        const PlaneY p(p1,p2,p3);
        for (const auto& p4: { p1, p2, p3 } )
        {
          const auto error = p.CalcError(p4);
          const auto error_per_c = error / p.GetFunctionC();
          assert(error_per_c >= zero);
          //TRACE(double(min_error_per_c) / p.GetFunctionC());
          if (error_per_c > min_error_per_c)
          {
            min_error_per_c = error_per_c;
            //TRACE(min_error_per_c);
            //TRACE(x);
            //TRACE(y);
            //TRACE(z);
            //TRACE(p.GetFunctionC());
            //TRACE(double(min_error_per_c) / p.GetFunctionC());
            //std::stringstream s;
            //s << Geometry().ToStr(p4) << " " << min_error;
            //TRACE(s.str());
          }
        }
      }
    }
    //TRACE(min_error_per_c);
  }
  //TRACE(min_error_per_c); //Output: TRACE 'min_error_per_c' line 127 in file '..\..\Classes\CppPlane\planey.cpp': '0.000000001e0'
  assert(min_error_per_c > zero);
  assert(min_error_per_c < stub_value);
  assert(min_error_per_c > 0.99 * stub_value);
  return min_error_per_c;
  #endif // USE_STUB
}

double ribi::PlaneY::CalcMaxError(const Coordinat3D& /*coordinat*/) const noexcept
{
  assert(CalcMinErrorPerC() > 0.0);
  const double max_error{std::abs(CalcMinErrorPerC() * GetFunctionC())};
  assert(max_error >= 0.0);
  return max_error;
}

ribi::PlaneY::Coordinats2D ribi::PlaneY::CalcProjection(
  const Coordinats3D& points
) const
{
  auto v(points);
  for(auto& i: v) { i = Rotate(i); }
  try
  {
    return m_plane_z->CalcProjection(v);
  }
  catch (std::logic_error&)
  {
    throw std::logic_error("PlaneY::CalcProjection: cannot calculate projection");
  }
}

ribi::PlaneY::Double ribi::PlaneY::CalcY(const Double& x, const Double& z) const
{
  try
  {
    return m_plane_z->CalcZ(x,z);
  }
  catch (std::logic_error&)
  {
    throw std::logic_error("ribi::PlaneY::CalcY: cannot calculate Y of a horizontal plane");
  }
}

std::unique_ptr<ribi::PlaneZ> ribi::PlaneY::Create(
  const Coordinat3D& p1,
  const Coordinat3D& p2,
  const Coordinat3D& p3
)
{
  std::unique_ptr<PlaneZ> p(
    new PlaneZ(Rotate(p1), Rotate(p2), Rotate(p3))
  );
  assert(p);
  return p;
}

std::vector<double> ribi::PlaneY::GetCoefficients() const noexcept
{
  const auto v(m_plane_z->GetCoefficients());
  assert(v.size() == 4);
  return { v[1],v[2],v[0],v[3] };
}

ribi::PlaneY::Double ribi::PlaneY::GetFunctionA() const
{
  assert(m_plane_z);
  return m_plane_z->GetFunctionA();
}

ribi::PlaneY::Double ribi::PlaneY::GetFunctionB() const
{
  assert(m_plane_z);
  return m_plane_z->GetFunctionB();
}

ribi::PlaneY::Double ribi::PlaneY::GetFunctionC() const
{
  assert(m_plane_z);
  return m_plane_z->GetFunctionC();
}

std::string ribi::PlaneY::GetVersion() const noexcept
{
  return "1.6";
}

std::vector<std::string> ribi::PlaneY::GetVersionHistory() const noexcept
{
  return {
    "2014-03-10: version 1.0: initial version, split off from PlaneX",
    "2014-03-13: version 1.1: bug fixed",
    "2014-04-01: version 1.2: use of std::unique_ptr",
    "2014-06-13: version 1.3: shortened time to compile",
    "2014-07-03: version 1.4: use of apfloat",
    "2014-07-09: version 1.5: use double in interface only",
    "2014-07-10: version 1.6: use of apfloat only"
  };
}

bool ribi::PlaneY::IsInPlane(const Coordinat3D& coordinat) const noexcept
{
  try
  {
    const double error = CalcError(coordinat);
    const double max_error = CalcMaxError(coordinat);
    return error <= max_error;
  }
  catch (std::exception& e)
  {
    // TRACE("ERROR");
    // TRACE(e.what());
    assert(!"Should not get here");
    throw;
  }
}

ribi::PlaneY::Doubles ribi::PlaneY::Rotate(const Doubles& coefficients) noexcept
{
  assert(coefficients.size() == 4);
  return
  {
    coefficients[1],
    coefficients[2],
    coefficients[0],
    coefficients[3]
  };
}

ribi::PlaneY::Coordinat3D ribi::PlaneY::Rotate(
  const Coordinat3D& point
) noexcept
{
  //The 0-2-1 order is confirmed by doing a projection of a triangle on the Y=0 plane
  //on a Y=0 plane
  return Coordinat3D(
    boost::geometry::get<0>(point),
    boost::geometry::get<2>(point),
    boost::geometry::get<1>(point)
  );
}

std::string ribi::PlaneY::ToFunction() const
{
  std::stringstream s;
  s << (*this);
  return s.str();
 }

std::ostream& ribi::operator<<(std::ostream& os,const PlaneY& planey)
{
  assert(planey.m_plane_z);
  try
  {
    os
      << "y=("
      << planey.m_plane_z->GetFunctionA() << "*x) + ("
      << planey.m_plane_z->GetFunctionB() << "*z) + "
      << planey.m_plane_z->GetFunctionC()
    ;
  }
  catch (std::logic_error&)
  {
    const std::string error
      = "ribi::PlaneY::ToFunction: cannot calculate X of a horizontal plane";
    throw std::logic_error(error.c_str());
  }
  return os;
}
