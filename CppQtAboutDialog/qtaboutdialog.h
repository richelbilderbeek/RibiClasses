#ifndef QTABOUTDIALOG_H
#define QTABOUTDIALOG_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "about.h"
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtAboutDialog;
}

namespace ribi {

///An About dialog
class QtAboutDialog : public QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtAboutDialog(const ribi::About& about) noexcept;
  QtAboutDialog(const QtAboutDialog&) = delete;
  QtAboutDialog& operator=(const QtAboutDialog&) = delete;
  ~QtAboutDialog() noexcept;


  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

private:
  Ui::QtAboutDialog *ui;
};

} //~namespace ribi

#endif // QTABOUTDIALOG_H
