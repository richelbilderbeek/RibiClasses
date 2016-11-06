#ifndef QTROUNDEDEDITRECTITEMDIALOG_H
#define QTROUNDEDEDITRECTITEMDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
class QtRoundedEditRectItemDialog;
}

namespace ribi {

//struct QtRoundedRectItem; //QtRoundedEditRectItem its base class
struct QtRoundedEditRectItem; //The item
struct QtRoundedRectItemDialog; //its base class display

class QtRoundedEditRectItemDialog : public QtHideAndShowDialog
{
  Q_OBJECT
    
public:

  explicit QtRoundedEditRectItemDialog(QWidget *parent = 0);
  QtRoundedEditRectItemDialog(const QtRoundedEditRectItemDialog&) = delete;
  QtRoundedEditRectItemDialog& operator=(const QtRoundedEditRectItemDialog&) = delete;
  ~QtRoundedEditRectItemDialog() noexcept;

  boost::shared_ptr<QtRoundedEditRectItem> GetItem() const noexcept { return m_item; }

  static int GetMinimumHeight(const QtRoundedEditRectItem& /* item */) noexcept { return 276 + 500; }
  static int GetMinimumHeight(                                       ) noexcept { return 276 + 500; }

  ///Read the X value directly from GUI
  double GetUiX() const noexcept;
  ///Read the Y value directly from GUI
  double GetUiY() const noexcept;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetItem(const boost::shared_ptr<QtRoundedEditRectItem>& item) noexcept;

  ///Set the X value directly to GUI
  void SetUiX(const double x) noexcept;
  ///Set the Y value directly to GUI
  void SetUiY(const double y) noexcept;


protected:
  void keyPressEvent(QKeyEvent * event);

private slots:
  void on_button_font_clicked();
  void on_text_textChanged();
  void on_button_text_pen_clicked();
  void on_box_padding_left_valueChanged(double arg1);
  void on_box_padding_top_valueChanged(double arg1);
  void on_box_padding_right_valueChanged(double arg1);
  void on_box_padding_bottom_valueChanged(double arg1);

private:
  Ui::QtRoundedEditRectItemDialog *ui;

  ///Dialog for its base class
  const boost::shared_ptr<QtRoundedRectItemDialog> m_dialog;

  ///The QtRoundedRectItem to work on
  boost::shared_ptr<QtRoundedEditRectItem> m_item;



  void OnBaseChanged(QtRoundedEditRectItem * const qtitem) noexcept;
  void OnFontChanged(QtRoundedEditRectItem * const qtitem) noexcept;
  void OnPaddingChanged(QtRoundedEditRectItem * const qtitem) noexcept;
  void OnTextChanged(QtRoundedEditRectItem * const qtitem) noexcept;
  void OnTextPenChanged(QtRoundedEditRectItem * const qtitem) noexcept;
};

} //~namespace ribi

#endif // QTROUNDEDEDITRECTITEMDIALOG_H
