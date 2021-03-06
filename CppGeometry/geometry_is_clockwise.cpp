#include "geometry.h"

#include <cassert>

#include "plane.h"

bool ribi::Geometry::IsClockwise(const double a, const double b) const noexcept
{
  const double pi  = boost::math::constants::pi<double>();
  const double tau = boost::math::constants::two_pi<double>();
  const double c { Fmod(a,tau) };
  const double d { Fmod(b,tau) };
  assert(c >= 0);
  assert(c < tau);
  assert(d >= 0);
  assert(d < tau);
  if (d - c >= 0.0 && d - c < pi) return true;
  if (d + tau - c >= 0.0 && d + tau - c < pi) return true;
  return false;
}

bool ribi::Geometry::IsClockwise(const Doubles& angles) const noexcept
{
  const int sz = static_cast<int>(angles.size());
  assert(sz >= 2 && "Need at least two angles to determine if these are clockwise");
  for (int i=0; i!=sz-1; ++i)
  {
    if (!IsClockwise(angles[i],angles[i+1])) return false;
  }

  //Calculate cumulative clockwise distance
  const double tau{boost::math::constants::two_pi<double>()};
  double sum{0.0};
  for (int i=0; i!=sz-1; ++i)
  {
    const double diff{angles[i+1] - angles[i]};
    if (diff > 0.0)
    {
      sum += diff;
    }
    else
    {
      assert(diff + tau > 0.0);
      sum += (diff + tau);
    }
  }
  return sum < tau;
}

bool ribi::Geometry::IsClockwiseCartesian(
  const Coordinats3D& points,
  const Coordinat3D& observer
) const noexcept
{
  // const bool verbose{false};
  const int n_points = static_cast<int>(points.size());
  assert(n_points == 3 || n_points == 4);
  if (n_points == 3)
  {
    const Coordinat3D a{points[0]};
    const Coordinat3D b{points[1]};
    const Coordinat3D c{points[2]};
    const Coordinat3D normal{CalcNormal(a,b,c)};
    const double direction{CalcDotProduct(normal,a - observer)};
    const bool is_clockwise { direction < 0.0 }; //Difference between CW ('<') and CCW ('>')
    return is_clockwise;
  }
  else
  {
    assert(n_points == 4);
    //See if the points in the projection are in the same direction
    assert(Geometry().IsPlane(points));
    const std::unique_ptr<Plane> plane(new Plane(points[0],points[1],points[2]));
    assert(plane);
    const auto v(
        plane->CalcProjection(
          {
            points[0],
            points[1],
            points[2],
            points[3]
          }
        )
    );
    //If the points are messed up, they cannot be clockwise
    if (!IsClockwiseCartesianHorizontal(v) && !IsCounterClockwiseCartesianHorizontal(v)) return false;
    //The neatly orderder points have the same winding as the first three
    std::remove_const<std::remove_reference<decltype(points)>::type>::type a;
    std::copy(points.begin() + 0,points.begin() + 3,std::back_inserter(a));
    return IsClockwiseCartesian(a,observer);
  }
}

bool ribi::Geometry::IsClockwiseCartesianHorizontal(const Coordinats3D& points) const noexcept
{
  // const bool verbose{false};
  using boost::geometry::get;
  const auto center = CalcCenter(points);
  Doubles angles;
  angles.reserve(points.size());
  std::transform(points.begin(),points.end(),
    std::back_inserter(angles),
    [this,center](const Coordinat3D& coordinat)
    {
      return GetAngleClockCartesian(
        get<0>(coordinat) - get<0>(center),
        get<1>(coordinat) - get<1>(center)
      );
    }
  );
  const bool is_clockwise = IsClockwise(angles);
  return is_clockwise;
}


bool ribi::Geometry::IsClockwiseCartesianHorizontal(const Coordinats2D& points) const noexcept
{
  //Points are determined from their center
  const auto center = CalcCenter(points);
  Doubles angles;
  std::transform(points.begin(),points.end(),
    std::back_inserter(angles),
    [this,center](const auto& coordinat)
    {
      return this->GetAngleClockCartesian(
        coordinat.x() - center.x(),
        coordinat.y() - center.y()
      );
    }
  );
  return IsClockwise(angles);
}
