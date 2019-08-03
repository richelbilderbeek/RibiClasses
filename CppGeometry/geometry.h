#ifndef RIBI_GEOMETRY_H
#define RIBI_GEOMETRY_H

#pragma GCC diagnostic push


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
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Coordinat3D;

  typedef boost::geometry::model::linestring<Coordinat2D> Linestring;
  typedef boost::geometry::model::polygon<Coordinat2D> Polygon;
  typedef boost::geometry::model::box<Coordinat2D> Rect;

  typedef double Double;
  typedef std::vector<Coordinat2D> Coordinats2D;
  typedef std::vector<Coordinat3D> Coordinats3D;
  typedef std::vector<Double> Doubles;
  typedef std::vector<Linestring> Linestrings;
  typedef std::vector<Polygon> Polygons;

  typedef std::pair<Polygons,Linestrings> Shapes;

  Geometry();

  Coordinat2D CalcCenter(const Coordinats2D& v) const noexcept;
  Coordinat3D CalcCenter(const Coordinats3D& v) const noexcept;

  Coordinat3D CalcCrossProduct(const Coordinat3D& a,const Coordinat3D& b) const noexcept;


  double CalcDotProduct(
    const Coordinat3D& a,
    const Coordinat3D& b
  ) const noexcept;

  Coordinat3D CalcNormal(
    const Coordinat3D& a,
    const Coordinat3D& b,
    const Coordinat3D& c
  ) const noexcept;

  ///Determine the plane that goes through these three points
  ///Returns a std::vector of size 4
  ///CalcPlane return the coefficients in the following form:
  /// A.x + B.y + C.z = D
  ///Converting this to z being a function of x and y:
  /// -C.z = A.x + B.y - D
  /// z = -A/C.x - B/C.y + D/C
  std::vector<double> CalcPlane(
    const Coordinat3D& p1,
    const Coordinat3D& p2,
    const Coordinat3D& p3
  ) const noexcept;

  Coordinats2D CalcProjection(const   Coordinats3D& v) const;

  Coordinat2D Coordinat2DToBoostGeometryPointXy(
    const Coordinat2D& c
  ) const noexcept;

  std::vector<Coordinat2D> Coordinats2DToBoostGeometryPointsXy(
    const std::vector<Coordinat2D>& v
  ) const noexcept;

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

  ///Obtain the zero or one intersections between two finite lines
  //From http://www.richelbilderbeek.nl/CppGetLineLineIntersections.htm
  std::vector<Coordinat2D> GetLineLineIntersections(
    const Linestring line1,
    const Linestring line2
  ) const;

  ///Obtain the zero, one or two intersections between a line and a rectangle
  //From http://www.richelbilderbeek.nl/CppGetLineRectIntersections.htm
  std::vector<Coordinat2D> GetLineRectIntersections(
    const Linestring line,
    const Rect rect
  ) const;

  Coordinat2D CreatePoint(
    const double x,
    const double y
  ) const noexcept;

  Coordinat3D CreatePoint(
    const double x,
    const double y,
    const double z
  ) const noexcept;

  Rect CreateRect(
    const double left,
    const double top,
    const double width,
    const double height
  ) const noexcept;

  boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>>
    CreateHouseShape() const noexcept;

  Polygon CreateShapeHeart(const double scale = 1.0) const noexcept;
  Polygon CreateShapeHouse(const double scale = 1.0) const noexcept;
  Polygon CreateShapePolygon(const int n, const double rotation = 0.0, const double scale = 0.0) const noexcept;
  Polygon CreateShapeTriangle(const double scale = 1.0) const noexcept;

  ///Functor for X-Y-Z ordering
  std::function<bool(const Coordinat2D& lhs, const Coordinat2D& rhs)> Equals2d() const noexcept;
  std::function<bool(const Coordinat3D& lhs, const Coordinat3D& rhs)> Equals() const noexcept;

  ///Take the floating point modulus
  double Fmod(const double x, const double mod) const noexcept;

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
  double GetAngleClockScreen(const double dx, const double dy) const noexcept; //The original GetAngle
  double GetAngleClockScreen(const Coordinat2D& p) const noexcept;

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
  double GetAngleClockCartesian(const double dx, const double dy) const noexcept;
  double GetAngleClockCartesian(const Coordinat2D& p) const noexcept;


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

  //From www.richelbilderbeek.nl/CppGetDistance.htm
  double GetDistance(const double dx, const double dy) const noexcept;
  double GetDistance(const Coordinat2D& a, const Coordinat2D& b) const noexcept;
  double GetDistance(const double x1, const double y1, const double x2, const double y2) const noexcept { return GetDistance(x2-x1,y2-y1); }

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
  bool IsClockwise(const std::vector<double>& angles_radians) const noexcept;

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
  bool IsClockwiseCartesian(const   Coordinats3D& points, const   Coordinat3D& observer) const noexcept;
  ///Are the points ordered clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  //bool IsClockwiseHorizontal(const std::vector<Coordinat3D>& points) const noexcept;
  //bool IsClockwiseHorizontal(const std::vector<Coordinat2D>& points) const noexcept;
  bool IsClockwiseCartesianHorizontal(const std::vector<Coordinat2D>& v) const noexcept;
  bool IsClockwiseCartesianHorizontal(const std::vector<Coordinat3D>& v) const noexcept;

  ///Are two angles ordered counter-clockwise
  ///12 o'clock is 0.0 * pi
  /// 3 o'clock is 0.5 * pi
  /// 6 o'clock is 1.0 * pi
  /// 9 o'clock is 1.5 * pi
  ///
  ///Yes: 0.5 * pi and 0.0 * pi
  ///No : 0.0 * pi and 0.5 * pi
  bool IsCounterClockwise(const double angle_a_radians, const double angle_b_radians) const noexcept;
  bool IsCounterClockwise(const Doubles& angles_radians) const noexcept;

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
  bool IsCounterClockwiseCartesian(const   Coordinats3D& points, const   Coordinat3D& observer) const noexcept;

  ///Are the points ordered counter-clockwise in the XY plane seen from above
  /// (e.g. from coordinat {0,0,1} )
  //bool IsCounterClockwiseHorizontal(const std::vector<Coordinat3D>& points) const noexcept;
  bool IsCounterClockwiseCartesianHorizontal(const std::vector<  Coordinat2D>& v) const noexcept;
  bool IsCounterClockwiseCartesianHorizontal(const std::vector<  Coordinat3D>& v) const noexcept;



  ///Creates a polygon from the points and checks if the polygon is convex
  /*

   A---------B      A---------B
   E        /        \   D   /
    \      /          \ / \ /
     D----C            E   C

     Convex           Concave

  */
  bool IsConvex(boost::geometry::model::polygon<Coordinat2D> polygon) const noexcept;
  bool IsConvex(const   Coordinats2D& points) const noexcept;
  bool IsConvex(const   Coordinats3D& points) const noexcept;

  bool IsEqual2d(const Coordinat2D& lhs, const Coordinat2D& rhs) const noexcept { return Equals2d()(lhs,rhs); }
  bool IsEqual(const Coordinat3D& lhs, const Coordinat3D& rhs) const noexcept { return Equals()(lhs,rhs); }

  ///Determines if these coordinats are in a plane
  bool IsPlane(const std::vector<Coordinat3D>& v) const noexcept;

  ///Functor for X-Y ordering
  std::function<bool(const ribi::Geometry::Coordinat2D& lhs, const ribi::Geometry::Coordinat2D& rhs)> Order2dByX() const noexcept;

  ///Functor for X-Y-Z ordering
  std::function<bool(const ribi::Geometry::Coordinat3D& lhs, const ribi::Geometry::Coordinat3D& rhs)> OrderByX() const noexcept;

  ///Functor for Z-Y-X ordering
  std::function<bool(const ribi::Geometry::Coordinat3D& lhs, const ribi::Geometry::Coordinat3D& rhs)> OrderByZ() const noexcept;

  Linestring Rescale(
    const Linestring& polygon,
    const double scale,
    const double scale_origin_x = 0.0,
    const double scale_origin_y = 0.0
  ) const noexcept;

  Linestrings Rescale(
    const Linestrings& linestrings,
    const double scale,
    const double scale_origin_x = 0.0,
    const double scale_origin_y = 0.0
  ) const noexcept;

  Polygon Rescale(
    const Polygon& polygon,
    const double scale,
    const double scale_origin_x = 0.0,
    const double scale_origin_y = 0.0
  ) const noexcept;

  Polygons Rescale(
    const Polygons& polygons,
    const double scale,
    const double scale_origin_x = 0.0,
    const double scale_origin_y = 0.0
  ) const noexcept;

  Shapes Rescale(
    const Shapes& shapes,
    const double scale,
    const double scale_origin_x = 0.0,
    const double scale_origin_y = 0.0
  ) const noexcept;

  ///Convert a linestring to a closed polygon. If the linestring
  ///is open, close it
  /*

    +-+      +-+
    |    ->  | |
    +-+      +-+

    +-+      +-+
    | |  ->  | |
    +-+      +-+

  */
  Polygon ToPolygon(const Linestring& linestring) const noexcept;


  std::string ToStr(const   Coordinat2D& p) const noexcept;
  std::string ToStr(const   Coordinats2D& p) const noexcept;
  std::string ToStr(const   Coordinat3D& p) const noexcept;
  std::string ToStr(const Linestring& polygon) const noexcept;
  std::string ToStr(const Polygon& polygon) const noexcept;
  std::string ToStr(const QPen& pen) const noexcept;
  std::string ToStr(const QPoint& point) const noexcept;
  std::string ToStr(const QPointF& point) const noexcept;
  std::string ToStr(const QRect& rect) const noexcept;
  std::string ToStr(const QRectF& rect) const noexcept;
  std::string ToStr(const Rect& rect) const noexcept;

  //Create a complete SVG file contents
  std::string ToSvg(
    const Polygons& polygons,
    const double stroke_width = 1.0
  ) const noexcept;

  std::string ToSvg(
    const Polygons& polygons,
    const Linestrings& linestrings,
    const double stroke_width = 1.0
  ) const noexcept;

  std::string ToSvg(
    const Shapes& shapes,
    const double stroke_width = 1.0
  ) const noexcept { return ToSvg(shapes.first,shapes.second,stroke_width); }

  std::string ToWkt(
    const Polygons& polygons
  ) const noexcept { return ToWkt(polygons, {}); }

  std::string ToWkt(
    const Polygons& polygons,
    const Linestrings& linestrings
  ) const noexcept;

  std::string ToWkt(
    const Shapes& shapes
  ) const noexcept { return ToWkt(shapes.first,shapes.second); }


  Polygon Translate(
    const Polygon& shape,
    const double dx,
    const double dy
  ) const noexcept;


  //Throws if WKT does not convert to a linestring
  Linestring WktToLinestring(const std::string& wkt) const;

  //Throws if WKT does not convert to a polygon
  Polygon WktToPolygon(const std::string& wkt) const;
  Shapes WktToShapes(const std::string& wkt) const;
  Shapes WktToShapes(const std::vector<std::string>& wkt) const;
  std::string WktToSvg(const std::string& wkt, const double svg_stroke_width) const;
  std::string WktToSvg(const std::vector<std::string>& wkt, const double svg_stroke_width) const;

  private:

  //Create the line this geometry is in an SVG file
  std::string ToSvgStr(
    const std::vector<Polygon>& polygons,
    const double stroke_width = 1.0
  ) const noexcept;

  std::string ToSvgStr(
    const Polygon& polygon,
    const double stroke_width = 1.0
  ) const noexcept;

  std::string ToSvgStr(
    const Linestrings& linestrings,
    const double stroke_width = 1.0
  ) const noexcept;

  std::string ToSvgStr(
    const Linestring& linestring,
    const double stroke_width = 1.0
  ) const noexcept;

};

ribi::Geometry::Coordinat2D operator-(
  const ribi::Geometry::Coordinat2D& a,
  const ribi::Geometry::Coordinat2D& b
) noexcept;

ribi::Geometry::Coordinat3D operator-(
  const ribi::Geometry::Coordinat3D& a,
  const ribi::Geometry::Coordinat3D& b
) noexcept;


std::ostream& operator<<(std::ostream& os, const Geometry::Coordinat2D& p) noexcept;
std::ostream& operator<<(std::ostream& os, const Geometry::Coordinat3D& p) noexcept;
std::ostream& operator<<(std::ostream& os, const Geometry::Linestring& linestring) noexcept;
std::ostream& operator<<(std::ostream& os, const Geometry::Polygon& p) noexcept;

std::ostream& operator<<(std::ostream& os,const QPen& pen) noexcept;
std::ostream& operator<<(std::ostream& os,const QPoint& rect) noexcept;
std::ostream& operator<<(std::ostream& os,const QPointF& rect) noexcept;
std::ostream& operator<<(std::ostream& os,const QRect& rect) noexcept;
std::ostream& operator<<(std::ostream& os,const QRectF& rect) noexcept;
std::ostream& operator<<(std::ostream& os,const Geometry::Rect& rect) noexcept;



} //~namespace ribi

#endif // RIBI_GEOMETRY_H
