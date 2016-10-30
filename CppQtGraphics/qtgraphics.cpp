//---------------------------------------------------------------------------
/*
QtGraphics, Qt graphics code snippets
Copyright (C) 2015-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtArrowItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtgraphics.h"

#include <cassert>

//#include "stopwatch.h"
//
//#include "trace.h"
#pragma GCC diagnostic pop


ribi::QtGraphics::QtGraphics()
{

}

QImage ribi::QtGraphics::CreateImage(const int width, const int height, const int z) const noexcept
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
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  #ifdef FIX_MAZIAK_ISSUE_2
  const auto s = source.format();
  TRACE(s);
  assert(source.format() == QImage::Format::Format_RGB32
      || source.format() == QImage::Format::Format_ARGB32
  );
  #endif // FIX_MAZIAK_ISSUE_2
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  #else
  //const auto n_channels = source.pixelFormat().channelCount();
  //assert(source.format() == QImage::Format::Format_RGB32
  //    || source.format() == QImage::Format::Format_ARGB32
  //);
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  #endif
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
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  assert(source.format() == QImage::Format::Format_ARGB32);
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  #else
  //const auto n_channels = source.pixelFormat().channelCount();
  const int n_channels{4};
  assert(n_channels == 3 || n_channels == 4);
  #endif
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

