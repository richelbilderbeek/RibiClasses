



#include "qtroundededitrectitem.h"

#include <cassert>
#include <algorithm>
#include <numeric>
#include <boost/lambda/lambda.hpp>

#include <QFontMetrics>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>

#include "container.h"



const ribi::QtRoundedEditRectItem::Padding ribi::QtRoundedEditRectItem::m_text_padding(3.0,0.0,0.0,3.0);

ribi::QtRoundedEditRectItem::QtRoundedEditRectItem(
  const std::vector<std::string>& text,
  const Padding& padding,
  const QFont& font,
  QGraphicsItem* parent)
  : QtRoundedRectItem(parent),
    m_font(font),
    m_padding(padding),
    m_text( {""} ), //Empty std::vector<std::string>, as m_text must be set by SetText
    m_text_pen{}
{
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  );

  ///Obtain a white background
  this->setBrush(QBrush(QColor(255,255,255)));

  this->SetFont(font);
  this->SetRadiusX(4.0);
  this->SetRadiusY(4.0);
  this->SetPadding(Padding(1.0,1.0,1.0,1.0));
  this->SetText(text);
  this->update();
}

ribi::QtRoundedEditRectItem::~QtRoundedEditRectItem() noexcept
{
  //OK
}

const QFont& ribi::QtRoundedEditRectItem::GetFont() const noexcept
{
  return m_font;
}

QRectF ribi::QtRoundedEditRectItem::GetPaddedTextRectAtOrigin(
  const std::string& s,
  const QFont& font
) noexcept
{
  return GetTextRectAtOrigin(s,font).adjusted(
    -m_text_padding.left,
    -m_text_padding.top,
     m_text_padding.right,
     m_text_padding.bottom);
}

QRectF ribi::QtRoundedEditRectItem::GetPaddedTextRectAtLine(
  const std::string& s,
  const QFont& font,
  const int line,
  const int n_lines
) noexcept
{
  const auto r = GetPaddedTextRectAtOrigin(s,font);
  const auto line_width = r.width();
  const auto line_height = r.height();
  const auto total_width = line_width;
  const auto total_height = line_height * n_lines;
  const auto total_x1 = -0.5 * total_width;
  const auto total_y1 = -0.5 * total_height;
  const auto x1 = total_x1;
  const auto y1 = total_y1 + (line * line_height);
  return QRectF(x1,y1,line_width,line_height);

}

const std::vector<std::string>& ribi::QtRoundedEditRectItem::GetText() const noexcept
{
  return m_text;
}

QRectF ribi::QtRoundedEditRectItem::GetTextRectAtOrigin(
  const std::string& s,
  const QFont& font
) noexcept
{
  const double h = QFontMetricsF(font).height();
  const double w = QFontMetricsF(font).width(s.c_str());
  assert(h > 0.0);
  assert(w >= 0.0 && "An empty text can have width 0");
  #ifdef _WIN32
  //adjusted(0.0,0.0,2.0,0.0) works fine for 50% of the fonts supplied by Wine under native Lubuntu
  //adjusted(0.0,0.0,3.0,0.0) works fine for 80% of the fonts supplied by Wine under native Lubuntu
  return QRectF(-0.5 * w, 0.0,w,h).adjusted(0.0,0.0,3.0,0.0);
  #else
  //adjusted(0.0,0.0,2.0,-1.0) works fine for 90% of the fonts under native Lubuntu
  //adjusted(0.0,0.0,3.0,-1.0) works fine for 99% of the fonts under native Lubuntu
  //adjusted(0.0,0.0,4.0,-1.0) works fine for all the fonts I've tried under native Lubuntu
  //return QRectF(-0.5 * w,0.0,w,h).adjusted(0.0,0.0,2.0,-1.0);
  const QRectF result = QRectF(-0.5 * w,-0.5 * h,w,h).adjusted(0.0,0.0,2.0,0.0);
  assert(result.width() >= 0.0);
  assert(result.height() > 0.0);
  return result;
  #endif


}

QRectF ribi::QtRoundedEditRectItem::GetTextRectAtOrigin(
  const std::vector<std::string>& text,
  const QFont& font) noexcept
{
  std::vector<QRectF> v;
  std::transform(text.begin(),text.end(),std::back_inserter(v),
    [font](const std::string& s)
    {
      return QtRoundedEditRectItem::GetPaddedTextRectAtOrigin(s,font);
    }
  );
  const auto width_iter = std::max_element(v.begin(),v.end(),
    [](const QRectF& lhs, const QRectF& rhs)
    {
      return lhs.width() < rhs.width();
    }
  );
  const double width = width_iter == v.end() ? 1.0 : width_iter->width();
  const double height = std::accumulate(v.begin(),v.end(),0.0,
    [](double& init, const QRectF& r)
    {
      assert(r.height() > 0.0);
      return init + r.height() + 0.0;
    }
  );

  assert(width  > 0.0);
  assert(height > 0.0);
  return QRectF(-0.5 * width,-0.5 * height, width, height).adjusted(-0.0,-0.0,0.0,0.0);
}

std::string ribi::QtRoundedEditRectItem::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::QtRoundedEditRectItem::GetVersionHistory() noexcept
{
  return {
    "2012-12-19: version 1.0: initial version",
    "2014-08-09: version 1.1: increased use of TDD"
  };
}

void ribi::QtRoundedEditRectItem::paint(
  QPainter* painter,
  const QStyleOptionGraphicsItem* option,
  QWidget* widget
)
{
  //Draws the rounded rectangle
  QtRoundedRectItem::paint(painter,option,widget);

  //Draw the text lines at the right spot
  painter->setFont(m_font);
  painter->setPen(m_text_pen);
  const int sz = static_cast<int>(m_text.size());
  for (int i=0; i!=sz;++i)
  {
    // For this line, work down from
    // (1) a padded text rectangle at the right location
    // (2) an (ordinary) text rectangle (where the text will be drawn), at the right location
    const std::string& s = m_text[i];

    // (1) a padded text rectangle at the right location
    const QRectF padded_rect = GetPaddedTextRectAtLine(s,m_font,i,sz);

    // (2) an (ordinary) text rectangle (where the text will be drawn), at the right location
    const QRectF text_rect(
      padded_rect.adjusted(
         m_text_padding.left,
         m_text_padding.top,
        -m_text_padding.right,
        -m_text_padding.bottom
      )
    );

    painter->drawText(text_rect,s.c_str());
  }
}



void ribi::QtRoundedEditRectItem::SetFont(const QFont& font) noexcept
{
  if (m_font != font)
  {
    m_font = font;
    this->update();
  }
}


void ribi::QtRoundedEditRectItem::SetPadding(const Padding& padding) noexcept
{
  if (padding != m_padding)
  {
    m_padding = padding;

    //Adapt the size
    const QRectF text_rect = GetTextRectAtOrigin(m_text,m_font);
    this->SetInnerWidth(
      text_rect.width() + m_padding.left + m_padding.right
    );
    this->SetInnerHeight(
      text_rect.height() + m_padding.top + m_padding.bottom
    );

    this->update();
  }
}

void ribi::QtRoundedEditRectItem::SetText(const std::vector<std::string>& text) noexcept
{
  if (text != m_text)
  {
    m_text = text;

    //Adapt the size
    const QRectF text_rect = GetTextRectAtOrigin(m_text,m_font);
    this->SetInnerWidth(
      text_rect.width() + m_padding.left + m_padding.right
    );
    this->SetInnerHeight(
      text_rect.height() + m_padding.top + m_padding.bottom
    );
    this->update();
  }
}

void ribi::QtRoundedEditRectItem::SetTextPen(const QPen& pen) noexcept
{
  if (m_text_pen != pen)
  {
    m_text_pen = pen;
    this->update();
  }
}
