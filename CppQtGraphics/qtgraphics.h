#ifndef QTGRAPHICS_H
#define QTGRAPHICS_H

#include <string>
#include <vector>





#include <QImage>


namespace ribi {

struct QtGraphics
{
  QtGraphics();

  ///Create a pretty QImage for testing purposes
  QImage CreateImage(
    const int width,
    const int height,
    const int z = 0) const noexcept;

  ///Draw a QImage on another QImage
  ///Scanline with std::copy
  void DrawImage(
    QImage& target,
    const QImage& source,
    const int left,
    const int top) const noexcept;

  ///Draw a QImage on another QImage
  ///Scanline with indexed copy
  void DrawImageSlow(
    QImage& target,
    const QImage& source,
    const int left,
    const int top) const noexcept;

  ///Draw a QImage on another QImage
  ///Pixel-by-pixel
  void DrawImageSlowest(
    QImage& target,
    const QImage& source,
    const int left,
    const int top) const noexcept;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;
};

///Determines if the image format is correct
bool IsValidFormat(const QImage::Format format) noexcept;

} //~namespace ribi

#endif // QTGRAPHICS_H
