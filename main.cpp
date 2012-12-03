// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

int main(int argc, char**argv) {
  cout << "Welcome to BlackJack!\n" 
          "This game is made by Yan Sun as "
          "part of a wonderful Arificial Intelligence learning experience."
       << endl;

  // Initialize random number generator.
  srand(time(NULL));

  Game::Run();

  return 0;
}
