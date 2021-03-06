


#include "imagecanvas.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <sstream>
#include <boost/math/constants/constants.hpp>

#include <QFile>
#include <QImage>

#include "canvascolorsystems.h"
#include "canvascoordinatsystems.h"
#include "fileio.h"


ribi::ImageCanvas::ImageCanvas() noexcept
  : Canvas{},
    m_canvas{},
    m_color_system{},
    m_coordinat_system{},
    m_n_cols{0}
{

}

ribi::ImageCanvas::ImageCanvas(
  const std::string& filename,
  const int n_cols,
  const ribi::CanvasColorSystem colorSystem,
  const ribi::CanvasCoordinatSystem coordinatSystem) noexcept
  : m_canvas{ConvertToGreyYx(filename)},
    m_color_system(colorSystem),
    m_coordinat_system(coordinatSystem),
    m_n_cols(n_cols)
{

}

ribi::ImageCanvas::ImageCanvas(
  const ImageCanvas& other) noexcept
  : m_canvas{other.m_canvas},
    m_color_system{other.m_color_system},
    m_coordinat_system{other.m_coordinat_system},
    m_n_cols{other.m_n_cols}
{

}

ribi::ImageCanvas& ribi::ImageCanvas::operator=(
  const ImageCanvas& other) noexcept
{
  const_cast<std::vector<std::vector<double>>&>(m_canvas) = other.m_canvas;
  //m_canvas = other.m_canvas;
  m_color_system = other.m_color_system;
  m_coordinat_system = other.m_coordinat_system;
  const_cast<int&>(m_n_cols) = other.m_n_cols;
  return *this;
}

std::vector<std::string> ribi::ImageCanvas::ConvertGreynessesToAscii(
  const std::vector<std::vector<double> >& image,
  const int width //How many chars the ASCII image will be wide
) noexcept
{
  if (width == 0) return std::vector<std::string>{};

  //If the number of chars is below 5,
  //the calculation would be more complicated due to a too trivial value of charWidth
  assert(width >= 5);

  std::vector<std::string> v;
  if (image.empty()) return v;

  assert(!image.empty());

  //Maxy is in proportion with the bitmap
  const int image_width  = image[0].size();
  const int image_height = image.size();

  const int maxy =
    (static_cast<double>(width)
    / static_cast<double>(image_width))
    * static_cast<double>(image_height) / 1.5; //Characters are 1.5 higher than wide
  assert(maxy > 0);
  const double dX = static_cast<double>(image_width)
    / static_cast<double>(width);
  const double dY = static_cast<double>(image_height)
    / static_cast<double>(maxy);
  assert(dX > 0.0);
  assert(dY > 0.0);

  for (int y=0; y!=maxy; ++y)
  {
    std::string s;
    for (int x=0; x!=width; ++x)
    {
      const int x1 = std::min(
        static_cast<double>(x) * dX,
        image_width  - 1.0) + 0.5;
      const int y1 = std::min(
        static_cast<double>(y) * dY,
        image_height - 1.0) + 0.5;
      const int x2 = std::min(
        (static_cast<double>(x) * dX) + dX,
        image_width  - 1.0) + 0.5;
      const int y2 = std::min(
        (static_cast<double>(y) * dY) + dY,
        image_height - 1.0) + 0.5;
      assert(x1 >= 0);
      assert(x2 >= 0);
      assert(y1 >= 0);
      assert(y2 >= 0);
      assert(x1 < image_width);
      assert(x2 < image_width);
      assert(y1 < image_height);
      assert(y2 < image_height);

      const double f = GetFractionGrey(image,x1,y1,x2,y2);
      assert(f >= 0.0 && f <= 1.0);

      const std::vector<char> m_gradient {
        GetAsciiArtGradient()
      };
      const int i
        = boost::numeric_cast<int>(
          f * boost::numeric_cast<double>(m_gradient.size() - 1));
      assert(i >= 0);
      assert(i < boost::numeric_cast<int>(m_gradient.size()));
      const char c = m_gradient[i];
      s+=c;
    }
    v.push_back(s);
  }
  return v;
}

std::vector<std::vector<double>>
  ribi::ImageCanvas::ConvertToGreyYx(const QImage& i) noexcept
{
  const int maxy = i.height();
  const int maxx = i.width();

  std::vector<std::vector<double> > v;

  if (maxx == 0) return v;
  assert(maxx > 0);
  const int n_bytes = i.bytesPerLine() / maxx;

  for (int y=0; y!=maxy; ++y)
  {
    v.push_back(std::vector<double>());
    const unsigned char * const line = i.scanLine(y);
    for (int x=0; x!=maxx; ++x)
    {
      int sum = 0;
      for (int byte=0; byte!=n_bytes; ++byte)
      {
        sum += line[(x * n_bytes) + byte];
      }
      const double greyness
        = (boost::numeric_cast<double>(sum)
        / boost::numeric_cast<double>(n_bytes))
        / 256.0;
      assert(greyness >= 0.0);
      assert(greyness <= 1.0);
      v.back().push_back(greyness);
    }
  }
  return v;
}

std::vector<std::vector<double>>
  ribi::ImageCanvas::ConvertToGreyYx(const std::string& filename) noexcept
{
  const QImage qimage(filename.c_str());
  return ConvertToGreyYx(qimage);
}

double ribi::ImageCanvas::GetFractionGrey(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int y1,
  const int x2,
  const int y2) noexcept
{
  assert(x1 <= x2);
  assert(y1 <= y2);
  if (x1 == x2 && y1 == y2) return GetGreyness(image,x1,y1);
  if (y1 == y2) return GetGreyness(image,x1,x2,y1);
  if (x1 == x2)
  {
    assert(y1 < y2);
    double sum = 0;
    for (int y=y1; y!=y2; ++y)
    {
      const double g = GetGreyness(image,x1,y);
      assert(g >= 0.0);
      assert(g <= 1.0);
      sum+=g;
    }
    const double average_greyness
      = sum / boost::numeric_cast<double>(y2-y1);
    assert(average_greyness >= 0.0);
    assert(average_greyness <= 1.0);
    return average_greyness;
  }
  return GetGreyness(image,x1,y1,x2,y2);
}

double ribi::ImageCanvas::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x,
  const int y) noexcept
{
  assert(!image.empty()
    && "Image is NULL");
  assert(x >= 0
    && "x coordinat is below zero");
  assert(y >= 0
    && "y coordinat is below zero");
  assert(y < boost::numeric_cast<int>(image.size())
    && "y coordinat is beyond image height");
  assert(x < boost::numeric_cast<int>(image[y].size())
    && "x coordinat is beyond image width");
  const double greyness = image[y][x];
  assert(greyness >= 0.0);
  assert(greyness <= 1.0);
  return greyness;
}

double ribi::ImageCanvas::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int x2,
  const int y) noexcept
{
  assert(!image.empty()
    && "Image is NULL");
  assert(x1 >= 0
    && "x1 coordinat is below zero");
  assert(x2 >= 0
    && "x2 coordinat is below zero");
  assert(y >= 0
    && "y coordinat is below zero");
  assert(y < boost::numeric_cast<int>(image.size())
    && "y coordinat is beyond image height");
  assert(x1 < x2
    && "X-coordinats must be different and ordered");
  assert(x1 < boost::numeric_cast<int>(image[y].size())
    && "x1 coordinat is beyond image width");
  assert(x2 < boost::numeric_cast<int>(image[y].size())
    && "x2 coordinat is beyond image width");
  assert(image[y].begin() + x2 != image[y].end()
    && "x2 coordinat iterator must not be beyond image width");
  const double average_greyness = std::accumulate(
    image[y].begin() + x1,
    image[y].begin() + x2,
    0.0) / boost::numeric_cast<double>(x2-x1);
  assert(average_greyness >= 0.0);
  assert(average_greyness <= 1.0);
  return average_greyness;
}

//Get a square of pixels' average greyness
double ribi::ImageCanvas::GetGreyness(
  const std::vector<std::vector<double> >& image,
  const int x1,
  const int y1,
  const int x2,
  const int y2) noexcept
{
  assert(y1 < y2
    && "Y-coordinats must be ordered");

  double sum = 0.0;

  for (int y=y1; y!=y2; ++y)
  {
    const double grey = GetGreyness(image,x1,x2,y);
    assert(grey >= 0 && grey < 1.0);
    sum+=grey;
  }
  const double average_greyness = sum
    / boost::numeric_cast<double>(y2 - y1);

  assert(average_greyness >=0.0 && average_greyness <= 1.0);
  return average_greyness;
}



int ribi::ImageCanvas::GetHeight() const noexcept
{
  return static_cast<int>(m_canvas.size());
}


void ribi::ImageCanvas::SetColorSystem(const CanvasColorSystem colorSystem) noexcept
{
  if (this->m_color_system != colorSystem)
  {
    this->m_color_system = colorSystem;
  }
}

void ribi::ImageCanvas::SetCoordinatSystem(const CanvasCoordinatSystem coordinatSystem) noexcept
{
  if (this->m_coordinat_system != coordinatSystem)
  {
    this->m_coordinat_system = coordinatSystem;
  }
}

std::vector<std::string> ribi::ImageCanvas::ToStrings() const noexcept
{
  std::vector<std::vector<double>> canvas { m_canvas };
  if (m_color_system == CanvasColorSystem::invert)
  {
    for (auto& line: canvas)
    {
      for (double& x: line)
      {
        x = 1.0 - x;
      }
    }
  }
  std::vector<std::string> text {
    ConvertGreynessesToAscii(canvas,m_n_cols)
  };
  if (m_coordinat_system == CanvasCoordinatSystem::graph)
  {
    std::reverse(text.begin(),text.end());
  }
  return text;
}

std::ostream& ribi::operator<<(std::ostream& os, const ImageCanvas& canvas) noexcept
{
  const auto v = canvas.ToStrings();
  std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"));
  return os;
}

bool ribi::operator==(const ImageCanvas& lhs, const ImageCanvas& rhs)
{
  return lhs.ToStrings() == rhs.ToStrings();
}

bool ribi::operator!=(const ImageCanvas& lhs, const ImageCanvas& rhs)
{
  return !(lhs == rhs);
}
