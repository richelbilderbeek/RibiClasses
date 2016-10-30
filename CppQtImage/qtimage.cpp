#include "qtimage.h"

#include <cassert>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>


// #include "trace.h"
#pragma GCC diagnostic pop

ribi::QtImage::QtImage() noexcept
{

}

QImage ribi::QtImage::Difference(const QImage& base, const QImage& to_xor) noexcept
{
  QImage result{base};
  {
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Difference);
    painter.drawImage(0,0,to_xor);
  }
  return result;
}

std::string ribi::QtImage::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::QtImage::GetVersionHistory() noexcept
{
  return {
    "2014-08-05: Version 1.0: initial version",
  };
}

QImage ribi::QtImage::Paint(const QGraphicsItem& item) noexcept
{
  if (item.scene())
  {
    auto& scene = *item.scene();
    // Create the image with the exact size of the shrunk scene
    const QSize old_size{scene.sceneRect().size().toSize()};
    //Rescaled by a factor two to fix BUG_260
    //const QSize new_size(old_size.scaled(2,2, Qt::KeepAspectRatio));
    QImage image(old_size, QImage::Format_ARGB32);
    // Start all pixels transparent
    image.fill(Qt::transparent);
    QPainter painter(&image);
    scene.render(&painter);
    return image;
  }
  else
  {
    QGraphicsScene scene;
    assert(!item.scene());
    scene.addItem(&const_cast<QGraphicsItem&>(item)); //Temporarily add the item, won't modify it
    assert(item.scene());

    const QSize old_size{scene.sceneRect().size().toSize()};
    //Rescaled by a factor two to fix BUG_260
    //const QSize new_size(old_size.scaled(2,2, Qt::KeepAspectRatio));
    QImage image(old_size, QImage::Format_ARGB32);
    // Start all pixels transparent
    image.fill(Qt::transparent);
    QPainter painter(&image);
    scene.render(&painter);
    assert(item.scene());
    scene.removeItem(&const_cast<QGraphicsItem&>(item)); //Prevent item being deleted
    assert(!item.scene());
    return image;
  }
}

QImage ribi::QtImage::Xor(const QImage& base, const QImage& to_xor) noexcept
{
  QImage result{base};
  {
    QPainter painter(&result);
    //painter.setCompositionMode(QPainter::CompositionMode_Xor);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.drawImage(0,0,to_xor);
  }
  return result;
}

