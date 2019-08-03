


#include "qtgraphics.h"

#include <cassert>



ribi::QtGraphics::QtGraphics()
{

}

QImage ribi::QtGraphics::CreateImage(
  const int width, const int height, const int z) const noexcept
{
  QImage image(width,height,QImage::Format_ARGB32);
  for (int y=0;y!=height;++y)
  {
    for (int x=0;x!=width;++x)
    {
      image.setPixel(
        x,y,
        qRgb((x+z+0)%256,(y+z+0)%256,(x+y+z)%256) //Color
      );
    }
  }
  return image;
}

void ribi::QtGraphics::DrawImage(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  assert(IsValidFormat(source.format()));
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    const auto line_to = target.scanLine(y + top);
    const auto line_from = source.constScanLine(y);
    std::copy(
      &line_from[0],
      &line_from[width * n_channels],
      &line_to[left * n_channels]
    );
  }
}

void ribi::QtGraphics::DrawImageSlow(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  assert(source.format() == QImage::Format::Format_ARGB32);
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    const auto line_to = target.scanLine(y + top);
    const auto line_from = source.constScanLine(y);
    for (int x=0; x!=width; ++x)
    {
      for (int c=0; c!=n_channels; ++c)
      {
        line_to[ ((left + x) * n_channels) + c]
          = line_from[ (x * n_channels) + c]
        ;
      }
    }
  }
}

void ribi::QtGraphics::DrawImageSlowest(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      target.setPixel(left+x,top+y,source.pixel(x,y));
    }
  }
}

std::string ribi::QtGraphics::GetVersion() const noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QtGraphics::GetVersionHistory() const noexcept
{
  return {
    "2015-02-22: version 1.0: initial version",
    "2015-08-28: version 1.1: added CreateImage and DrawImage",
    "2015-09-05: version 1.2: improved speed of DrawImage by an order of magnitude",
    "2016-03-27: version 1.3: use of Boost.Test",
  };
}

bool ribi::IsValidFormat(const QImage::Format f) noexcept
{
  return f == QImage::Format::Format_RGB32
      || f == QImage::Format::Format_ARGB32;
}
