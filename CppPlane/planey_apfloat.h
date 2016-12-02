#ifndef RIBI_PLANEY_H
#define RIBI_PLANEY_H

#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/make_shared.hpp>
//#include "planez.h"

#include "apfloat.h"
#pragma GCC diagnostic pop

namespace ribi {

struct PlaneZ;

///A 3D plane that can have its X expressed as a function of Y and Z.
///Can be constructed from its equation and at least three 3D points
///A plane stores its coefficients in the following form:
/// A.z + B.x + C.y = D
///Converting this to y being a function of x and z:
/// -C.y =  A  .z + B  .x - D
///    y = -A/C.z - B/C.x + D/C
///Where A,B,C and D can be obtained by using GetCoefficients
///Easier might be to express Y as:
///y = Ax + Bz + C
///Where A,B and C can be obtained by using GetFunctionA/B/C

struct PlaneY
{
  typedef boost::geometry::model::d2::point_xy<apfloat> Coordinat2D;
  typedef boost::geometry::model::point<apfloat,3,boost::geometry::cs::cartesian> Coordinat3D;
  typedef std::vector<Coordinat2D> Coordinats2D;
  typedef std::vector<Coordinat3D> Coordinats3D;
  typedef apfloat Double;
  typedef std::vector<Double> Doubles;

  ///Create plane Y = 0.0
  PlaneY() noexcept;

  ///Construct from three points
  explicit PlaneY(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
  );

  Double CalcError(const Coordinat3D& coordinat) const noexcept;

  Double CalcMaxError(const Coordinat3D& coordinat) const noexcept;

  ///Get the 2D projection of these 3D points,
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

  ///Throws when cannot calculate Y, which is when the plane is horizontal
  Double CalcY(const Double& y, const Double& z) const;

  ///y = Ax + Bz + C
  ///Will throw if A cannot be calculated
  Double GetFunctionA() const;

  ///y = Ax + Bz + C
  ///Will throw if B cannot be calculated
  Double GetFunctionB() const;

  ///y = Ax + Bz + C
  ///Will throw if C cannot be calculated
  Double GetFunctionC() const;

  Doubles GetCoefficients() const noexcept;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  ///Checks if the coordinat is in the plane
  bool IsInPlane(const Coordinat3D& coordinat) const noexcept;

  ///Convert the PlaneY to a y(x,z), e.g 'y=(2*x) + (3*z) + 5' (spaces exactly as shown)
  std::string ToFunction() const;

  private:

  ///A PlaneY is actually a PlaneZ used with its coordinats rotated from (X,Y,Z) to (Z,Y,Y)
  const std::unique_ptr<PlaneZ> m_plane_z;

  ///Calculates m_min_error per GetFunctionC()
  static Double CalcMinErrorPerC() noexcept;

  static std::unique_ptr<PlaneZ> Create(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
  );

  static Doubles Rotate(const Doubles& coefficients) noexcept;

  static Coordinat3D Rotate(const Coordinat3D& point) noexcept;

  friend std::ostream& operator<<(std::ostream& os,const PlaneY& planey);
};

std::ostream& operator<<(std::ostream& os,const PlaneY& planey);

} //~namespace ribi

#endif // RIBI_PLANEY_H
