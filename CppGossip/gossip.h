#ifndef GOSSIP_H
#define GOSSIP_H

#include <vector>

namespace ribi {

enum class Operation {
  default_constructor,
  copy_constructor,
  move_constructor,
  assignment_operator,
  move_assignment_operator
};


struct Gossip
{
  Gossip();
  Gossip(const Gossip& rhs);
  Gossip(Gossip&& rhs);
  ~Gossip() noexcept;
  Gossip& operator=(const Gossip& rhs);
  Gossip& operator=(Gossip&& rhs);

  const std::vector<Operation>& GetOperations() const noexcept { return m_operations; }

  private:
  std::vector<Operation> m_operations;
};

} //~namespace ribi

#endif // GOSSIP_H
