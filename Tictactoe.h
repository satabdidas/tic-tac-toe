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
    Mark(char val);
    bool operator == (const Mark& mark) const;
    bool operator != (const Mark& mark) const;
    char getChar() const;
  private:
    MarkEnum  _mark;
  };

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
  bool isGameOver();
  bool isADraw();
  bool isAWin();
  bool isAWinForRow();
  bool isAWinForColumn();
  bool isAWinForDiagonal();
  bool isAWinForALine(int row, int column, int dRow, int dColumn);
  void getPlayerMove(const std::string& player,
                     const Mark& mark);
  void getComputerMove(const Mark& mark);
  std::pair<int, int> getRowAndColumn();
  void report(const std::string& winner);
  bool isValidMove(int row, int column);
  Mark whoseTurn();

  int         _row;
  int         _column;
  int         _k;
  int         _moveCount;
  Mark        _winnerMark;
  Mark        _currentMark;
  Mode        _mode;
  Board       _board;
};

#endif /* __TIC_TAC_TOE_H__ */
