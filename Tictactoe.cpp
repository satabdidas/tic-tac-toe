#include "Tictactoe.h"

#include <iostream>

namespace {

std::ostream& operator << (std::ostream& os, const Tictactoe::Mark& mark) {
  os << mark.getChar();
  return os;
}

}

Tictactoe::Mark::Mark() :
  _mark(BLANK)
{}

Tictactoe::Mark::Mark(const Mark::MarkEnum& mark) {
  _mark = mark;
}

Tictactoe::Mark::Mark(char val) {
  switch(val) {
  case 'x':
  case 'X':
    _mark = CROSS;
    break;
  case 'o':
  case 'O':
    _mark = NAUGHT;
    break;
  default:
    _mark = BLANK;
    break;
  }
}

bool Tictactoe::Mark::operator != (const Mark& mark) const {
  return !(*this == mark);
}

bool Tictactoe::Mark::operator == (const Mark& mark) const {
  return mark._mark == _mark;
}

char Tictactoe::Mark::getChar() const {
  char val = ' ';
  switch (_mark) {
  case NAUGHT:
    val = 'O';
    break;
  case CROSS:
    val = 'X';
    break;
  default:
    break;
  }
  return val;
}

Tictactoe::Tictactoe() :
  _row(3),
  _column(3),
  _k(3),
  _moveCount(0),
  _winnerMark(Mark::BLANK),
  _mode(TWO_PLAYER),
  _board(_row, Marks(_column, Mark()))
{}

void Tictactoe::setMode(int player) {
  switch(player) {
  case 1:
    break;
  case 2:
    _mode = TWO_PLAYER;
    break;
  default:
    _mode = INVALID;
    break;
  }
}

void Tictactoe::printBoard() const {
  auto printRow = [] (const Marks& marks) {
    std::cout << "|";
    std::for_each(marks.begin(), marks.end(),
                  [](const Mark& mark) { std::cout << " " << mark << " |"; });
    std::cout << std::endl;
  };
  std::for_each(_board.begin(), _board.end(), printRow);
}

void Tictactoe::play() {
  if (_mode == ONE_PLAYER) {
    std::cout << "Ooops! Not yet supported\n";
  } else if (_mode == TWO_PLAYER) {
    playTwoPlayerMode();
  }
}

void Tictactoe::playTwoPlayerMode() {
  std::string player1 = getPlayerName();
  Mark mark1 = getPlayerMark(player1);

  std::string player2 = getPlayerName();
  Mark mark2 = mark1 == Mark::CROSS ? Mark::NAUGHT : Mark::CROSS;

  printBoard();

  int turn = 1;
  getPlayerMove(player1, mark1);
  printBoard();
  while (!isGameOver()) {
    if (!(turn & 1)) {
      getPlayerMove(player1, mark1);
    } else {
      getPlayerMove(player2, mark2);
    }
    ++turn;
    printBoard();
  }

  std::string winner;
  if (_winnerMark == mark1) {
    winner = player1;
  } else if (_winnerMark == mark2) {
    winner = player2;
  }
  report(winner);
}

std::string Tictactoe::getPlayerName() {
  std::cout << "Enter player name: ";
  std::string name;
  std::cin >> name;
  return name;
}

Tictactoe::Mark Tictactoe::getPlayerMark(const std::string& player) {
  std::cout << "Select mark for player " << player
            << ". You can select either O or a X: ";
  Mark mark;
  while (mark == Mark::BLANK) {
    char val;
    std::cin >> val;
    mark = val;
  }
  return mark;
}

bool Tictactoe::isGameOver() {
  if (isADraw() || isAWin()) {
    return true;
  }
  return false;
}

bool Tictactoe::isADraw() {
  return _moveCount == _row * _column;
}

bool Tictactoe::isAWin() {
  return isAWinForRow() || isAWinForColumn() || isAWinForDiagonal();
}

bool Tictactoe::isAWinForRow() {
  for (int row = 0; row < _row; ++row) {
    if (isAWinForALine(row, 0, 0, 1)) {
      return true;
    }
  }
  return false;
}

bool Tictactoe::isAWinForColumn() {
  for (int col = 0; col < _column; ++col) {
    if (isAWinForALine(0, col, 1, 0)) {
      return true;
    }
  }
  return false;
}

bool Tictactoe::isAWinForDiagonal() {
  // Check for first row negative slope
  for (int col = 0; col <= _column - _k; ++col) {
    if (isAWinForALine(0, col, 1, 1)) {
      return true;
    }
  }
  // Check for first column
  for (int row = 1; row <= _row - _k; ++row) {
    if (isAWinForALine(row, 0, 1, 1)) {
      return true;
    }
  }
  // Check for first row positive slope
  for (int col = _column - 1; col >= _column -_k + 1; --col) {
    if (isAWinForALine(0, col, 1, -1)) {
      return true;
    }
  }
  // Check for last column
  for (int row = 1; row <= _row - _k; ++row) {
    if (isAWinForALine(row, _column - 1, 1, -1)) {
      return true;
    }
  }
  return false;
}

bool Tictactoe::isAWinForALine(int row, int column, int dRow, int dColumn) {
  Mark mark = _board[row][column];
  int count = 0;
  while (row < _row && column < _column && count < _k) {
    if (mark != Mark::BLANK && mark == _board[row][column]) {
      ++count;
    } else {
      count = 1;
      mark = _board[row][column];
    }
    row += dRow;
    column += dColumn;
  }
  if (count >= _k) {
    _winnerMark = mark;
    return true;
  }
  return false;
}

void Tictactoe::getPlayerMove(const std::string& player,
                              const Mark& mark) {
  std::cout << "Player " << player << " move\n";

  int row = -1;
  int column = -1;
  bool validMove = false;
  while (!validMove){
    std::cout << "Enter row: ";
    std::cin >> row;
    std::cout << "Enter column: ";
    std::cin >> column;

    if (row < 0 || row >= _row ||
        column < 0 || column >= _column) {
      std::cout << "Enter a valid move.\n";
    } else if (_board[row][column] != Mark::BLANK) {
      std::cout << "Cell is not empty. Enter a valid move.\n";
    } else {
      validMove = true;
    }
  }
  
  ++_moveCount;
  _board[row][column] = mark;
}

void Tictactoe::report(const std::string& winner) {
  if (winner.empty()) {
    std::cout << "It's a draw\n";
  } else {
    std::cout << "The winner is " << winner << std::endl;
  }
}
