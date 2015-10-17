#include "gossip.h"

#include <iostream>

ribi::Gossip::Gossip()
  : m_operations(1,Operation::default_constructor)
{
  std::cout << "Default constructor\n";
}

ribi::Gossip::Gossip(const Gossip& rhs)
  : m_operations{rhs.m_operations}
{
  std::cout << "Copy constructor\n";
  m_operations.push_back(Operation::copy_constructor);
}

ribi::Gossip::Gossip(Gossip&& rhs)
  : m_operations{rhs.m_operations}
{
  std::cout << "Move constructor\n";
  m_operations.push_back(Operation::move_constructor);
}

ribi::Gossip::~Gossip() noexcept
{
  std::cout << "Destructor\n";
}

ribi::Gossip& ribi::Gossip::operator=(const Gossip& rhs)
{
  std::cout << "Assignment operator\n";
  m_operations = rhs.m_operations;
  m_operations.push_back(Operation::assignment_operator);
  return *this;
}

ribi::Gossip& ribi::Gossip::operator=(Gossip&& rhs)
{
  std::cout << "Move assignment operator\n";
  m_operations = rhs.m_operations;
  m_operations.push_back(Operation::move_assignment_operator);
  return *this;
}
