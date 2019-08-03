#ifndef QTALPHAFILTERDIALOG_H
#define QTALPHAFILTERDIALOG_H



#include <boost/shared_ptr.hpp>
#include <QDialog>
#include "stateobserverfwd.h"


namespace Ui {
  class QtAlphaFilterDialog;
}

namespace ribi {

///Dialog to create an alpha filter
class QtAlphaFilterDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit QtAlphaFilterDialog(QWidget *parent = 0) noexcept;
  QtAlphaFilterDialog(const QtAlphaFilterDialog&) = delete;
  QtAlphaFilterDialog& operator=(const QtAlphaFilterDialog&) = delete;
  ~QtAlphaFilterDialog() noexcept;

  ///Create an alpha filter with the current parameters
  boost::shared_ptr<AlphaFilter> Create() const noexcept;
  
private:
  Ui::QtAlphaFilterDialog *ui;
};

} //~namespace ribi

#endif // QTALPHAFILTERDIALOG_H
