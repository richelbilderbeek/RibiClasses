


#include "textcanvas.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>

#include <boost/math/constants/constants.hpp>

#include "dotmatrixstring.h"




ribi::TextCanvas::TextCanvas(
  const int width,
  const int height,
  const CanvasCoordinatSystem coordinatSystem)
  : m_canvas(std::vector<std::string>(height,std::string(width,' '))),
    m_coordinat_system(coordinatSystem)
{
  assert(width  > 0);
  assert(height > 0);
}

ribi::TextCanvas::TextCanvas(
    const std::vector<std::string>& canvas,
    const CanvasCoordinatSystem coordinatSystem
) : m_canvas(canvas),
    m_coordinat_system(coordinatSystem)
{
}

void ribi::TextCanvas::Clear() noexcept
{
  for (auto& row: m_canvas)
  {
    for (auto& cell:row)
    {
      cell = ' ';
    }
  }

  #ifndef NDEBUG
  for (const auto& row: m_canvas)
  {
    assert(std::count(row.begin(),row.end(),' ') == static_cast<int>(row.size()));
  }
  #endif
}

char ribi::TextCanvas::GetChar(const int x, const int y) const noexcept
{
  assert(IsInRange(x,y));
  return m_canvas[y][x];
}

bool ribi::TextCanvas::IsInRange(const int x, const int y) const noexcept
{
  if (   x < 0
      || y < 0
      || y >= static_cast<int>(m_canvas.size())
      || x >= static_cast<int>(m_canvas[y].size())
     )
    return false;
  return true;
}

std::string ribi::TextCanvas::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::TextCanvas::GetVersionHistory() noexcept
{
  return {
    "2014-01-09: version 1.0: initial version"
  };
}

void ribi::TextCanvas::PutCanvas(
  const int left, const int top,
  const TextCanvas& canvas
) noexcept
{
  const int height { canvas.GetHeight() };
  const int width { canvas.GetWidth() };
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      PutChar(left + x, top + y,canvas.GetChar(x,y));
    }
  }
}

void ribi::TextCanvas::PutChar(const int x, const int y, const char c) noexcept
{
  if(!IsInRange(x,y)) return;
  if(m_canvas[y][x] != c)
  {
    m_canvas[y][x] = c;
  }
}

void ribi::TextCanvas::PutText(const int x, const int y, const std::string& text) noexcept
{
  int i=0;
  for (const auto& c: text)
  {
    const int x_here = x + i;
    const int y_here = y;
    if (IsInRange(x_here,y_here))
    {
      PutChar(x_here,y_here,c);
    }
    ++i;
  }
}

void ribi::TextCanvas::SetCoordinatSystem(const CanvasCoordinatSystem coordinatSystem) noexcept
{
  if (this->m_coordinat_system != coordinatSystem)
  {
    this->m_coordinat_system = coordinatSystem;
  }
}

std::string ribi::TextCanvas::ToString() const noexcept
{
  const std::vector<std::string> v { ToStrings() };
  std::string s;
  for (const auto& t: v)
  {
    s += t;
    s += '\n';
  }
  if (!s.empty())
  {
    //Remove the trailing '\n'
    s.pop_back();
  }
  return s;
}


std::vector<std::string> ribi::TextCanvas::ToStrings() const noexcept
{
  if (m_coordinat_system == CanvasCoordinatSystem::screen)
  {
    return m_canvas;
  }
  else
  {
    std::vector<std::string> v(m_canvas);
    std::reverse(std::begin(v),std::end(v));
    return v;
  }
}

std::ostream& ribi::operator<<(std::ostream& os, const TextCanvas& canvas)
{
  const auto v = canvas.ToStrings();
  std::copy(v.begin(),v.end(),
    std::ostream_iterator<std::string>(os,"\n")
  );
  return os;
}

bool ribi::operator==(const TextCanvas& lhs, const TextCanvas& rhs) noexcept
{
  return lhs.ToString() == rhs.ToString();
}

bool ribi::operator!=(const TextCanvas& lhs, const TextCanvas& rhs) noexcept
{
  return !(lhs == rhs);
}
