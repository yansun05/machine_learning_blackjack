// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include "game.h"
#include <iostream>
#include <string>

using namespace std;

Deck Game::deck_;

Game::Game(Player* player, Player* dealer, bool show_game_process):
  player_(player),
  dealer_(dealer),
  show_game_process_(show_game_process) {
}

Game::~Game()
{
}

// Distribute hole cards to all players.
void Game::DistributeCards() {
  // Deal two cards to dealer (one face-up, one face-down).
  dealer_->DealCard(deck_.DrawNextCard(), false);
  dealer_->DealCard(deck_.DrawNextCard(), true);

  // Deal two face-up cards to player.
  player_->DealCard(deck_.DrawNextCard(), false);
  player_->DealCard(deck_.DrawNextCard(), false);
}

// Run HIT action by a player.
void Game::RunHitAction(Player* player) {
  if (show_game_process_) {
    cout << player->GetName() << " decides to HIT\n";
  }  
  const Card* card = deck_.DrawNextCard();
  // All cards dealt on HIT action are face-up.
  player->DealCard(card, false);

  if (show_game_process_) {
    cout << "Dealing one card to " << player->GetName() << " ...\n";
  }
}

// Run all actions chosen by a player.
// Return final points of the player's cards.
int Game::RunPlayer(Player* player) {
    int points;
    // Loop until the player's card >= 21 points or the player decides to
    // stand.
    while (true) {
      OutputGameInfo();
      points = player->GetPoints();
      if (points >= 21) {
	break;
      }
      // Ask the player to make decision.
      Player::Action action = player->AskPlayerAction(this);
      if (action == Player::STAND_ACTION) {
        if (show_game_process_) {
	  cout << player->GetName() << " decides to STAND\n";
        }
	break;
      } else {
	RunHitAction(player);
      }
    }
    return points;
}

// Run a complete game from shuffling cards to the end.
// Returns negive value if dealer wins; returns 0 if it is a tie;
// returns positive value if player wins.
int Game::RunOneGame() {
  Output("\n\n==========================================================\n");
  Output("New game started...\n");
  // A new hand now. Shuffle and Distribute cards
  Output("Randomly shuffling cards ...\n");
  deck_.Shuffle();

  // Distribute hole cards to player and dealer
  Output("Dealing hole cards to player and dealer ...\n");
  DistributeCards();

  //  OutputGameInfo();
    
  // Let the player play first.
  int player_points = RunPlayer(player_);
  if (player_points > 21) {
    Output("Player busted!\n");
    return -1;
  }

  // Flip the face-down card of the dealer.
  Output("Flip face-down card of the dealer\n");
  dealer_->FlipFaceDownCard();

  // Let the dealer play second.
  int dealer_points = RunPlayer(dealer_);
  if (dealer_points > 21) {
    Output("Dealer busted!\n");
    return 1;
  }
  if (player_points < dealer_points) {
    if (show_game_process_) {
      cout << "Dealer wins: player_points=" << player_points
	   << " dealer_points=" << dealer_points << '\n';
    }
    return -1;
  } else if (player_points == dealer_points) {
    if (show_game_process_) {
      cout << "Player and dealer tie: player_points=" << player_points
	   << " dealer_points=" << dealer_points << '\n';
    }
    return 0;
  } else {
    if (show_game_process_) {
      cout << "Player wins: player_points=" << player_points
	   << " dealer_points=" << dealer_points << '\n';
    }
    return 1;
  }
}

static double GetPercentage(unsigned long a, unsigned long b) {
  if (b != 0) {
    return a / double(b) * 100;
  } else {
    return 0;
  }
}

void Game::Run() {
  // Dealer always uses Fixed Strategy
  FixedStrategyPlayer dealer(true);
  while (true) {
    Player::PlayerType player_type;
    // Loop until user makes a valid choices.
    while (true) {
      // Output a menu to let user select what to do.
      cout << "Select: 1)Run Q-Learning Player 2)Run Random Player "
	"3)Run Fixed-Strategy Player 4)Quit ?";
      string cmd;
      cin >> cmd;

      if (cmd == "4") {
	cout << "Nice games. Goodbye!\n";
	return;
      }
      if (cmd == "1") {
	player_type = Player::QLEARNING_PLAYER;
	break;
      } else if (cmd == "2") {
	player_type = Player::Player::RANDOM_PLAYER;
	break;
      } else if (cmd == "3") {
	player_type = Player::Player::FIXED_STRATEGY_PLAYER;
	break;
      } else {
	cout << "Invalid choice. Please try again.\n";
      }
    }
    // Now we have a valid player type. Ask user how many times to play.
    cout << "Play how many times ?";
    unsigned long run_times;
    cin >> run_times;

    // Create player and dealer
    Player* player = Player::CreatePlayer(player_type, false);
   
    unsigned long win_times = 0;
    unsigned long lose_times = 0;
    unsigned long tie_times = 0;
    // Run the game for run_times.
    for (unsigned long i = 0; i < run_times; ++i) {
      // We only display gaming process when run_times <= 100.
      Game game(player, &dealer, run_times <= 100);
      int game_result = game.RunOneGame();
      // Add training
    //  if (i > 1000000) {
      if (game_result > 0) {
	++win_times;
      } else if (game_result == 0) {
	++tie_times;
      } else if (game_result < 0) {
	++lose_times;
      }
      //}
      // Tell player about this game result.
      // This is useful for Machine Leaning based player.
      player->TellResultFeedback(game_result);

      // Reset the player/dealer to prepare for next game.
      player->Reset();
      dealer.Reset();
    }

    // free player object after game finishes.
    delete player;

    // Output game statistics.
    cout << "Total_run_times=" << run_times
	 << " win_times=" << win_times
	 << " (" << GetPercentage(win_times, run_times) << "%)"
	 << " tie_times=" << tie_times
	 << " (" << GetPercentage(tie_times, run_times) << "%)"
	 << " lose_times=" << lose_times
	 << " (" << GetPercentage(lose_times, run_times) << "%)"
         << endl;
  }
}

void Game::Output(const string& message) const {
  if (show_game_process_) {
    cout << message;
  }
}

void Game::OutputGameInfo() const {
  if (show_game_process_) {
    cout << "========================================================\n";
    cout << "Dealer: ";
    dealer_->OutputPlayerInfo();
    cout << "\nPlayer: ";
    player_->OutputPlayerInfo();
    cout << '\n';
  }
}
