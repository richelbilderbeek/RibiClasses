#ifndef BEASTSTATEFILE_H
#define BEASTSTATEFILE_H

#include <string>

namespace ribi {

///BEAST2 state file, the results of a BEAST2 run, has a .xml.state file extension
struct BeastStateFile
{
  BeastStateFile(const std::string& filename);

  std::string GetTree() const noexcept { return m_tree; }

  private:

  const std::string m_tree;

  ///Find the tree in a file
  std::string FindTree(const std::string& filename) const;

  ///Find the line the tree is to be from extracted
  std::string FindTreeLine(const std::string& filename) const;

};

} //~namespace ribi

#endif // BEASTSTATEFILE_H
