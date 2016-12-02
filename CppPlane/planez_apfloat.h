#ifndef RIBI_PLANEZ_H
#define RIBI_PLANEZ_H

#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/make_shared.hpp>
#ifndef _WIN32
#include <boost/geometry/geometries/polygon.hpp>
#endif
#include "apfloat.h"
#pragma GCC diagnostic pop

namespace ribi {

///A 3D plane that can have its Z expressed as a function of X and Y;
///A 3D plane that encompasses all X and Y coordinats;
///Examples are:
/// Z = X + Y
/// Z = X
/// Z = Y
/// Z = 0
///Can be constructed from its equation and at least three 3D points
//A plane stores its coefficients in the following form:
// A.x + B.y + C.z = D
//Converting this to z being a function of x and y:
// -C.z =  A  .x + B  .y - D
//    z = -A/C.x - B/C.y + D/C
//
//
struct PlaneZ
{
  typedef boost::geometry::model::d2::point_xy<apfloat> Coordinat2D;
  typedef boost::geometry::model::point<apfloat,3,boost::geometry::cs::cartesian> Coordinat3D;
  typedef std::vector<Coordinat2D> Coordinats2D;
  typedef std::vector<Coordinat3D> Coordinats3D;
  typedef apfloat Double;
  typedef std::vector<Double> Doubles;

  ///Create plane Z = 0.0
  PlaneZ() noexcept;


  ///Construct from three points
  explicit PlaneZ(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
  );

  ///Construct from its coefficients
  PlaneZ(const Doubles& coefficients);

  Double CalcError(const Coordinat3D& coordinat) const noexcept;

  ///Get the 2D projection of these 3D points,
  ///Assumes these are in a plane
  /*

  A: (0,0,1)                  A: (0,0)
  B: (1,0,0)                  B: (SQRT(2),0)
  C: (1,1,0)                  C: (SQRT(2),SQRT(2))

  |    /
  |   /                       |
  A-----C                     |  C
  |\/  /      -> becomes ->   | /|
  |/\ /                       |/ |
  +--B---                     A--B-----

  */
  Coordinats2D CalcProjection(const Coordinats3D& points) const;

  ///Calculates the maximum allowed error for that coordinat for it to be in the plane
  Double CalcMaxError(const Coordinat3D& coordinat) const noexcept;

  ///Throws when cannot calculate Z, which is when the plane is vertical
  Double CalcZ(const Double& x, const Double& y) const;

  const Doubles& GetCoefficients() const noexcept { return m_coefficients; }

  ///This plane has equation 'z = Ax + By + C'
  ///Will throw if A cannot be calculated
  Double GetFunctionA() const;

  ///This plane has equation 'z = Ax + By + C'
  ///Will throw if B cannot be calculated
  Double GetFunctionB() const;

  ///This plane has equation 'z = Ax + By + C'
  ///Will throw if C cannot be calculated
  Double GetFunctionC() const;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  ///Checks if the coordinat is in the plane
  bool IsInPlane(const Coordinat3D& coordinat) const noexcept;

  ///Obtain a testing series of doubles (to be used as coordinat elements)
  ///in increasing order of difficulty
  static std::vector<double> GetTestSeries() noexcept;

  ///Convert the Plane to function z(x,y), e.g
  ///'z=(2*x) + (3*y) + 5' (spaces exactly as shown)
  ///Where 2,3 and 5 can be obtained with GetFunctionA,GetFunctionB and GetFunctionC
  ///respectively
  std::string ToFunction() const;

  private:

  //m_coefficients.size == 4
  const Doubles m_coefficients;

  ///Calculates m_min_error per GetFunctionC()
  static Double CalcMinErrorPerC() noexcept;

  static Doubles CalcPlaneZ(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
  ) noexcept;
};

std::ostream& operator<<(std::ostream& os,const PlaneZ& planez);

} //~namespace ribi

#endif // RIBI_PLANEZ_H
