#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "widget.h"

#include <boost/geometry/algorithms/equals.hpp>

#include "geometry.h"


#pragma GCC diagnostic pop


void ribi::WidgetTest() noexcept
{
  using Point = boost::geometry::model::d2::point_xy<double>;
  {
    const Widget a;
    const Widget b;
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.0,1.0));
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.5,0.0),Point(1.0,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.5),Point(1.0,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.5,1.0));
    assert(a != b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    const Widget b(Point(0.0,0.0),Point(1.0,1.5));
    assert(a != b);
  }

  {
    const Widget a;
    const Widget b(a);
    assert(a == b);
  }
  {
    const Widget a(Point(0.0,0.0),Point(1.0,1.0));
    Widget b(Point(0.0,0.0),Point(1.0,1.5));
    assert(a != b);
    b = a;
    assert(a == b);
  }
}
