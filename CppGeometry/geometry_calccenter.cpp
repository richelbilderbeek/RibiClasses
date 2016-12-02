#include "geometry.h"

ribi::Geometry::Coordinat2D ribi::Geometry::CalcCenter(const Coordinats2D& points) const noexcept
{
  Coordinat2D sum(0.0,0.0);
  for (const auto& point: points)
  {
    sum.x(sum.x() + point.x());
    sum.y(sum.y() + point.y());
  }
  const double n(static_cast<double>(points.size()));
  const Coordinat2D center(
    sum.x() / n,
    sum.y() / n
  );
  return center;

}

ribi::Geometry::Coordinat3D ribi::Geometry::CalcCenter(const Coordinats3D& points) const noexcept
{
  double sum_x = 0.0;
  double sum_y = 0.0;
  double sum_z = 0.0;
  for (const auto& point: points)
  {
    sum_x += boost::geometry::get<0>(point);
    sum_y += boost::geometry::get<1>(point);
    sum_z += boost::geometry::get<2>(point);
  }
  const double n{static_cast<double>(points.size())};
  const Coordinat3D center(
    sum_x / n,
    sum_y / n,
    sum_z / n
  );
  return center;
}
