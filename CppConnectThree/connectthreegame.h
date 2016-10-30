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
#ifndef CONNECTTHREE_H
#define CONNECTTHREE_H

#include <bitset>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/tuple/tuple.hpp>
#include "connectthreefwd.h"
#include "connectthreeplayer.h"
#include "connectthreesquare.h"
#include "connectthreemove.h"
#include "connectthreewinner.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace con3 {

///ConnectThree does not know which players are human.
///ConnectThree can suggest a move, but for this it needs to know if the
///others are human or computer
struct Game
{
  explicit Game(
    const int n_cols = 16,
    const int n_rows = 12
  );

  ///Is there at least one valid move?
//  bool CanDoAnyMove() const noexcept;

  bool CanDoMove(const int x, const int y) const noexcept;
  int CanGetSquare(const int x, const int y) const noexcept;
  void DoMove(const int x, const int y);
  void DoMove(const Move& p) noexcept;
  Player GetActivePlayer() const noexcept { return m_player; }
  int GetCols() const noexcept;
  int GetRows() const noexcept;
  Square GetSquare(const int x, const int y) const noexcept;
  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///GetWinner returns the index of the winner.
  ///Returns draw if the board is full without a winner
  ///Returns no_player if there are still free squares
  Winner GetWinner() const noexcept;

  void Restart() noexcept;

  ///SuggestMove suggests a good move.
  ///If the game is a draw, it will throw a logic error
  Move SuggestMove(const std::bitset<3>& is_player_human) const;

private:

  //X-Y-ordered 2D std::vector
  std::vector<std::vector<Square>> m_area;
  Player m_player;

  bool CanDoMove(const Move& p) const noexcept;
  Move CheckOneOther(const std::bitset<3>& is_player_human) const;
  Move CheckTwoDiagonally() const;
  Move CheckTwoHorizontalOwn() const;
  Move CheckTwoOther(const std::bitset<3>& is_player_human) const;
  Move CheckTwoVerticalOwn() const;

  ///Returns all possible moves
  std::vector<Move> GetAllPossibleMoves() const noexcept;
  Player GetNextPlayer() const noexcept;
  std::vector<Move> GetTwoHorizontalOtherMoves() const noexcept;
  std::vector<Move> GetTwoVerticalOtherMoves() const noexcept;

  ///Are all the squares occupied?
  bool IsDraw() const noexcept;
  bool IsPlayerHuman(const Player player, const std::bitset<3>& is_player_human) const noexcept;

  ///Throws std::logic_error if there is no move left
  Move MakeRandomMove() const;
  Square PlayerToSquare(const Player player) const noexcept;
  Player SquareToPlayer(const Square square) const noexcept;
  Winner SquareToWinner(const Square square) const noexcept;
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::ostream& operator<<(std::ostream& os, const Game& c);

} //~namespace con3
} //~namespace ribi

#endif // CONNECTTHREE_H
