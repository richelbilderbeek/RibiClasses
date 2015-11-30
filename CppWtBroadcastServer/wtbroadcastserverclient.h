#ifndef WTBROADCASTSERVERCLIENT_H
#define WTBROADCASTSERVERCLIENT_H

#include <string>
#include <vector>

///WtBroadcastServerClient is a client responding to WtBroadcastServer
///and to be used as a base class
struct WtBroadcastServerClient
{
  virtual ~WtBroadcastServerClient();

  ///Get the version of this class
  static std::string GetVersion() noexcept;

  ///Get the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///UpdatePage is called when the WtBroadcastServer triggers an update by timer
  virtual void UpdatePage() = 0;

  protected:
  ///WtBroadcastServerClient constructor is protected
  ///because it is to be used as a base class
  WtBroadcastServerClient();

  private:
  ///Respond to the server
  void OnServer();
};

#endif // WTBROADCASTSERVERCLIENT_H
