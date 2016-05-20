#ifndef __TIC_TAC_TOE_H__
#define __TIC_TAC_TOE_H__

#include <vector>
#include <string>
#include <utility>

class Tictactoe {
 public:

  class Mark {
  public:
    enum MarkEnum {NAUGHT, CROSS, BLANK};
    Mark();
    Mark(const Mark::MarkEnum& mark);
    Mark(const std::string& val);
    bool operator == (const Mark& mark) const;
    bool operator != (const Mark& mark) const;
    char getChar() const;
  private:
    MarkEnum  _mark;
  };

  using Player = std::pair<std::string, Mark>;

  void setMode(int mode);
  void printBoard() const;
  void play();

  Tictactoe();

 private:
  enum Mode {ONE_PLAYER, TWO_PLAYER, INVALID};

  using Marks = std::vector<Mark>;
  using Board = std::vector<Marks>; 

  std::string getPlayerName();
  Mark getPlayerMark(const std::string& player);
  std::pair<bool, bool> isGameOver();
  bool isADraw();
  bool isAWin();
  bool isAWinForRow();
  bool isAWinForColumn();
  bool isAWinForDiagonal();
  bool isAWinForALine(int row, int column, int dRow, int dColumn);
  void getPlayerMove(const Player& player);
  void getComputerMove();
  std::pair<int, int> getRowAndColumn();
  void report(std::pair<bool, bool>& status,
              const Player& player1,
              const Player& player2);
  bool isValidMove(int row, int column);
  Mark whoseTurn();
  Player getPlayer1();
  Player getPlayer2(const Mark& mark1);

  int         _row;
  int         _column;
  int         _k;
  int         _moveCount;
  Mark        _currentMark;
  Mode        _mode;
  Board       _board;
};

#endif /* __TIC_TAC_TOE_H__ */
