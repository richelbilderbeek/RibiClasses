//---------------------------------------------------------------------------
/*
ConnectThree. A connect-three class.
Copyright (C) 2010-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConnectThree.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "connectthreegame.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "connectthreemove.h"
#include "connectthreemovefactory.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::con3::Game::Game(
  const int n_cols,
  const int n_rows
) : m_area(n_cols, std::vector<Square>(n_rows,Square::empty)),
    m_player(Player::player1)
{
  #ifndef NDEBUG
  Test();
  #endif

  Restart();

  assert(n_rows > 1); //Sure, a 2x2 board is useless, but should work
  assert(n_cols > 1); //Sure, a 2x2 board is useless, but should work
  assert(GetCols() == n_cols);
  assert(GetRows() == n_rows);
}


//bool ribi::con3::Game::CanDoAnyMove() const noexcept {
//  const bool are_no_moves_possible{
//    GetAllPossibleMoves().empty()
//  };
//  return !are_no_moves_possible;
//}

bool ribi::con3::Game::CanDoMove(const int x, const int y) const noexcept
{
  return CanGetSquare(x,y)
    && m_area[x][y] == Square::empty;
}

bool ribi::con3::Game::CanDoMove(const Move& p) const noexcept
{
  return CanDoMove(p.GetX(), p.GetY());
}

int ribi::con3::Game::CanGetSquare(const int x, const int y) const noexcept
{
  return
       x >= 0
    && x <  GetCols()
    && y >= 0
    && y <  GetRows()
  ;
}

void ribi::con3::Game::DoMove(const int x, const int y)
{
  assert(CanDoMove(x,y));
  m_area[x][y] = PlayerToSquare(m_player);
  m_player = GetNextPlayer();
}

void ribi::con3::Game::DoMove(const Move& p) noexcept
{
  assert(CanDoMove(p));
  DoMove(p.GetX(),p.GetY());
}

ribi::con3::Square ribi::con3::Game::GetSquare(const int x, const int y) const noexcept
{
  assert(CanGetSquare(x,y));
  assert(!m_area.empty());
  assert(x >= 0);
  assert(x < static_cast<int>(m_area.size()));
  assert(y >= 0);
  #ifndef NDEBUG
  if (y >= static_cast<int>(m_area[x].size()))
  {
    TRACE("ERROR");
    TRACE(y);
    TRACE(m_area.size());
    TRACE(m_area[x].size());
    TRACE("BREAK");
  }
  #endif
  assert(y < static_cast<int>(m_area[x].size()));
  return m_area[x][y];
}

std::string ribi::con3::Game::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::con3::Game::GetVersionHistory() noexcept
{
  return {
    "2010-12-28: version 0.1: initial seperation of game logic from GUI",
    "2011-01-09: version 0.2: converted square values to enum constant, fixed small constructor bug",
    "2011-01-11: version 1.0: added that the game can end in a draw. First tested and debugged version",
    "2011-04-19: version 1.1: added Restart method, removed m_is_player_human",
    "2014-02-13: version 1.2: improved interface",
    "2014-06-30: version 1.3: fixed bug in ribi::con3::ConnectThree::DoMove",
    "2014-07-21: version 1.4: fixed another bug",
    "2015-11-14: version 2.0: simplified classes"
  };
}

ribi::con3::Winner ribi::con3::Game::GetWinner() const noexcept
{
  const int n_rows = GetRows();
  for (int y=0; y!=n_rows; ++y)
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols; ++x)
    {
      if (m_area[x][y] == Square::empty) continue;
      //Horizontal
      if (x + 2 < n_cols
        && m_area[x  ][y] == m_area[x+1][y]
        && m_area[x+1][y] == m_area[x+2][y])
      {
        return SquareToWinner(m_area[x][y]);
      }
      //Vertical
      if (y + 2 < n_rows
        && m_area[x][y  ] == m_area[x][y+1]
        && m_area[x][y+1] == m_area[x][y+2])
      {
        return SquareToWinner(m_area[x][y]);
      }
    }
  }
  //Check for draw: if all
  return IsDraw() ? Winner::draw : Winner::no_winner;
}

ribi::con3::Move ribi::con3::Game::SuggestMove(
  const std::bitset<3>& is_player_human
) const
{
  try {
    const auto m = CheckTwoHorizontalOwn();
    if (CanDoMove(m)) return m;
  }
  catch (std::logic_error&) { /* OK */ }

  try {
    const auto m = CheckTwoVerticalOwn();
    if (CanDoMove(m)) return m;
  }
  catch (std::logic_error&) { /* OK */ }

  try {
    const auto m = CheckTwoOther(is_player_human);
    if (CanDoMove(m)) return m;
  }
  catch (std::logic_error&) { /* OK */ }

  try {
    const auto m = CheckTwoDiagonally();
    if (CanDoMove(m)) return m;
  }
  catch (std::logic_error&) { /* OK */ }

  try {
    const auto m = CheckOneOther(is_player_human);
    if (CanDoMove(m)) return m;
  }
  catch (std::logic_error&) { /* OK */ }

  return MakeRandomMove();
}

ribi::con3::Move ribi::con3::Game::CheckTwoHorizontalOwn() const
{
  const int n_rows = GetRows();
  for (int y=0; y!=n_rows; ++y)
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols-1; ++x) //-1 to prevent out of range
    {
      //Two consequtive selfs
      if (m_area[x][y] == PlayerToSquare(m_player) && m_area[x+1][y] == PlayerToSquare(m_player))
      {
        if (x >= 1)
        {
          if (m_area[x-1][y] == Square::empty)
          {
            const Move p{ MoveFactory().Create(x-1,y,m_player) };
            assert(CanDoMove(p));
            return p;
          }
        }
        if (x < n_cols-2 && m_area[x+2][y] == Square::empty)
        {
          const Move p { MoveFactory().Create(x+2,y,m_player) };
          assert(CanDoMove(p));
          return p;
        }
      }
      //Perhaps a gap?
      if (x < n_cols-2)
      {
        if (m_area[x][y] == PlayerToSquare(m_player) && m_area[x+1][y] == Square::empty && m_area[x+2][y] == PlayerToSquare(m_player))
        {
          const Move p { MoveFactory().Create(x+1,y,m_player) };
          assert(CanDoMove(p));
          return p;
        }
      }
    }
  }
  throw std::logic_error("no move");
}

ribi::con3::Move ribi::con3::Game::CheckTwoVerticalOwn() const
{
  const int n_rows = GetRows();
  assert(n_rows > 1);
  for (int y=0; y!=n_rows-1; ++y) //-1 to prevent out of range
  {
    const int n_cols = GetCols();
    assert(n_cols > 1);
    for (int x=0; x!=n_cols; ++x)
    {
      //Two consequtive selfs?
      if (m_area[x][y] == PlayerToSquare(m_player) && m_area[x][y+1] == PlayerToSquare(m_player))
      {
        if (y >= 1)
        {
          if (m_area[x][y-1] == Square::empty)
          {
            const Move p { MoveFactory().Create(x,y-1,m_player) };
            assert(CanDoMove(p));
            return p;
          }
        }
        if (y < n_rows-2)
        {
          if (m_area[x][y+2] == Square::empty)
          {
            const Move p { MoveFactory().Create(x,y+2,m_player) };
            assert(CanDoMove(p));
            return p;
          }
        }
      }
      //Perhaps a gap?
      if (y < n_rows-2)
      {
        if (m_area[x][y] == PlayerToSquare(m_player) && m_area[x][y+1] == Square::empty && m_area[x][y+2] == PlayerToSquare(m_player))
        {
          const Move p { MoveFactory().Create(x,y+1,m_player) };
          assert(CanDoMove(p));
          return p;
        }
      }
    }
  }
  throw std::logic_error("no move");
}

ribi::con3::Move ribi::con3::Game::CheckTwoOther(const std::bitset<3>& is_player_human) const
{
  const auto moves(GetAllPossibleMoves());

  const int nMoves = moves.size();
  if (nMoves==0)
  {
    throw std::logic_error("no move");
  }

  {
    //Get anti-human moves
    std::vector<Move> v;
    for(const auto m: moves)
    {
      assert(CanDoMove(m));
      //Player is human
      if (IsPlayerHuman(m.GetPlayer(),is_player_human))
      {
        v.push_back(m);
      }
    }
    //If there are anti-player moves, choose one at random
    if (!v.empty())
    {
      const auto move = v[std::rand() % v.size()];
      assert(CanDoMove(move));
      return move;
    }
  }
  {
    //Get moves anti-next-player
    const Player next_player_index = GetNextPlayer();
    std::vector<Move> v;
    for(const auto& move: moves)
    {
      assert(CanDoMove(move));
      if (move.GetPlayer() == next_player_index)
        v.push_back(move);
    }
    //If there are anti-next-player moves, choose one at random
    if (!v.empty())
    {
      const auto move = v[std::rand() % v.size()];
      assert(CanDoMove(move));
      return move;
    }
  }
  //Choose a move at random
  {
    const auto move = moves[std::rand() % moves.size()];
    assert(CanDoMove(move));
    return move;
  }
}

std::vector<ribi::con3::Move>
  ribi::con3::Game::GetAllPossibleMoves() const noexcept
{
  std::vector<Move> v(GetTwoHorizontalOtherMoves());
  const std::vector<Move> w(GetTwoVerticalOtherMoves());
  std::copy(w.begin(),w.end(),std::back_inserter(v));
  return v;
  /*
  std::vector<Move> m;
  const int n_cols = GetCols();
  const int n_rows = GetRows();
  for (int x=0; x!=n_cols; ++x)
  {
    for (int y=0; y!=n_rows; ++y)
    {
      if (CanDoMove(x,y)) {
        m.push_back(Move(x,y))
      }
    }
  }
  return m;
  */
}

std::vector<ribi::con3::Move> ribi::con3::Game::GetTwoHorizontalOtherMoves() const noexcept
{
  const int n_rows = GetRows();
  std::vector<Move> moves;
  for (int y=0; y!=n_rows; ++y)
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols-1; ++x) //-1 to prevent out of range
    {
      //Check consequtive
      if (m_area[x][y]!=Square::empty && m_area[x][y] == m_area[x+1][y])
      {
        //Check A X B
        if (x > 0 && m_area[x-1][y] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x-1,y,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          moves.push_back(m);
        }
        //Check X B C
        if (x < n_cols-2 && m_area[x+2][y] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x+2,y,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          moves.push_back(m);
        }
      }
      //Check gap, also X B C
      if (m_area[x][y] != Square::empty && x + 2 < n_cols && m_area[x+1][y] == Square::empty && m_area[x][y] == m_area[x+2][y])
      {
        const Move m { MoveFactory().Create(
          x+1,y,SquareToPlayer(m_area[x][y]))
        };
        assert(CanDoMove(m));
        moves.push_back(m);
      }

    }
  }
  return moves;
}

//A X B C (x is focus of for loop)
std::vector<ribi::con3::Move>
  ribi::con3::Game::GetTwoVerticalOtherMoves() const noexcept
{
  const int n_rows = GetRows();
  std::vector<Move> v;

  for (int y=0; y!=n_rows-1; ++y) //-1 to prevent out of range
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols; ++x)
    {
      //Check consequtive
      if (m_area[x][y] != Square::empty && m_area[x][y] == m_area[x][y+1])
      {
        //Check A X B
        if (y > 0 && m_area[x][y-1] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x,y-1,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          v.push_back(m);
        }
        //Check X B C
        if (y < n_rows-2 && m_area[x][y+2] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x,y+2,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          v.push_back(m);
        }
      }
      //Check gap, also X B C
      if (m_area[x][y] != Square::empty && y < n_rows && m_area[x][y+1] == Square::empty && m_area[x][y] == m_area[x][y+2])
      {
        const Move m { MoveFactory().Create(
          x,y+1,SquareToPlayer(m_area[x][y]))
        };
        assert(CanDoMove(m));
        v.push_back(m);
      }
    }
  }
  return v;
}

ribi::con3::Move ribi::con3::Game::CheckTwoDiagonally() const
{
  std::vector<Move> v;

  const int n_rows = GetRows();
  for (int y=0; y!=n_rows-1; ++y) //-1 To prevent out of range
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols-1; ++x) //-1 to prevent out of range
    {
      if (m_area[x][y] == PlayerToSquare(m_player) && m_area[x+1][y+1] == PlayerToSquare(m_player))
      {
        if (m_area[x+1][y] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x+1,y,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          v.push_back(m);
        }
        if (m_area[x][y+1] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x,y+1,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          v.push_back(m);
        }
      }
    }
  }
  if (v.empty())
  {
    throw std::logic_error("no move");
  }
  const auto m = v[std::rand() % v.size()];
  assert(CanDoMove(m));
  return m;
}

ribi::con3::Move ribi::con3::Game::CheckOneOther(const std::bitset<3>& is_player_human) const
{
  std::vector<Move> v;

  const int n_rows = GetRows();

  for (int y=0; y!=n_rows; ++y)
  {
    const int n_cols = GetCols();
    for (int x=0; x!=n_cols; ++x)
    {
      if (m_area[x][y] != Square::empty)
      {
        if (y >= 1 && m_area[x][y-1] == Square::empty)
        {
          const Move m { MoveFactory().Create(
            x,y-1,SquareToPlayer(m_area[x][y]))
          };
          assert(CanDoMove(m));
          v.push_back(m);
        }
        if (y < n_rows-1)
        {
          if (m_area[x][y+1] == Square::empty)
          {
            const Move m { MoveFactory().Create(
              x,y+1,SquareToPlayer(m_area[x][y]))
            };
            assert(CanDoMove(m));
            v.push_back(m);
          }
        }
        if (x >= 1)
        {
          if (m_area[x-1][y] == Square::empty)
          {
            const Move m { MoveFactory().Create(
              x-1,y,SquareToPlayer(m_area[x][y]))
            };
            assert(CanDoMove(m));
            v.push_back(m);
          }
        }
        if (x < n_cols-1)
        {
          if (m_area[x+1][y] == Square::empty)
          {
            const Move m { MoveFactory().Create(
              x+1,y,SquareToPlayer(m_area[x][y]))
            };
            assert(CanDoMove(m));
            v.push_back(m);
          }
        }
      }
    }
  }
  if (v.empty())
  {
    throw std::logic_error("no move");
  }

  {
    //Get anti-human moves
    std::vector<Move> w;
    for(const auto m: v)
    {
      assert(CanDoMove(m));
      if (IsPlayerHuman(m.GetPlayer(),is_player_human))
        w.push_back(m);
    }
    //If there are anti-player moves, choose one at random
    if (!w.empty())
    {
      const auto m = w[std::rand() % w.size()]; //ex-bug ('w.size()' was 'v.size()')
      assert(CanDoMove(m));
      return m;
    }
  }

  {
    //Get moves anti-next-player
    const Player next_player_index = GetNextPlayer();
    std::vector<Move> w;
    for(const auto m: v)
    {
      assert(CanDoMove(m));
      if (m.GetPlayer() == next_player_index)
        w.push_back(m);
    }
    //If there are anti-next-player moves, choose one at random
    if (!w.empty())
    {
      const auto m = w[std::rand() % w.size()];
      assert(CanDoMove(m));
      return m;
    }
  }
  //Choose a move at random
  {
    assert(v.size() != 0);
    const auto m = v[std::rand() % v.size()];
    assert(CanDoMove(m));
    return m;
  }
}

ribi::con3::Move ribi::con3::Game::MakeRandomMove() const
{
  std::vector<Move> v;
  const int n_cols = GetCols();
  const int n_rows = GetRows();

  for (int y=0; y!=n_rows; ++y)
  {
    for (int x=0; x!=n_cols; ++x)
    {
      if (this->GetSquare(x,y) == Square::empty)
      {
        const Move m { MoveFactory().Create(
          x,y,m_player)
        };
        assert(CanDoMove(m));
        v.push_back(m);
      }
    }
  }
  if (v.empty())
  {
    throw std::logic_error("no move");
  }
  const int index = std::rand() % v.size();
  return v[index];
}

int ribi::con3::Game::GetCols() const noexcept
{
  assert(!m_area.empty());
  return m_area.size();
}

ribi::con3::Player ribi::con3::Game::GetNextPlayer() const noexcept
{
  return ribi::con3::GetNextPlayer(m_player);
}

int ribi::con3::Game::GetRows() const noexcept
{
  assert(!m_area.empty());
  return m_area[0].size();
}

bool ribi::con3::Game::IsDraw() const noexcept
{
  const int n_cols = GetCols();
  const int n_rows = GetRows();
  for (int y=0; y!=n_rows; ++y)
  {
    for (int x=0; x!=n_cols; ++x)
    {
      assert(CanGetSquare(x,y));
      if (GetSquare(x,y) == Square::empty) { return false; }
    }
  }
  return true;
}

bool ribi::con3::Game::IsPlayerHuman(const Player player, const std::bitset<3>& is_player_human) const noexcept
{
  switch (player)
  {
    case Player::player1: return is_player_human[0];
    case Player::player2: return is_player_human[1];
    case Player::player3: return is_player_human[2];
    default:
      assert(!"Should not get here");
      throw std::logic_error("Unknown value of player");
  }
}

ribi::con3::Square ribi::con3::Game::PlayerToSquare(const Player player) const noexcept
{
  switch (player)
  {
    case Player::player1: return Square::player1;
    case Player::player2: return Square::player2;
    case Player::player3: return Square::player3;
    default:
      assert(!"Should not get here");
      throw std::logic_error("Unknown player");
  }
}


void ribi::con3::Game::Restart() noexcept
{
  m_area = std::vector<std::vector<Square> >(GetCols(),
    std::vector<Square>(GetRows(),Square::empty));
  m_player = Player::player1;
}

ribi::con3::Player ribi::con3::Game::SquareToPlayer(const Square square) const noexcept
{
  switch (square)
  {
    case Square::player1: return Player::player1;
    case Square::player2: return Player::player2;
    case Square::player3: return Player::player3;
    default:
      assert(!"Should not get here");
      throw std::logic_error("Unknown Square");
  }
}

ribi::con3::Winner ribi::con3::Game::SquareToWinner(const Square square) const noexcept
{
  switch (square)
  {
    case Square::player1: return Winner::player1;
    case Square::player2: return Winner::player2;
    case Square::player3: return Winner::player3;
    default:
      assert(!"Should not get here");
      throw std::logic_error("Unknown Square");
  }
}

#ifndef NDEBUG
void ribi::con3::Game::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  //Can a 2x2 game be played without exceptions thrown
  const int n_rows = 2;
  const int n_cols = 2;
  std::bitset<3> is_player_human;
  is_player_human[0] = true;
  is_player_human[1] = true;
  is_player_human[2] = true;
  Game c(n_cols,n_rows);
  assert(c.GetCols() == n_cols);
  assert(c.GetRows() == n_rows);
  assert(c.GetActivePlayer() == Player::player1);
  assert( c.CanDoMove(0,0));
  assert( c.CanDoMove(0,1));
  assert( c.CanDoMove(1,0));
  assert( c.CanDoMove(1,1));
  assert(!c.CanDoMove(0,n_rows));
  assert(!c.CanDoMove(n_rows,0));
  assert(c.SuggestMove(is_player_human).GetX() >= 0); //It just shouldn't throw
  assert(c.GetWinner() == Winner::no_winner); //No winner yet
  c.DoMove(0,0);
  assert(c.GetActivePlayer() == Player::player2);
  assert(!c.CanDoMove(0,0));
  assert( c.CanDoMove(0,1));
  assert( c.CanDoMove(1,0));
  assert( c.CanDoMove(1,1));
  assert(c.SuggestMove(is_player_human).GetX() >= 0);//It just shouldn't throw
  assert(c.GetWinner() == Winner::no_winner); //No winner yet
  c.DoMove(0,1);
  assert(c.GetActivePlayer() == Player::player3);
  assert(!c.CanDoMove(0,0));
  assert(!c.CanDoMove(0,1));
  assert( c.CanDoMove(1,0));
  assert( c.CanDoMove(1,1));
  assert(c.SuggestMove(is_player_human).GetX() >= 0);//It just shouldn't throw
  assert(c.GetWinner() == Winner::no_winner); //No winner yet
  c.DoMove(1,0);
  assert(c.GetActivePlayer() == Player::player1);
  assert(!c.CanDoMove(0,0));
  assert(!c.CanDoMove(0,1));
  assert(!c.CanDoMove(1,0));
  assert( c.CanDoMove(1,1));
  assert(c.SuggestMove(is_player_human).GetX() >= 0);//It just shouldn't throw
  assert(c.GetWinner() == Winner::no_winner); //No winner yet
  c.DoMove(1,1);
  assert(c.GetActivePlayer() == Player::player2);
  assert(!c.CanDoMove(0,0));
  assert(!c.CanDoMove(0,1));
  assert(!c.CanDoMove(1,0));
  assert(!c.CanDoMove(1,1));
  try {
    c.SuggestMove(is_player_human);
  }
  catch (std::logic_error& ) {
    //OK
  }
  assert(c.GetWinner() == Winner::draw);
}
#endif

std::ostream& ribi::con3::operator<<(std::ostream& os, const ribi::con3::Game& c)
{
  const int n_cols = c.GetCols();
  const int n_rows = c.GetRows();
  for (int y=0; y!=n_rows; ++y)
  {
    for (int x=0; x!=n_cols; ++x)
    {
      char d = ' ';
      switch (c.GetSquare(x,y))
      {
        case Square::empty  : d = '.'; break;
        case Square::player1: d = '1'; break;
        case Square::player2: d = '2'; break;
        case Square::player3: d = '3'; break;
        default: assert(!"Should not get here");
      }
      os << d;
    }
    os << '\n';
  }
  return os;
}
