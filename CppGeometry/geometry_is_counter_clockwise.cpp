#include "geometry.h"

#include <cassert>

#include "plane.h"

bool ribi::Geometry::IsCounterClockwise(const double a, const double b) const noexcept
{
  return !IsClockwise(a,b);
}

bool ribi::Geometry::IsCounterClockwise(const Doubles& angles) const noexcept
{
  const int sz = static_cast<int>(angles.size());
  assert(sz >= 2 && "Need at least two angles to determine if these are counter-clockwise");
  for (int i=0; i!=sz-1; ++i)
  {
    if (!IsCounterClockwise(angles[i],angles[i+1])) return false;
  }

  //Calculate cumulative clockwise distance
  const double tau{boost::math::constants::two_pi<double>()};
  double sum{0.0};
  for (int i=0; i!=sz-1; ++i)
  {
    const double diff{angles[i] - angles[i+1]}; //Order reversed compared to IsClockwise
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

bool ribi::Geometry::IsCounterClockwiseCartesian(
  const Coordinats3D& points,
  const Coordinat3D& observer
) const noexcept
{
  // const bool verbose{false};
  const int n_points{static_cast<int>(points.size())};
  assert(n_points == 3 || n_points == 4);
  if (n_points == 3)
  {
    const auto a = points[0];
    const auto b = points[1];
    const auto c = points[2];
    const auto normal = CalcNormal(a,b,c);
    const double direction{CalcDotProduct(normal,a - observer)};
    const bool is_counter_clockwise{direction > 0.0}; //Difference between CW ('<') and CCW ('>')
    return is_counter_clockwise;
  }
  else
  {
    assert(n_points == 4);
    //See if the points in the projection are in the same direction
    assert(Geometry().IsPlane(points));
    const std::unique_ptr<Plane> plane(new Plane(points[0],points[1],points[2]));
    assert(plane);
    const auto v =
      plane->CalcProjection(
        {
          points[0],
          points[1],
          points[2],
          points[3]
        }
      )
    ;

    //If the points are messed up, they cannot be clockwise
    if (!IsClockwiseCartesianHorizontal(v) && !IsCounterClockwiseCartesianHorizontal(v)) return false;
    //The neatly orderder point have the same winding as the first three
    std::remove_const<std::remove_reference<decltype(points)>::type>::type a;
    std::copy(points.begin() + 0,points.begin() + 3,std::back_inserter(a));
    return IsCounterClockwiseCartesian(a,observer);
  }

}

bool ribi::Geometry::IsCounterClockwiseCartesianHorizontal(const Coordinats2D& points) const noexcept
{
  //Points are determined from their center
  const auto center(CalcCenter(points));
  Doubles angles;
  angles.reserve(points.size());
  std::transform(points.begin(),points.end(),
    std::back_inserter(angles),
    [this,center](const Coordinat2D& coordinat)
    {
      return GetAngleClockCartesian(
        coordinat - center
      );
    }
  );
  return IsCounterClockwise(angles);
}

bool ribi::Geometry::IsCounterClockwiseCartesianHorizontal(const Coordinats3D& points3d) const noexcept
{
  Coordinats2D points2d;
  for (const auto& point3d: points3d)
  {
    points2d.push_back(
      {
        boost::geometry::get<0>(point3d),
        boost::geometry::get<1>(point3d)
      }
    );
  }
  return IsCounterClockwiseCartesianHorizontal(points2d);
}
