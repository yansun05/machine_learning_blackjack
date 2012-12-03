// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include <vector>

#include "card.h"

class Game;

// Summary of the hand of cards which a player or dealer has.
class CardHand {
public:
  CardHand(): card_points_(0),
	      exist_ace_(false) {
  }

  bool Empty() const {
    return card_points_ == 0;
  }


  bool operator==(const CardHand& other) const {
    return exist_ace_ == other.exist_ace_ &&
      card_points_ == other.card_points_;
  }

  // Add one Card to the hand.
  void AddCard(const Card& card) {
    if (card.GetValue() == Card::ACE) {
      exist_ace_ = true;
      card_points_ += 1;
    }
    else {
      card_points_ += card.GetBlackJackPoint();
    }
  }

  // Get Points of the Card Hand.
  int GetPoints() const {
    if (exist_ace_) {
      // Try to use the ace as point 11.
      int points2 = card_points_ + 10;
      if (points2 <= 21) {
	return points2;
      }
    }
    return card_points_;
  }

  unsigned int GetHash() const {
    return card_points_ * 2 + (exist_ace_ ? 1: 0);
  }

  void Reset() {
    card_points_ = 0;
    exist_ace_ = false;
  }

  // Total points of the player's cards(counting each ACE as point 1).
  int card_points_;

  // Do I have at least one ACE?  Used for computing alternative total point
  // values by counting ACE as pount 11.
  // Note: even if there are multuple ACE cards, at most one ACE card can be
  // counted as point 11; otherwise, it will go BURST.
  bool exist_ace_;
};


// Representation of a player in poker game.
class Player
{
 public:

  enum PlayerType {
    RANDOM_PLAYER = 0,
    FIXED_STRATEGY_PLAYER = 1,
    QLEARNING_PLAYER = 2
  };

  // Create a Player object of given player type.
  static Player* CreatePlayer(PlayerType player_type, bool is_dealer);

  // Possible action of a player.
  enum Action {
    STAND_ACTION = 0,
    HIT_ACTION = 1
  };

  // Deal one card to the player.
  void DealCard(const Card* card, bool face_down);

  int GetPoints() const {
    return card_hand_.GetPoints();
  }

  const char* GetName() const {
    return is_dealer_ ? "Dealer" : "Player";
  }

  // Flip the face down card of the dealer.
  void FlipFaceDownCard();

  // Output information about player.
  // Called only when show_game_process == true.
  void OutputPlayerInfo() const;

  // Ask player what action to perform.
  virtual Action AskPlayerAction(Game* game) = 0;

  // Tell Player about the game result as feedback.
  // This is useful for Machine Learning based player.
  // "game_result": negative if the player loses; 0 if ties; positive if wins.
  virtual void TellResultFeedback(int game_result) {
    // Do nothing by default.
  }

  const CardHand& GetCardHand() const {
    return card_hand_;
  }

  // Returns the only face up card of the dealer.
  const Card& GetDealerFaceUpCard() const;

  // Reset the player after a game.
  virtual void Reset() {
    // Note: is_dealer_ is unchanged.
    cards_.resize(0);
    face_down_card_ = 0;
    card_hand_.Reset();
  }

 protected:
  // Default constructor of player.     
  Player(bool is_dealer): is_dealer_(is_dealer),
			  face_down_card_(NULL) {
  }


  bool is_dealer_;

  // The cards distributed to the player/dealer.
  vector<const Card*> cards_;

  // Only one card (of the dealer) is face-down.
  // Set it to NULL when all cards are face-up.
  const Card* face_down_card_;

  //Summary of the card had.
  CardHand card_hand_;
};

// Representation of a player with random strategy.
class RandomPlayer: public Player {
public:
  RandomPlayer(bool is_dealer): Player(is_dealer) {
  }

  // Ask player what action to perform.
  Action AskPlayerAction(Game* game);
};

// Representation of a player with fixed strategy.
// Dealer is always such a player.
class FixedStrategyPlayer: public Player {
public:
  FixedStrategyPlayer(bool is_dealer): Player(is_dealer) {
  }

  // Ask player what action to perform.
  virtual Action AskPlayerAction(Game* game);
};

#endif // _PLAYER_H
