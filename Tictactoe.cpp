#include "Tictactoe.h"

#include <time.h>
#include <stdlib.h>

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

Tictactoe::Mark::Mark(const std::string& val) {
  if (val.compare("x") == 0 || val.compare("X") == 0) {
    _mark = CROSS;
  } else if (val.compare("x") == 0 || val.compare("X") == 0) {
    _mark = NAUGHT;
  } else {
    _mark = BLANK;
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
  _currentMark(Mark::BLANK),
  _mode(TWO_PLAYER),
  _board(_row, Marks(_column, Mark()))
{}

void Tictactoe::setMode(int player) {
  switch(player) {
  case 1:
    _mode = ONE_PLAYER;
    break;
  case 2:
    _mode = TWO_PLAYER;
    break;
  default:
    std::cout << "Invalid mode. Default mode of two player mode\n";
    _mode = TWO_PLAYER;
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

Tictactoe::Mark Tictactoe::whoseTurn() {
  if (_currentMark == Mark::NAUGHT) {
    return Mark(Mark::CROSS);
  } else {
    return Mark(Mark::NAUGHT);
  }
}

Tictactoe::Player Tictactoe::getPlayer1() {
 std::string name1 = "Computer";
  Mark mark1(Mark::CROSS);
  if (_mode == TWO_PLAYER) {
    name1 = getPlayerName();
    mark1 = getPlayerMark(name1);
  }
  return make_pair(name1, mark1);
}

Tictactoe::Player Tictactoe::getPlayer2(const Mark& mark1) {
  std::string name2 = getPlayerName();
  Mark mark2(mark1 == Mark::CROSS ? Mark::NAUGHT : Mark::CROSS);
  return std::make_pair(name2, mark2);
}

void Tictactoe::play() {
  Player player1 = getPlayer1();
  Player player2 = getPlayer2(player1.second);

  _currentMark = player2.second;
  printBoard();
  std::pair<bool, bool> status(false, false);
  while (!status.first && !status.second) {
    _currentMark = whoseTurn();
    if (player1.second == _currentMark) {
      if (_mode == ONE_PLAYER) {
        getComputerMove();
      } else {
        getPlayerMove(player1);
      }
    } else {
      getPlayerMove(player2);
    }
    ++_moveCount;
    printBoard();
    
    status = isGameOver();
  }
  report(status, player1, player2);
}

void Tictactoe::getComputerMove() {
  int row = -1;
  int column = -1;
  while (!isValidMove(row, column)) {
    std::pair<int, int> position = getRowAndColumn();
    row = position.first;
    column = position.second;
  }
  std::cout << "Computer move\n";
  std::cout << "Row: " << row << std::endl;
  std::cout << "Column: " << column << std::endl;
  _board[row][column] = Mark::CROSS;
}

std::pair<int, int> Tictactoe::getRowAndColumn() {
  srand(time(NULL));
  int row = rand() % 3;
  int column = rand() % 3;
  return std::make_pair(row, column);
}

bool Tictactoe::isValidMove(int row, int column) {
  bool validMove = true;
  if (row < 0 || row >= _row ||
      column < 0 || column >= _column) {
    validMove = false;
  } else if (_board[row][column] != Mark::BLANK) {
    validMove = false;
  }

  return validMove;
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
    std::cout << "Enter mark: ";

    std::string val;
    std::cin >> val;
    mark = val;
  }
  return mark;
}

std::pair<bool, bool> Tictactoe::isGameOver() {
  bool win = false;
  bool draw = false;

  if (isAWin()) {
    win = true;
  } else if (isADraw()) {
    draw = true;
  }
  return std::make_pair(win, draw);
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
  // Check for first column and last column
  for (int row = 1; row <= _row - _k; ++row) {
    if (isAWinForALine(row, 0, 1, 1) ||
        isAWinForALine(row, _column - 1, 1, -1)) {
      return true;
    }
  }
  // Check for first row positive slope
  for (int col = _column - 1; col >= _column -_k + 1; --col) {
    if (isAWinForALine(0, col, 1, -1)) {
      return true;
    }
  }
  return false;
}

bool Tictactoe::isAWinForALine(int row, int column, int dRow, int dColumn) {
  Mark mark = _currentMark;
  int count = 0;
  while (row < _row && column < _column && count < _k) {
    if (mark != _board[row][column]) {
      break;
    }
    ++count;
    row += dRow;
    column += dColumn;
  }
  if (count >= _k) {
    return true;
  }
  return false;
}

void Tictactoe::getPlayerMove(const Player& player) {
  std::cout << "Player " << player.first << " move\n";

  int row = -1;
  int column = -1;
  bool validMove = false;
  std::string val;
  while (!validMove){
    std::cout << "Enter row: ";
    std::cin >> val;
    row = std::stoi(val);
    std::cout << "Enter column: ";
    std::cin >> val;
    column = std::stoi(val);

    validMove = isValidMove(row, column);
  }
  
  _board[row][column] = player.second;
}

void Tictactoe::report(std::pair<bool, bool>& status,
                       const Player& player1,
                       const Player& player2) {
  if (status.second) {
    std::cout << "It's a draw\n";
  } else {
    std::cout << "The winner is ";
    if (_currentMark == player1.second) {
      std::cout << player1.first << std::endl;
    } else {
      std::cout << player2.first << std::endl;
    }
  }
}
