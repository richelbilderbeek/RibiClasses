#include "chessboardfactory.h"

#include <algorithm>
#include <cassert>
#include <numeric>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/make_shared.hpp>
#include "chessboard.h"
#include "chesspiecefactory.h"
#pragma GCC diagnostic pop

#include "testtimer.h"

ribi::Chess::BoardFactory::BoardFactory()
{
  #ifndef NDEBUG
  Test();
  #endif
}


boost::shared_ptr<ribi::Chess::Board> ribi::Chess::BoardFactory::Create(const Chess::Board::Pieces& pieces) const
{
  const boost::shared_ptr<Chess::Board> p
    = boost::make_shared<Chess::Board>(pieces);
  assert(p);
  return p;
}

boost::shared_ptr<ribi::Chess::Board> ribi::Chess::BoardFactory::DeepCopy(const Board& board) const
{
  const Chess::Board::ConstPieces original = board.GetPieces();
  Chess::Board::Pieces pieces;
  std::transform(original.begin(),original.end(),
    std::inserter(pieces,pieces.begin()),
    [](const Chess::Board::ConstPiecePtr& piece)
    {
      assert(piece);
      Chess::Board::PiecePtr p = PieceFactory().DeepCopy(piece);
      assert(p);
      return p;
    }
  );
  boost::shared_ptr<Board> b(Create(pieces));
  assert(b);
  return b;
}

#ifndef NDEBUG
void ribi::Chess::BoardFactory::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  {
    BoardFactory().Create();
  }
  const ribi::TestTimer test_timer(__func__,__FILE__,1.0);
}
#endif