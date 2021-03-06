#ifndef RIBI_QTCANVAS_H
#define RIBI_QTCANVAS_H





#include <boost/scoped_ptr.hpp>
#include <boost/signals2.hpp>
#include "imagecanvas.h"
#include "textcanvas.h"
#include <QPlainTextEdit>
#include "canvas.h"


namespace ribi {

struct Canvas;
struct ImageCanvas;
struct TextCanvas;

///A Canvas class that can be used on a QDialog
///If the Canvas is modified, this is displayed in the QtCanvas
class QtCanvas : public QWidget
{
  Q_OBJECT

public:

  QtCanvas();
  QtCanvas(const boost::shared_ptr<Canvas> canvas);
  QtCanvas(const QtCanvas&) = delete;
  QtCanvas& operator=(const QtCanvas&) = delete;
  virtual ~QtCanvas() noexcept;

  const Canvas * GetCanvas() const noexcept { return m_active_canvas; }
        Canvas * GetCanvas()       noexcept { return m_active_canvas; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetCanvas(const boost::shared_ptr<Canvas> canvas);
  void SetImageCanvas(const ImageCanvas& canvas);
  void SetTextCanvas(const TextCanvas& canvas);

  ///Emitted when the QtCanvas is destroyed
  boost::signals2::signal<void()> m_signal_on_destroy;

  virtual void keyPressEvent(QKeyEvent * e) override;
  virtual void keyReleaseEvent(QKeyEvent * e) override;
  virtual void paintEvent(QPaintEvent *) override;

private:
  friend void boost::checked_delete<>(QtCanvas* x);
  friend void boost::checked_delete<>(const QtCanvas* x);

  Canvas * m_active_canvas;
  boost::shared_ptr<QImage> m_image; //Used as buffer
  ImageCanvas m_image_canvas;
  boost::shared_ptr<Canvas> m_smart_canvas;
  TextCanvas m_text_canvas;

  void ShowCanvas(const Canvas * const);
};

} //~namespace ribi

#endif // RIBI_QTCANVAS_H
