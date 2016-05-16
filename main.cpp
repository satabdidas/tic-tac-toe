#include "Tictactoe.h"

#include <iostream>

int main() {
  Tictactoe tictactoe;
  std::cout << "Please enter <1> if you want to play with the computer."
    " Otherwise please enter <2> ";
  int player = 0;
  std::cin >> player;
  tictactoe.setMode(player);
  tictactoe.play();
  return 0;
}
