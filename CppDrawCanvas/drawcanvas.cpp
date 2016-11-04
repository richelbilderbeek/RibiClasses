//---------------------------------------------------------------------------
/*
DrawCanvas, ASCII art painting surface class
Copyright (C) 2008-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
// From http://www.richelbilderbeek.nl/CppDrawCanvas.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "drawcanvas.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>

#include <boost/algorithm/string/split.hpp>
#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>

#include <QString>

#include "canvascolorsystems.h"
#include "canvascoordinatsystems.h"
#include "container.h"
#include "dotmatrixstring.h"
#include "fileio.h"
#include "geometry.h"
#include "ribi_regex.h"
#include "xml.h"

#pragma GCC diagnostic pop

ribi::DrawCanvas::DrawCanvas(
  const int width,
  const int height,
  const CanvasColorSystem color_system,
  const CanvasCoordinatSystem coordinat_system)
  :
    m_canvas(std::vector<std::vector<double>>(height,std::vector<double>(width,0.0))),
    m_color_system(color_system),
    m_coordinat_system(coordinat_system)
{
  assert(width  > 0);
  assert(height > 0);
}

ribi::DrawCanvas::DrawCanvas(
  const std::vector<std::vector<double>>& canvas,
  const CanvasColorSystem color_system,
  const CanvasCoordinatSystem coordinat_system)
  : m_canvas(canvas),
    m_color_system(color_system),
    m_coordinat_system(coordinat_system)
{
  assert(!canvas.empty());
  assert(!canvas[0].empty());
}

ribi::DrawCanvas::DrawCanvas(const std::string& filename)
  : m_canvas{},
    m_color_system{},
    m_coordinat_system{}
{
  assert(fileio::FileIo().IsRegularFile(filename));
  std::string s;
  {
    std::ifstream f(filename.c_str());
    f >> s;
  }
  assert(s.size() >= 17);
  assert(s.substr(0,8) == "<canvas>");
  assert(s.substr(s.size() - 9,9) == "</canvas>");
  {
    const std::vector<std::string> v { Regex().GetRegexMatches(s,"(<color_system>.*</color_system>)") };
    assert(v.size() == 1);
    m_color_system = CanvasColorSystems().ToType(ribi::xml::StripXmlTag(v[0]));
  }

  {
    const std::vector<std::string> v { Regex().GetRegexMatches(s,"(<coordinat_system>.*</coordinat_system>)") };
    assert(v.size() == 1);
    m_coordinat_system = CanvasCoordinatSystems().ToType(ribi::xml::StripXmlTag(v[0]));
  }
  int n_cols = -1;
  {
    const std::vector<std::string> v { Regex().GetRegexMatches(s,"(<n_cols>.*</n_cols>)") };
    assert(v.size() == 1);
    //assert(CanCast<int>(ribi::xml::StripXmlTag(v[0])));
    n_cols = boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
  }

  m_canvas.push_back( {} );
  {
    const std::vector<std::string> v { Regex().GetRegexMatches(s,"(<data>.*</data>)") };
    assert(v.size() == 1 && "(<data>.*</data>) must be present exactly once");
    const std::pair<std::string,std::vector<std::string>> lines { xml::XmlToVector(v[0]) };
    assert(lines.first == "data");
    const std::vector<std::string>& data { lines.second };
    int i = 0;
    for (const std::string& s: data)
    {
      const double d = boost::lexical_cast<double>(s);

      m_canvas.back().push_back(d);
      ++i;
      if (i == n_cols)
      {
        m_canvas.push_back( {} );
        i = 0;
      }
    }
  }
  assert(m_canvas.back().empty());
  m_canvas.pop_back();
}

void ribi::DrawCanvas::Clear() noexcept
{
  for (auto& row: m_canvas)
  {
    for (auto& cell:row)
    {
      cell = 0.0;
    }
  }

  #ifndef NDEBUG
  for (const auto& row: m_canvas)
  {
    assert(std::accumulate(row.begin(),row.end(),0.0) == 0.0);
  }
  #endif
}

void ribi::DrawCanvas::DrawArc(
  const double left, const double top, const double right, const double bottom,
    const boost::units::quantity<boost::units::si::plane_angle> startAngle,
    const boost::units::quantity<boost::units::si::plane_angle> spanAngle) noexcept
{
  assert(left < right);
  assert(top < bottom);
  const double midx = (left + right) / 2.0;
  const double midy = (top + bottom) / 2.0;
  const double pi = boost::math::constants::pi<double>();
  const double ray_horizontal = (right  - left) / 2.0;
  const double ray_vertical   = (bottom - top ) / 2.0;
  const double average_ray    = (ray_horizontal + ray_vertical) / 2.0;
  const double arclength = average_ray * pi * 2.0 * (spanAngle.value() / (2.0 * pi));
  const int n_steps = std::abs(static_cast<int>(arclength + 0.5));
  if (n_steps == 0) return;
  assert(n_steps > 0);
  double angle { startAngle.value() };
  const double dAngle = spanAngle.value() / static_cast<double>(n_steps);
  for (int i=0; i!=n_steps; ++i)
  {
    double x = midx + (std::sin(angle) * ray_horizontal);
    double y = midy - (std::cos(angle) * ray_vertical);
    DrawDot(x,y);
    angle += dAngle;
  }
}

void ribi::DrawCanvas::DrawCircle(const double xMid, const double yMid, const double ray) noexcept
{
  const double pi = boost::math::constants::pi<double>();
  const double circumference = ray * pi * 2.0;
  const int n_steps = static_cast<int>(circumference + 0.5);
  if (n_steps == 0) return;
  assert(n_steps > 0);
  const double dAngle = 2.0 * pi / static_cast<double>(n_steps);
  double angle = 0.0;
  for (int i=0; i!=n_steps; ++i)
  {
    double x = xMid + (std::sin(angle) * ray);
    double y = yMid - (std::cos(angle) * ray);
    DrawDot(x,y);
    angle += dAngle;
  }
}

void ribi::DrawCanvas::DrawDot(const double x, const double y) noexcept
{
  //Assume a dot has dimensions 1.0 x 1.0
  //and x and y are exactly in the middle of this dot
  const double xBegin = x - 0.5;
  const double yBegin = y - 0.5;
  const double fracLeft = std::ceil(xBegin) - xBegin;
  const double fracTop  = std::ceil(yBegin) - yBegin;
  const int indexLeft = std::floor(xBegin);
  const int indexTop  = std::floor(yBegin);
  if (IsInRange(indexLeft  ,indexTop  ))
    m_canvas[indexTop  ][indexLeft  ] += (fracLeft * fracTop);
  if (IsInRange(indexLeft+1,indexTop  ))
    m_canvas[indexTop  ][indexLeft+1] += ((1.0-fracLeft) * fracTop);
  if (IsInRange(indexLeft  ,indexTop+1))
    m_canvas[indexTop+1][indexLeft  ] += (fracLeft * (1.0-fracTop));
  if (IsInRange(indexLeft+1,indexTop+1))
    m_canvas[indexTop+1][indexLeft+1] += ((1.0-fracLeft) * (1.0-fracTop));
}

void ribi::DrawCanvas::DrawEllipse(const double left, const double top, const double right, const double bottom) noexcept
{
  assert(left < right);
  assert(top < bottom);
  const double midx = (left + right) / 2.0;
  const double midy = (top + bottom) / 2.0;
  assert(midx > 0.0);
  const double pi = boost::math::constants::pi<double>();
  const double ray_horizontal = (right  - left) / 2.0;
  const double ray_vertical   = (bottom - top ) / 2.0;
  assert(ray_horizontal > 0.0);
  assert(ray_vertical > 0.0);
  const double average_ray    = (ray_horizontal + ray_vertical) / 2.0;
  assert(average_ray > 0.0);
  const double circumference = average_ray * pi * 2.0;
  const int n_steps = static_cast<int>(circumference + 0.5);
  assert(n_steps > 0);
  const double d_angle = 2.0 * pi / static_cast<double>(n_steps);
  assert(d_angle > 0.0);
  double angle = 0.0;
  for (int i=0; i!=n_steps; ++i)
  {
    const double x = midx + (std::sin(angle) * ray_horizontal);
    const double y = midy - (std::cos(angle) * ray_vertical);
    DrawDot(x,y);
    angle += d_angle;
  }
}

void ribi::DrawCanvas::DrawSurface(const std::vector<std::vector<double>>& v)
{
  if (m_canvas != v)
  {
    m_canvas = v;
  }
}

void ribi::DrawCanvas::DrawLine(const double x1, const double y1, const double x2, const double y2) noexcept
{

  const double dx = x2 - x1;
  const double dy = y2 - y1;
  const double dist = Geometry().GetDistance(dx,dy);
  const double step_x = dx / dist;
  const double step_y = dy / dist;
  const int n_steps = static_cast<int>(dist + 0.5);
  double x = x1;
  double y = y1;
  for (int i=0; i!=n_steps; ++i)
  {
    DrawDot(x,y);
    x+=step_x;
    y+=step_y;
  }
}

void ribi::DrawCanvas::DrawLine(
  const ribi::DrawCanvas::Coordinat from,
  const ribi::DrawCanvas::Coordinat to
) noexcept
{
  DrawLine(from.x(),from.y(),to.x(),to.y());
}

void ribi::DrawCanvas::DrawPolygon(
  const boost::geometry::model::polygon<ribi::DrawCanvas::Coordinat>& polygon
) noexcept
{
  const std::vector<Coordinat> points = polygon.outer();
  const int n = static_cast<int>(points.size());
  for (int i=0; i!=n; ++i)
  {
    DrawLine(points[i],points[ (i + 1) % n]);
  }
}

void ribi::DrawCanvas::DrawText(const double top, const double left, const std::string& text) noexcept
{
  const int spacing = 2;
  const DotMatrixString m(text,spacing);
  const int width  = m.GetMatrixWidth();
  const int height = m.GetMatrixHeight();
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      if (m.GetMatrix(x,y))
      {
        DrawDot(
          left + static_cast<double>(x) + 0.5,
          top  + static_cast<double>(y) + 0.5
        );
      }
    }
  }
}

std::string ribi::DrawCanvas::GetVersion() noexcept
{
  return "4.0";
}

std::vector<std::string> ribi::DrawCanvas::GetVersionHistory() noexcept
{
  return {
    "2008-xx-xx: version 1.0: initial C++ Builder version, initially called Canvas",
    "2013-08-21: version 2.0: port to C++11 under Qt Creator",
    "2013-08-22: version 2.1: allow two color and coordinat systems"
    "2014-01-07: version 2.2: added the DrawText member function",
    "2014-01-10: version 3.0: renamed to DrawCanvas, inherits from new class called Canvas",
    "2014-05-10: version 3.1: allow to draw a Boost.Geometry polygon, increase support for Boost.Geometry",
    "2015-11-21: version 4.0: made this class a regular type",
  };
}

bool ribi::DrawCanvas::IsInRange(const int x, const int y) const
{
  if (   x < 0
      || y < 0
      || y >= static_cast<int>(m_canvas.size())
      || x >= static_cast<int>(m_canvas[y].size())
     )
    return false;
  return true;
}

void ribi::DrawCanvas::PlotSurface(
  std::ostream& os,
  const std::vector<std::vector<double>>& v,
  const bool use_normal_color_system,
  const bool as_screen_coordinat_system)
{
  assert(v.empty() == false && "Surface must have a size");
  assert(v[0].size() > 0 && "Surface must have a two-dimensional size");

  //Obtain the ASCII art gradient and its size
  static const std::vector<char> asciiArtGradient = GetAsciiArtGradient();
  static const int nAsciiArtGradientChars = asciiArtGradient.size();

  //Minimum and maximum are not given, so these need to be calculated
  const double minVal = MinElement(v);
  double maxVal = MaxElement(v);
  if (minVal == maxVal)
  {
    maxVal = minVal == 0.0 ? 1.0 : minVal * 2.0;
  }

  //Draw the pixels

  const auto row_function(
    [](const std::vector<double>& row,
      std::ostream& os,
      const double minVal,
      const double maxVal,
      const bool use_normal_color_system)
    {
      //Iterate through each row's columns
      const std::vector<double>::const_iterator colEnd = row.end();
      for (std::vector<double>::const_iterator col = row.begin();
        col != colEnd;
        ++col)
      {
        //Scale the found grey value to an ASCII art character
        assert(maxVal != minVal);
        assert(maxVal - minVal != 0.0);
        assert(maxVal > minVal);
        const double greyValueDouble = ( (*col) - minVal) / (maxVal - minVal);
        assert(greyValueDouble >= 0.0 && greyValueDouble <= 1.0);
        const int greyValueInt
          = (use_normal_color_system
          ? greyValueDouble
          : 1.0 - greyValueDouble
          ) * nAsciiArtGradientChars;
        const int greyValue
          = ( greyValueInt < 0
          ? 0 : (greyValueInt > nAsciiArtGradientChars - 1
            ? nAsciiArtGradientChars - 1: greyValueInt) );
        assert(greyValue >= 0 && greyValue < nAsciiArtGradientChars);
        os << asciiArtGradient[greyValue];
      }
      os << std::endl;

    }
  );

  //Iterator through all rows
  if (as_screen_coordinat_system)
  {
    for (const auto& row: v)
    {
      row_function(row,os,minVal,maxVal,use_normal_color_system);
    }
  }
  else
  {
    const auto rowEnd = v.rend();
    for (auto row = v.rbegin(); row != rowEnd; ++row)
    {
      row_function(*row,os,minVal,maxVal,use_normal_color_system);
    }
  }
}

template <class Container>
const typename Container::value_type::value_type ribi::DrawCanvas::MinElement(const Container& v)
{
  assert(!v.empty() && "Container must have a size");
  //Obtain an initial lowest value
  typename Container::value_type::value_type minValue
    = *(std::min_element(std::begin(v[0]),std::end(v[0])));

  //Set the iterators
  const typename Container::const_iterator rowEnd = v.end();
  typename Container::const_iterator row = v.begin();
  ++row; //Move to the next position, as index 0 is already read from

  for ( ; row != rowEnd; ++row) //row is already initialized
  {
    const typename Container::value_type::value_type localMinVal
      = *(std::min_element(row->begin(),row->end()));
    if (localMinVal < minValue) minValue = localMinVal;
  }
  return minValue;
}

template <class Container>
const typename Container::value_type::value_type ribi::DrawCanvas::MaxElement(const Container& v)
{
  assert(v.empty() == false && "Container must have a size");

  //Obtain an initial heighest value
  typename Container::value_type::value_type maxValue
    = *(std::max_element(v[0].begin(),v[0].end()));

  //Set the iterators
  const typename Container::const_iterator rowEnd = v.end();
  typename Container::const_iterator row = v.begin();
  ++row; //Move to the next position, as index 0 is already read from
  for ( ; row != rowEnd; ++row) //row is already initialized
  {
    const typename Container::value_type::value_type localMaxVal
      = *(std::max_element(row->begin(),row->end()));
    if (localMaxVal > maxValue) maxValue = localMaxVal;
  }
  return maxValue;
}

void ribi::DrawCanvas::Save(const std::string& filename) const noexcept
{
  std::stringstream s;
  {
    std::vector<std::string> v;
    for (const auto& line: m_canvas)
    {
      for (const auto& element: line)
      {
        const std::string t { boost::lexical_cast<std::string>(element) };
        v.push_back(t);
      }
    }
    s << xml::VectorToXml("data",v);
    s << xml::ToXml("n_cols",GetWidth());
    //color system
    s << xml::ToXml("color_system",CanvasColorSystems().ToStr(m_color_system));
    //coordinat system
    s << xml::ToXml("coordinat_system",CanvasCoordinatSystems().ToStr(m_coordinat_system));
  }
  {
    const std::string t = xml::ToXml("canvas",s.str());
    std::ofstream f(filename.c_str());
    f << t;
  }
}

void ribi::DrawCanvas::SetColorSystem(const CanvasColorSystem colorSystem) noexcept
{
  if (this->m_color_system != colorSystem)
  {
    this->m_color_system = colorSystem;
  }
}

void ribi::DrawCanvas::SetCoordinatSystem(const CanvasCoordinatSystem coordinatSystem) noexcept
{
  if (this->m_coordinat_system != coordinatSystem)
  {
    this->m_coordinat_system = coordinatSystem;
  }
}

std::vector<std::string> ribi::DrawCanvas::ToStrings() const noexcept
{
  std::stringstream s;
  s << (*this);
  const auto v = Container().SeperateString(s.str(),'\n');
  return v;
}

std::ostream& ribi::operator<<(std::ostream& os, const DrawCanvas& canvas)
{
  ribi::DrawCanvas::PlotSurface(
    os,
    canvas.m_canvas,
    canvas.m_color_system == ribi::CanvasColorSystem::normal,
    canvas.m_coordinat_system == ribi::CanvasCoordinatSystem::screen
  );
  return os;
}

bool ribi::operator==(const DrawCanvas& lhs, const DrawCanvas& rhs) noexcept
{
  return lhs.m_canvas == rhs.m_canvas
    && lhs.m_color_system == rhs.m_color_system
    && lhs.m_coordinat_system == rhs.m_coordinat_system;
}

bool ribi::operator!=(const DrawCanvas& lhs, const DrawCanvas& rhs) noexcept
{
  return !(lhs == rhs);
}

bool ribi::IsAboutEqual(const DrawCanvas& lhs, const DrawCanvas& rhs) noexcept
{
  if (lhs.GetColorSystem() != rhs.GetColorSystem())
  {
    return false;
  }
  if (lhs.GetCoordinatSystem() != rhs.GetCoordinatSystem())
  {
    return false;
  }

  if (lhs.GetGreynesses().size() != rhs.GetGreynesses().size())
  {
    return false;
  }
  const std::size_t n_rows = lhs.GetGreynesses().size();
  for (std::size_t row=0; row!=n_rows; ++row)
  {
    const std::vector<double>& v { lhs.GetGreynesses()[row] };
    const std::vector<double>& w { rhs.GetGreynesses()[row] };
    if (v.size() != w.size())
    {
      return false;
    }
    const std::size_t n_cols = v.size();
    for (std::size_t col=0; col!=n_cols; ++col)
    {
      const double diff = std::abs(v[col] - w[col]);
      if (diff > 0.01)
      {
        return false;
      }
    }
  }
  return true;
}
