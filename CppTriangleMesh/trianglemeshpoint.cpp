#include "trianglemeshpoint.h"

#include <iostream>
#include <iomanip>
#include <sstream>




#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "geometry.h"

#include "trianglemeshface.h"
#include "trianglemeshhelper.h"

#include "xml.h"


ribi::trim::Point::Point(
  const boost::shared_ptr<const Coordinat2D> coordinat,
  const int index,
  const PointFactory&
) :
    m_connected{},
    m_coordinat(coordinat),
    m_index{index},
    m_z{}
{
  #ifndef NDEBUG
  Test();
  #endif
  using boost::geometry::get;
  assert(m_coordinat);
  assert(m_coordinat == coordinat && "A shallow copy please");
  assert(!std::isnan(get<0>(*m_coordinat)));
  assert(!std::isnan(get<1>(*m_coordinat)));
}

ribi::trim::Point::~Point() noexcept
{

}

void ribi::trim::Point::AddConnected(const boost::weak_ptr<Face>& face)
{
  assert(face.lock().get() != nullptr);
  m_connected.push_back(face);
}

ribi::trim::Point::Coordinat3D ribi::trim::Point::GetCoordinat3D() const noexcept
{
  assert(!std::isnan(boost::geometry::get<0>(*GetCoordinat())));
  assert(!std::isnan(boost::geometry::get<1>(*GetCoordinat())));
  assert(!CanGetZ() || !std::isnan(GetZ().value()));
  
  return Geometry().CreatePoint(
    boost::geometry::get<0>(*GetCoordinat()),
    boost::geometry::get<1>(*GetCoordinat()),
    CanGetZ() ? GetZ().value() : 0.0
  );
}

bool ribi::trim::Point::CanGetZ() const noexcept
{
  return m_z.get();
}

boost::units::quantity<boost::units::si::length> ribi::trim::Point::GetZ() const noexcept
{
  assert(CanGetZ());
  return *m_z;
}

void ribi::trim::Point::OnFaceDestroyed(const ribi::trim::Face * const face) noexcept
{
  assert(1==2);
  const auto new_end = std::remove_if(m_connected.begin(),m_connected.end(),
    [face](const boost::weak_ptr<Face>& connected) { return connected.lock().get() == face; }
  );
  m_connected.erase(new_end,m_connected.end());
}

std::function<
    bool(
      const boost::shared_ptr<const ribi::trim::Face>& lhs,
      const boost::shared_ptr<const ribi::trim::Face>& rhs
    )
  >
  ribi::trim::Point::OrderByIndex() const noexcept
{
  return [](const boost::shared_ptr<const Face>& lhs, const boost::shared_ptr<const Face>& rhs)
  {
    assert(lhs);
    assert(rhs);
    assert(lhs->GetIndex() != rhs->GetIndex());
    return lhs->GetIndex() < rhs->GetIndex();
  };
}

void ribi::trim::Point::SetZ(const boost::units::quantity<boost::units::si::length> z) const noexcept
{
  if (m_z)
  {
    assert(*m_z == z);
    return;
  }
  //assert(!m_z&& "m_z can be set exactly once");
  boost::shared_ptr<boost::units::quantity<boost::units::si::length>> p {
    new boost::units::quantity<boost::units::si::length>(z)
  };
  m_z = p;
  assert(m_z);

  //Let the Point check for themselves for being horizontal or vertical
  #ifndef NDEBUG
  if (GetConnected().empty()) return;
  for (const auto& face: GetConnected())
  {
    assert(face.lock());
    face.lock()->CheckOrientation();
  }
  #endif
}

#ifndef NDEBUG
void ribi::trim::Point::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif

std::string ribi::trim::Point::ToStr() const noexcept
{

  std::stringstream s;
  s << *this;
  return s.str();
}

std::string ribi::trim::Point::ToXml() const noexcept
{
  
  std::stringstream s;
  s
    << ribi::xml::ToXml("point_index",GetIndex())
    << Helper().ToXml(*GetCoordinat())
    << ribi::xml::ToXml("z", CanGetZ()
      ?  boost::lexical_cast<std::string>(GetZ().value())
      : ""
      )
  ;
  return s.str();
}

bool ribi::trim::operator==(const ribi::trim::Point& lhs, const ribi::trim::Point& rhs) noexcept
{
  return lhs.GetCoordinat() == rhs.GetCoordinat()
  ;
}

bool ribi::trim::operator!=(const ribi::trim::Point& lhs, const ribi::trim::Point& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::trim::operator<<(std::ostream& os, const Point& n) noexcept
{
  using boost::geometry::get;
  const auto c = n.GetCoordinat3D();
  os
    << "("
    << get<0>(c)
    << ","
    << get<1>(c)
    << ","
    << get<2>(c)
    << ") (index: "
    << n.GetIndex()
    << ")"
  ;
  return os;
}
