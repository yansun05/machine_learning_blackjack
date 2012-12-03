// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#ifndef _GAME_H_
#define _GAME_H_

#include "player.h"
#include "deck.h"

// Representation of game.
class Game {
public:
  // Distribute cards to players and assign dealer
  void DistributeCards();

  // Main funtion to run the game process.
  static void Run(); 
    
   const Deck& GetDeck() const
  {
    return deck_;
  }
     
  ~Game();

  // Output to the console if show_game_process_ == true.
  void Output(const string& message) const;

  const Player* GetDealer() const {
    return dealer_;
  }

private:
  // Constructor: "player_type" is the type of the player;
  //              "show_game_process" is true iff game process will be
  //              displayed on console.
  Game(Player* player, Player* dealer, bool show_game_process);

  // Output the game information if show_game_process_ == true;
  void OutputGameInfo() const;

  // Run HIT action by a player.
  void RunHitAction(Player* player);

  // Run all actions chosen by a player.
  // Return final points of the player's cards.
  int RunPlayer(Player* player);

  // Run a complete game from shuffling cards to the end.
  // Returns negive value if dealer wins; returns 0 if it is a tie;
  // returns positive value if player wins.
  int RunOneGame();

  // Deck of cards from which cards are dealt. 
  static Deck deck_;

  Player* player_;

  Player* dealer_;

  // Should we output information about the game on the console?
  bool show_game_process_;
};

#endif // _GAME_H
