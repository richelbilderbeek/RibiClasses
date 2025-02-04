#ifndef RECTANGLE_H
#define RECTANGLE_H

#ifdef REALLY_USE_RECTANGLE
//Use, for example, this code instead:
//
//#include <boost/geometry/geometries/box.hpp>
//#include <boost/geometry/geometries/point_xy.hpp>
//
//  typedef boost::geometry::model::d2::point_xy<double> Point;
//  typedef boost::geometry::model::box<Point> Rect;


#include <string>
#include <vector>

namespace ribi {

///RectanglePolicyUseBottomRight stores a rectangle by its topleft
///and bottomright coordinats

template <class T>
struct RectanglePolicyUseBottomRight
{
  RectanglePolicyUseBottomRight(
    const T& any_x1 = 0,
    const T& any_y1 = 0,
    const T& any_x2 = 0,
    const T& any_y2 = 0)
  : x1(any_x1), y1(any_y1), x2(any_x2), y2(any_y2) {}

  const T GetBottom() const { return y2; }
  const T GetRight() const { return x2; }
  const T GetHeight() const { return y2 - y1; }
  const T GetWidth() const { return x2 - x1; }

  private:
  T x1;
  T y1;
  T x2;
  T y2;
};

///IntRectanglePolicyUseSize stores a rectangle by its topleft coordinats and its size
struct Rect
{
  Rect(
    const int any_x = 0,
    const int any_y = 0,
    const int any_w = 0,
    const int any_h = 0);

  ///Obtain the x coordinat of the rectangle its bottom side
  int GetBottom() const noexcept { return m_y + m_h; }

  ///Obtain the y coordinat of the rectangle its right side
  int GetRight() const noexcept { return m_x + m_w; }

  ///Obtain the rectangle its height
  int GetHeight() const noexcept { return m_h; }

  ///Obtain this class its version
  static std::string GetVersion() noexcept;

  ///Obtain this class its version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  int GetLeft() const noexcept { return GetX(); }

  ///Obtain the rectangle its width
  int GetWidth() const noexcept { return m_w; }

  ///Obtain the x coordinat of the rectangle its left side
  int GetX() const noexcept { return m_x; }

  ///Obtain the y coordinat of the rectangle its top side
  int GetY() const noexcept { return m_y; }
  int GetTop() const noexcept { return GetX(); }

  bool IsIn(const int any_x, const int any_y) const noexcept
  {
    return any_x > m_x && any_y > m_y && any_x < GetRight() && any_y < GetBottom();
  }

  void Translate(const int dx, const int dy) noexcept;

  private:
  int m_x;
  int m_y;
  int m_w;
  int m_h;

  friend bool operator==(const Rect& lhs, const Rect& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& os,const Rect& rect) noexcept;
};

std::ostream& operator<<(std::ostream& os,const Rect& rect) noexcept;
bool operator==(const Rect& lhs, const Rect& rhs) noexcept;
bool operator!=(const Rect& lhs, const Rect& rhs) noexcept;

} //~namespace ribi

#endif // RECTANGLE_H

#endif //#ifdef REALLY_USE_RECTANGLE
