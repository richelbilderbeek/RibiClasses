#include "trianglemeshpointfactory.h"

#include <cassert>



#include "geometry.h"

#include "trianglemeshhelper.h"
#include "trianglemeshpoint.h"
#include "trianglemeshface.h"
#include "trianglemeshfacefactory.h"
#include "trianglemeshhelper.h"
#include "trianglemeshwindings.h"


ribi::trim::PointFactory::PointFactory()
{
  #ifndef NDEBUG
  Test();
  #endif
}

boost::shared_ptr<ribi::trim::Point> ribi::trim::PointFactory::Create(
  const boost::shared_ptr<const Coordinat2D> coordinat
) const noexcept
{
  
  //Give every Point some index at creation
  static int cnt = 1;
  const int n = cnt;
  ++cnt;


  const boost::shared_ptr<Point> point(
    new Point(
      coordinat,
      n,
      *this
    )
  );
  assert(point);
  return point;
}

boost::shared_ptr<ribi::trim::Point> ribi::trim::PointFactory::Create(
  const boost::shared_ptr<const Coordinat2D> coordinat,
  const boost::units::quantity<boost::units::si::length> z
) const noexcept
{
  const auto point = Create(coordinat);
  assert(point);
  point->SetZ(z);
  return point;
}

std::vector<boost::shared_ptr<ribi::trim::Point>> ribi::trim::PointFactory::CreateTestCube() const noexcept
{
  const boost::shared_ptr<Coordinat2D> co_a { new Coordinat2D(-1.0,-1.0) };
  const boost::shared_ptr<Coordinat2D> co_b { new Coordinat2D(-1.0, 1.0) };
  const boost::shared_ptr<Coordinat2D> co_c { new Coordinat2D( 1.0,-1.0) };
  const boost::shared_ptr<Coordinat2D> co_d { new Coordinat2D( 1.0, 1.0) };

  const auto a = PointFactory().Create(co_a,-1.0 * boost::units::si::meter);
  const auto b = PointFactory().Create(co_b,-1.0 * boost::units::si::meter);
  const auto c = PointFactory().Create(co_c,-1.0 * boost::units::si::meter);
  const auto d = PointFactory().Create(co_d,-1.0 * boost::units::si::meter);

  const auto e = PointFactory().Create(co_a,1.0 * boost::units::si::meter);
  const auto f = PointFactory().Create(co_b,1.0 * boost::units::si::meter);
  const auto g = PointFactory().Create(co_c,1.0 * boost::units::si::meter);
  const auto h = PointFactory().Create(co_d,1.0 * boost::units::si::meter);

  const std::vector<boost::shared_ptr<Point>> cube {
    a,b,c,d,e,f,g,h
  };
  return cube;
}

std::vector<boost::shared_ptr<ribi::trim::Point>>
  ribi::trim::PointFactory::CreateTestInvalid() const noexcept
{
  //1: -1  0 0
  //2:  1 -0 1
  //7:  1 -0 0
  //3: -1  0 1
  const boost::shared_ptr<Coordinat2D> co_a { new Coordinat2D(-1.0, 0.0) };
  const boost::shared_ptr<Coordinat2D> co_b { new Coordinat2D( 1.0,-0.0) };
  const boost::shared_ptr<Coordinat2D> co_c { new Coordinat2D( 1.0,-0.0) };
  const boost::shared_ptr<Coordinat2D> co_d { new Coordinat2D(-1.0, 0.0) };
  const auto a(PointFactory().Create(co_a));
  const auto b(PointFactory().Create(co_b));
  const auto c(PointFactory().Create(co_c));
  const auto d(PointFactory().Create(co_d));
  a->SetZ(0.0 * boost::units::si::meter);
  b->SetZ(1.0 * boost::units::si::meter);
  c->SetZ(0.0 * boost::units::si::meter);
  d->SetZ(1.0 * boost::units::si::meter);
  //a->SetIndex(1);
  //b->SetIndex(2);
  //c->SetIndex(3);
  //d->SetIndex(4);
  const std::vector<boost::shared_ptr<Point>> square { a,b,c,d };
  return square;
}

std::vector<boost::shared_ptr<ribi::trim::Point>> ribi::trim::PointFactory::CreateTestPrism() const noexcept
{
  const boost::shared_ptr<Coordinat2D> co_a { new Coordinat2D(0.0,0.0) };
  const boost::shared_ptr<Coordinat2D> co_b { new Coordinat2D(1.0,0.0) };
  const boost::shared_ptr<Coordinat2D> co_c { new Coordinat2D(0.0,1.0) };
  const boost::shared_ptr<Coordinat2D> co_d { new Coordinat2D(0.0,0.0) };
  const boost::shared_ptr<Coordinat2D> co_e { new Coordinat2D(1.0,0.0) };
  const boost::shared_ptr<Coordinat2D> co_f { new Coordinat2D(0.0,1.0) };

  const auto a(PointFactory().Create(co_a));
  const auto b(PointFactory().Create(co_b));
  const auto c(PointFactory().Create(co_c));
  const auto d(PointFactory().Create(co_d));
  const auto e(PointFactory().Create(co_e));
  const auto f(PointFactory().Create(co_f));
  a->SetZ(0.0 * boost::units::si::meter);
  b->SetZ(0.0 * boost::units::si::meter);
  c->SetZ(0.0 * boost::units::si::meter);
  d->SetZ(1.0 * boost::units::si::meter);
  e->SetZ(1.0 * boost::units::si::meter);
  f->SetZ(1.0 * boost::units::si::meter);
  const std::vector<boost::shared_ptr<Point>> prism {
    a,b,c,d,e,f
  };
  return prism;
}

std::vector<boost::shared_ptr<ribi::trim::Point>>
  ribi::trim::PointFactory::CreateTestSquare(
  const ribi::trim::Winding winding
) const noexcept
{
  /*

    Clockwise:

      Y
  2.5 |
    2 + 1-2
  1.5 | | |
    1 + 0-3 where Z = 1.0 for all points
  0.5 |
    0 +-+-+-X
      0 1 2

    Counter-clockwise:

     Y
 2.5 |
   2 + 3-2
 1.5 | | |
   1 + 0-1  where Z = 1.0 for all points
 0.5 |
   0 +-+-+-X
     0 1 2

    Indeterminate:

      Y
  2.3 |
    2 +  2--1
  1.7 |   \/
  1.3 |   /\
    1 +  0--3 where Z = 1.0 for all points
  0.7 |
  0.3 |
    0 +--+--+-X
      0  1  2

  */
  const boost::shared_ptr<Coordinat2D> co_a {
    new Coordinat2D(1.0,1.0)
  };
  boost::shared_ptr<Coordinat2D> co_b {
    new Coordinat2D(1.0,2.0)
  };
  boost::shared_ptr<Coordinat2D> co_c {
    new Coordinat2D(2.0,2.0)
  };
  boost::shared_ptr<Coordinat2D> co_d {
    new Coordinat2D(2.0,1.0)
  };

  if (winding == Winding::counter_clockwise)
  {
    std::swap(co_b,co_d);
  }
  else if (winding == Winding::indeterminate)
  {
    std::swap(co_b,co_c);
    std::swap(co_c,co_d);
  }

  const boost::shared_ptr<Point> a { PointFactory().Create(co_a) };
  const boost::shared_ptr<Point> b { PointFactory().Create(co_b) };
  const boost::shared_ptr<Point> c { PointFactory().Create(co_c) };
  const boost::shared_ptr<Point> d { PointFactory().Create(co_d) };

  a->SetZ(1.0 * boost::units::si::meter);
  b->SetZ(1.0 * boost::units::si::meter);
  c->SetZ(1.0 * boost::units::si::meter);
  d->SetZ(1.0 * boost::units::si::meter);
  const std::vector<boost::shared_ptr<Point>> square { a,b,c,d };
  assert(Helper().CalcWindingHorizontal(AddConst(square)) == winding);
  return square;
}

std::vector<boost::shared_ptr<ribi::trim::Point>>
  ribi::trim::PointFactory::CreateTestTriangle(
  const ribi::trim::Winding winding
) const noexcept
{
  assert( (winding == Winding::clockwise || winding == Winding::counter_clockwise)
    && "It is impossible to create a triangle with an indeterminate winding");

  /*

    Clockwise:

      Y
  2.5 |
    2 + 1-2
  1.5 | |/
    1 + 0   where Z = 1.0 for all points
  0.5 |
    0 +-+-+-X
      0 1 2

    Counter-clockwise:

      Y
  2.5 |
    2 + 2-1
  1.5 | |/
    1 + 0   where Z = 1.0 for all points
  0.5 |
    0 +-+-+-X
      0 1 2

  */

  const boost::shared_ptr<Coordinat2D> co_a {
    new Coordinat2D(1.0,1.0)
  };
  boost::shared_ptr<Coordinat2D> co_b {
    new Coordinat2D(1.0,2.0)
  };
  boost::shared_ptr<Coordinat2D> co_c {
    new Coordinat2D(2.0,2.0)
  };

  if (winding == Winding::counter_clockwise) { std::swap(co_b,co_c); }

  const boost::shared_ptr<Point> a {
    PointFactory().Create(co_a)
  };
  const boost::shared_ptr<Point> b {
    PointFactory().Create(co_b)
  };
  const boost::shared_ptr<Point> c {
    PointFactory().Create(co_c)
  };

  a->SetZ(1.0 * boost::units::si::meter);
  b->SetZ(1.0 * boost::units::si::meter);
  c->SetZ(1.0 * boost::units::si::meter);
  const std::vector<boost::shared_ptr<Point>> triangle { a,b,c };
  assert(Helper().IsClockwiseHorizontal(triangle)   == (winding == Winding::clockwise));
  return triangle;
}

#ifndef NDEBUG
void ribi::trim::PointFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  PointFactory().CreateTestTriangle(Winding::clockwise);
  
  {
    const std::vector<boost::shared_ptr<Point>> prism {
      PointFactory().CreateTestPrism()
    };
    assert(prism.size() == 6 && "A prism has 6 points");
  }
  for (Winding winding: { Winding::clockwise, Winding::counter_clockwise } )
  {
    
    assert(
      Helper().CalcWindingHorizontal(
        AddConst(PointFactory().CreateTestTriangle(winding))
      ) == winding
    );
  }
}
#endif
