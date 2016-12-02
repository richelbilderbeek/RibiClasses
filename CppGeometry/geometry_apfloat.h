#ifndef RIBI_GEOMETRY_APFLOAT_H
#define RIBI_GEOMETRY_APFLOAT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <cassert>
#include <functional>
#include <string>
#include <tuple>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#ifndef _WIN32
#include <boost/geometry/geometries/polygon.hpp>
#endif

#include "apfloat.h"

struct QPen;
struct QRect;
struct QRectF;
struct QPoint;
struct QPointF;
#pragma GCC diagnostic pop

namespace ribi {

template <class T>
bool operator==(
  const boost::geometry::model::d2::point_xy<T>& a,
  const boost::geometry::model::d2::point_xy<T>& b)
{
  return boost::geometry::equals(a,b);
}

template <class T>
bool operator!=(
  const boost::geometry::model::d2::point_xy<T>& a,
  const boost::geometry::model::d2::point_xy<T>& b)
{
  return !(a == b);
}

template <class T>
bool operator==(
  const boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T> > a,
  const boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T> > b)
{
  return boost::geometry::equals(a,b);
}

template <class T>
bool operator!=(
  const boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T> > a,
  const boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T> > b)
{
  return !(a==b);
}


///Geometry functions, working with Boost.Geometry
struct Geometry
{
  typedef boost::geometry::model::d2::point_xy<double> Coordinat2D;
  typedef boost::geometry::model::d2::point_xy<apfloat> ApCoordinat2D;
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Coordinat3D;
  typedef boost::geometry::model::point<apfloat,3,boost::geometry::cs::cartesian> ApCoordinat3D;

  typedef boost::geometry::model::linestring<Coordinat2D> Linestring;
  typedef boost::geometry::model::polygon<Coordinat2D> Polygon;
  typedef boost::geometry::model::box<Coordinat2D> Rect;

  typedef apfloat Apfloat;
  typedef double Double;
  typedef std::vector<ApCoordinat2D> ApCoordinats2D;
  typedef std::vector<ApCoordinat3D> ApCoordinats3D;
  typedef std::vector<Apfloat> Apfloats;
  typedef std::vector<Coordinat2D> Coordinats2D;
  typedef std::vector<Coordinat3D> Coordinats3D;
  typedef std::vector<Double> Doubles;
  typedef std::vector<Linestring> Linestrings;
  typedef std::vector<Polygon> Polygons;

  typedef std::pair<Polygons,Linestrings> Shapes;

  Geometry();

  ///apfloat its atan2 does not correspond to std::atan2
  ///Atan2 makes it correspond
  Apfloat Atan2(const Apfloat& dx, const Apfloat& b) const noexcept;

  ApCoordinat2D CalcCenter(const ApCoordinats2D& v) const noexcept;
  ApCoordinat3D CalcCenter(const ApCoordinats3D& v) const noexcept;

  ApCoordinat3D CalcCrossProduct(const ApCoordinat3D& a,const ApCoordinat3D& b) const noexcept;


  Apfloat CalcDotProduct(
    const ApCoordinat3D& a,
    const ApCoordinat3D& b
  ) const noexcept;

  ApCoordinat3D CalcNormal(
    const ApCoordinat3D& a,
    const ApCoordinat3D& b,
    const ApCoordinat3D& c
  ) const noexcept;

  ///Determine the plane that goes through these three points
  ///Returns a std::vector of size 4
  ///CalcPlane return the coefficients in the following form:
  /// A.x + B.y + C.z = D
  ///Converting this to z being a function of x and y:
  /// -C.z = A.x + B.y - D
  /// z = -A/C.x - B/C.y + D/C
  std::vector<apfloat> CalcPlane(
    const ApCoordinat3D& p1,
    const ApCoordinat3D& p2,
    const ApCoordinat3D& p3
  ) const noexcept;

  ApCoordinats2D CalcProjection(const ApCoordinats3D& v) const;

  //Helper function to create a const line without a temporary std::vector
  template <class T>
  const boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T>
  >
  CreateLine(const std::initializer_list<boost::geometry::model::d2::point_xy<T> >& v) const
  {
    return boost::geometry::model::linestring<
      boost::geometry::model::d2::point_xy<T>
    >(v.begin(),v.end());
  }

  template <class T>
  boost::geometry::model::linestring<boost::geometry::model::d2::point_xy<T>>
  CreateLine(const std::vector<boost::geometry::model::d2::point_xy<T> >& v) const
  {
    return boost::geometry::model::linestring<
      boost::geometry::model::d2::point_xy<T>
    >(v.begin(),v.end());
  }

  ///Functor for X-Y-Z ordering
  std::function<bool(const ApCoordinat3D& lhs, const ApCoordinat3D& rhs)> Equals3d() const noexcept;

  ///Take the floating point modulus
  Apfloat Fmod(const Apfloat& x, const Apfloat& mod) const noexcept;

  ///Obtain the angle in radians between two deltas, using a screen geometry and following a clock
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi

  /*
   Y          Y
   |    (11)  |  (1)
 -2|          |
   |          |
 -1| (10)     |      (2)
   |          |
  0+----------0--------X
   |          |
 +1| (8)      |      (4)
   |          |
 +2|          |
   |     (7)  |  (5)
   +----------+--------X
        - - -   + + +
        3 2 1 0 1 2 3

  Approximate coordinat for a point for every hour, with the approximate angle
   1: ( 1,-2) :  1/6 * pi
   2: ( 2,-1) :  2/6 * pi
   3: ( 3, 0) :  3/6 * pi
   4: ( 2, 1) :  4/6 * pi
   5: ( 1, 2) :  5/6 * pi
   6: ( 0, 3) :  6/6 * pi
   7: (-1, 2) :  7/6 * pi
   8: (-2, 1) :  8/6 * pi
   9: (-3, 0) :  9/6 * pi
  10: (-2,-1) : 10/6 * pi
  11: (-1,-2) : 11/6 * pi
  12: ( 0,-3) : 12/6 * pi

  */
  //From www.richelbilderbeek.nl/CppGetAngle.htm
  Apfloat GetAngleClockScreen(const Apfloat& dx, const Apfloat& dy) const noexcept;
  Apfloat GetAngleClockScreen(const ApCoordinat2D& p) const noexcept;

  ///Obtain the angle in radians between two deltas, using a Cartesian plane and following a clock
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi

  /*
   Y          Y
   |    (11)  |  (1)
 +2|          |
   |          |
 +1| (10)     |      (2)
   |          |
  0+----------0--------X
   |          |
 -1| (8)      |      (4)
   |          |
 -2|          |
   |     (7)  |  (5)
   +----------+--------X
        - - -   + + +
        3 2 1 0 1 2 3

  Approximate coordinat for a point for every hour, with the approximate angle
   1: ( 1, 2) :  1/6 * pi
   2: ( 2, 1) :  2/6 * pi
   3: ( 3, 0) :  3/6 * pi
   4: ( 2,-1) :  4/6 * pi
   5: ( 1,-2) :  5/6 * pi
   6: ( 0,-3) :  6/6 * pi
   7: (-1,-2) :  7/6 * pi
   8: (-2,-1) :  8/6 * pi
   9: (-3, 0) :  9/6 * pi
  10: (-2, 1) : 10/6 * pi
  11: (-1, 2) : 11/6 * pi
  12: ( 0, 3) : 12/6 * pi

  */
  //From www.richelbilderbeek.nl/CppGetAngle.htm
  Apfloat GetAngleClockCartesian(const Apfloat& dx, const Apfloat& dy) const noexcept;
  Apfloat GetAngleClockCartesian(const ApCoordinat2D& p) const noexcept;


  template <class T>
  T GetBottom(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto bottom = get<max_corner,1>(r);
    #ifndef NDEBUG
    const auto top = get<min_corner,1>(r);
    assert(top <= bottom);
    #endif
    return bottom;
  }

  template <class T>
  T GetHeight(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto top   (get<min_corner,1>(r));
    const auto bottom(get<max_corner,1>(r));
    assert(top <= bottom);
    return bottom - top;
  }


  template <class T>
  T GetLeft(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto left (get<min_corner,0>(r));
    #ifndef NDEBUG
    const auto right(get<max_corner,0>(r));
    assert(left <= right);
    #endif
    return left;
  }

  template <class T>
  T GetRight(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto right(get<max_corner,0>(r));
    #ifndef NDEBUG
    const auto left (get<min_corner,0>(r));
    assert(left <= right);
    #endif
    return right;
  }

  template <class T>
  T GetTop(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto top   (get<min_corner,1>(r));
    #ifndef NDEBUG
    const auto bottom(get<max_corner,1>(r));
    assert(top <= bottom);
    #endif
    return top;
  }

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  template <class T>
  T GetWidth(const boost::geometry::model::box<boost::geometry::model::d2::point_xy<T>>& r) const noexcept
  {
    using boost::geometry::get;
    using boost::geometry::min_corner;
    using boost::geometry::max_corner;
    const auto left (get<min_corner,0>(r));
    const auto right(get<max_corner,0>(r));
    assert(left <= right);
    return right - left;
  }
  ///Are two angles ordered clockwise
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi
  ///
  ///Yes: 0.0 * pi and 0.5 * pi
  ///No : 0.5 * pi and 0.0 * pi
  bool IsClockwise(const double angle_a_radians, const double angle_b_radians) const noexcept;
  bool IsClockwise(const Apfloat& angle_a_radians, const Apfloat& angle_b_radians) const noexcept;
  bool IsClockwise(const std::vector<double>& angles_radians) const noexcept;
  bool IsClockwise(const Apfloats& angles_radians) const noexcept;

  ///Are the points ordered clockwise in the XYZ plane, seen from the observer?
  /*

  Uses the following geometry

                |
  (-2, 1) = D   +   A = (2,1)
                |
           -+-+-O-+-+-
                |
  (-2,-1) = C   +   B = (2,-1)
                |

  */
  bool IsClockwiseCartesian(const ApCoordinats3D& points, const ApCoordinat3D& observer) const noexcept;
  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  //bool IsClockwiseHorizontal(const std::vector<Coordinat3D>& points) const noexcept;
  //bool IsClockwiseHorizontal(const std::vector<Coordinat2D>& points) const noexcept;
  bool IsClockwiseCartesianHorizontal(const std::vector<ApCoordinat2D>& v) const noexcept;
  bool IsClockwiseCartesianHorizontal(const std::vector<ApCoordinat3D>& v) const noexcept;

  ///Are two angles ordered counter-clockwise
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi
  ///
  ///Yes: 0.5 * pi and 0.0 * pi
  ///No : 0.0 * pi and 0.5 * pi
  bool IsCounterClockwise(const Apfloat& angle_a_radians, const Apfloat& angle_b_radians) const noexcept;
  bool IsCounterClockwise(const Apfloats& angles_radians) const noexcept;

  ///Are the points ordered counterclockwise in the XYZ plane, seen from the observer?
  /*

  Uses the following geometry

                |
  (-2, 1) = D   +   A = (2,1)
                |
           -+-+-O-+-+-
                |
  (-2,-1) = C   +   B = (2,-1)
                |

  */
  bool IsCounterClockwiseCartesian(const ApCoordinats3D& points, const ApCoordinat3D& observer) const noexcept;

  ///Are the points ordered counter-clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  //bool IsCounterClockwiseHorizontal(const std::vector<Coordinat3D>& points) const noexcept;
  bool IsCounterClockwiseCartesianHorizontal(const std::vector<ApCoordinat2D>& v) const noexcept;
  bool IsCounterClockwiseCartesianHorizontal(const std::vector<ApCoordinat3D>& v) const noexcept;



  ///Creates a polygon from the points and checks if the polygon is convex
  /*

   A---------B      A---------B
   E        /        \   D   /
    \      /          \ / \ /
     D----C            E   C

     Convex           Concave

  */
  bool IsConvex(const ApCoordinats3D& points) const noexcept;

  bool IsEqual3d(const ApCoordinat3D& lhs, const ApCoordinat3D& rhs) const noexcept { return Equals3d()(lhs,rhs); }

  ///Determines if these coordinats are in a plane
  bool IsPlane(const std::vector<ApCoordinat3D>& v) const noexcept;

  Apfloats ToApfloat(const Doubles& v) const noexcept;
  ApCoordinat2D ToApfloat(const Coordinat2D& p) const noexcept;
  ApCoordinat3D ToApfloat(const Coordinat3D& p) const noexcept;
  ApCoordinats2D ToApfloat(const Coordinats2D& p) const noexcept;
  ApCoordinats3D ToApfloat(const Coordinats3D& p) const noexcept;

  ///Will throw a boost::bad_lexical_cast if it fails
  double ToDouble(const apfloat& a) const;
  std::vector<double> ToDouble(const std::vector<apfloat>& a) const;

  ///Will convert the apfloat to its smallest value, zero or its heighest value
  double ToDoubleSafe(const apfloat& a) const noexcept;
  Coordinat2D ToDoubleSafe(const ApCoordinat2D& a) const noexcept;
  Coordinat3D ToDoubleSafe(const ApCoordinat3D& a) const noexcept;
  Coordinats2D ToDoubleSafe(const ApCoordinats2D& a) const noexcept;

  std::string ToStr(const Apfloats& p) const noexcept;
  std::string ToStr(const ApCoordinat2D& p) const noexcept;
  std::string ToStr(const ApCoordinats2D& p) const noexcept;
  std::string ToStr(const ApCoordinat3D& p) const noexcept;
  std::string ToStr(const ApCoordinats3D& p) const noexcept;

  ///Uses ToDoubleSafe(const apfloat&)
  std::string ToStrSafe(const Apfloat& f) const noexcept;

  //Create a complete SVG file contents

  private:

  ///Replaces what static_cast<int>(a) would do
  int ToInt(const Apfloat& a) const noexcept;
};

ribi::Geometry::ApCoordinat2D operator-(
  const ribi::Geometry::ApCoordinat2D& a,
  const ribi::Geometry::ApCoordinat2D& b
) noexcept;

ribi::Geometry::ApCoordinat3D operator-(
  const ribi::Geometry::ApCoordinat3D& a,
  const ribi::Geometry::ApCoordinat3D& b
) noexcept;

} //~namespace ribi

#endif // RIBI_GEOMETRY_APFLOAT_H
