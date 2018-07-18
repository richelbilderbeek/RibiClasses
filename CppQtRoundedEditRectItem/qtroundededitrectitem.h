#ifndef QTROUNDEDEDITRECTITEM_H
#define QTROUNDEDEDITRECTITEM_H

#include <string>
#include <vector>

#include <QFont>
#include "qtroundedrectitem.h"
#include "qtroundededitrectitempadding.h"

namespace ribi {

///A QtRoundedRectTextItem displaying multiple lines of text
///For a single line of text, use QtRoundedEditRectItem
class QtRoundedEditRectItem : public QtRoundedRectItem
{
  //No need to inherit from Q_OBJECT

  public:
  typedef QtRoundedRectItem Base;
  typedef QtRoundedEditRectItemPadding Padding;

  explicit QtRoundedEditRectItem(
    const std::vector<std::string>& text = { "..." },
    const Padding& padding = Padding(),
    const QFont& font = QFont("monospace",9),
    QGraphicsItem* parent = 0
  );

  virtual ~QtRoundedEditRectItem() noexcept;

  ///Get the font by which the text is drawn
  const QFont& GetFont() const noexcept;

  ///Obtain the padded text rectangle for a single line
  ///where the center lies at the correct location
  /*
          |
          |
      +-------+
      |       |
      | ABCDE |
      |       |
      +-------+
      |       |
   ---|       |---
      |       |
      +-------+
      |       |
      |       |
      |       |
      +-------+
          |
          |

      Line 0/3
  */
  static QRectF GetPaddedTextRectAtLine(const std::string& s, const QFont& font, const int line, const int n_lines) noexcept;

  ///Obtain the padded text rectangle for a single line
  ///where the center lies at the origin
  /*
          |
          |
      +-------+
      |       |
   ---| ABCDE |---
      |       |
      +-------+
          |
          |
  */
  static QRectF GetPaddedTextRectAtOrigin(const std::string& s, const QFont& font) noexcept;

  const Padding& GetPadding() const noexcept { return m_padding; }

  ///Obtain the text on the item
  const std::vector<std::string>& GetText() const noexcept;

  const QPen& GetTextPen() const noexcept { return m_text_pen; }

  ///Obtain the unpadded text rectangle for a single line,
  ///where the center lies at the origin
  /*
          |
          |
       +-----+
    ---|ABCDE|---
       +-----+
          |
          |
  */
  static QRectF GetTextRectAtOrigin(const std::string& s, const QFont& font) noexcept;

  ///Obtain the unpadded text rectangle for the whole text,
  ///where the center lies at the origin
  ///Note: even this rectangle is enlarged by a pixel in both dimensions, so the text will be drawn in full
  /*
          |
          |
       +-----+
       |ABCDE|
    ---|ABCDE|---
       |ABCDE|
       +-----+
          |
          |
  */
  static QRectF GetTextRectAtOrigin(const std::vector<std::string>& text, const QFont& font) noexcept;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Set the font by which the text is drawn
  void SetFont(const QFont& font) noexcept;

  ///Set the padding between text and rectangle
  void SetPadding(const Padding& padding) noexcept;

  ///Set the text displayed
  void SetText(const std::vector<std::string>& text) noexcept;

  ///Set the pen by which the text is drawn
  void SetTextPen(const QPen& pen) noexcept;

protected:
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept;

private:

  ///The font by which the text is drawn
  QFont m_font;

  ///Set the padding between text and rectangle
  Padding m_padding;

  ///The text displayed
  std::vector<std::string> m_text;

  ///Set the padding around text, so the text will be centered
  static const Padding m_text_padding;

  ///The pen by which the text is drawn
  QPen m_text_pen;
};

} //~namespace ribi

#endif // QTROUNDEDEDITRECTITEM_H
