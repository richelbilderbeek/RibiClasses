



#include "qtcanvas.h"

#include <cassert>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QPainter>

#include "canvas.h"
#include "dotmatrixtext.h"


ribi::QtCanvas::QtCanvas()
  : m_signal_on_destroy{},
    m_active_canvas{nullptr},
    m_image{},
    m_image_canvas{},
    m_smart_canvas{},
    m_text_canvas{}
{
}

ribi::QtCanvas::QtCanvas(const boost::shared_ptr<ribi::Canvas> canvas)
  : m_signal_on_destroy{},
    m_active_canvas{nullptr},
    m_image{},
    m_image_canvas{},
    m_smart_canvas{canvas},
    m_text_canvas{}
{
  assert(canvas);
  SetCanvas(canvas);
  assert(m_active_canvas);
}

ribi::QtCanvas::~QtCanvas() noexcept
{
  m_signal_on_destroy();
}

std::string ribi::QtCanvas::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::QtCanvas::GetVersionHistory() noexcept
{
  return {
    "2015-12-02: version 2.0: start versioning"
  };
}

void ribi::QtCanvas::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape)
  {
    m_signal_on_destroy();
    close();
    return;
  }
  QWidget::keyPressEvent(event);
}

void ribi::QtCanvas::keyReleaseEvent(QKeyEvent* event)
{
  QWidget::keyReleaseEvent(event);
}

void ribi::QtCanvas::paintEvent(QPaintEvent *)
{
  if (!m_active_canvas)
  {
    return;
  }
  std::vector<std::string> text;
  for (std::string line: m_active_canvas->ToStrings()) { text.push_back(line); }

  DotMatrixText t(text,1,DotMatrixText::ColorSystem::inverted);
  m_image = t.CreateImage();

  QPainter painter(this);
  painter.drawPixmap(rect(),QPixmap::fromImage(*m_image.get()));
  //painter.drawImage(rect(),*m_image.get());

}

void ribi::QtCanvas::SetCanvas(const boost::shared_ptr<Canvas> canvas)
{
  assert(canvas);

  if (canvas == m_smart_canvas) { return; }

  //Use new Canvas
  m_smart_canvas = canvas;

  //Set minimum size
  {
    std::vector<std::string> text;
    for (std::string line: m_active_canvas->ToStrings()) { text.push_back(line); }
    DotMatrixText t(text,1);
    setMinimumSize(
      t.GetMatrixHeight(),
      t.GetMatrixWidth()
    );
  }

  m_active_canvas = m_smart_canvas.get();
  ShowCanvas(m_active_canvas);
}

void ribi::QtCanvas::SetImageCanvas(const ImageCanvas& canvas)
{
  if (canvas == m_image_canvas) { return; }

  //Use new Canvas
  m_image_canvas = canvas;
  m_active_canvas = &m_image_canvas;

  //Set minimum size
  {
    std::vector<std::string> text;
    for (std::string line: m_active_canvas->ToStrings()) { text.push_back(line); }
    DotMatrixText t(text,1);
    setMinimumSize(
      t.GetMatrixHeight(),
      t.GetMatrixWidth()
    );
  }

  m_active_canvas = m_smart_canvas.get();
  ShowCanvas(m_active_canvas);
}

void ribi::QtCanvas::SetTextCanvas(const TextCanvas& canvas)
{
  if (canvas == m_text_canvas) { return; }

  //Use new Canvas
  m_text_canvas = canvas;
  m_active_canvas = &m_text_canvas;

  //Set minimum size
  {
    std::vector<std::string> text;
    for (std::string line: m_active_canvas->ToStrings()) { text.push_back(line); }
    DotMatrixText t(text,1);
    setMinimumSize(
      t.GetMatrixHeight(),
      t.GetMatrixWidth()
    );
  }

  m_active_canvas = m_smart_canvas.get();
  ShowCanvas(m_active_canvas);
}

void ribi::QtCanvas::ShowCanvas(const Canvas * const)
{
  repaint();
}
