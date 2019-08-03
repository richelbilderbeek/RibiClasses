#ifndef GRABBER_H
#define GRABBER_H





#include <string>
#include <vector>

#include <QObject>


namespace ribi {

struct Grabber final : public QObject
{
  Q_OBJECT

  public:
  Grabber(
    const int win_id,
    const std::string& filename = "screengrab.png"
  );
  ~Grabber() noexcept;

  public slots:
  void Grab() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:
  const std::string m_filename;
  const int m_win_id;
};

} //~namespace ribi

#endif // GRABBER_H
