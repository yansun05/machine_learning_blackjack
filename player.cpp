// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "deck.h"
#include "game.h"
#include "player.h"
#include "learning.h"

using namespace std;

Player* Player::CreatePlayer(PlayerType player_type, bool is_dealer) {
  switch(player_type) {
  case Player::RANDOM_PLAYER:
    return new RandomPlayer(is_dealer);
  case Player::FIXED_STRATEGY_PLAYER:
    return new FixedStrategyPlayer(is_dealer);
  case Player::QLEARNING_PLAYER:
    return new QLearningPlayer(is_dealer);
  };
  cerr << "Player::CreatePlayer: Bad player_type=" << player_type << endl;
  return NULL;
}

void Player::OutputPlayerInfo() const {   
    // Output all cards the player has.
    for (unsigned int i = 0; i < cards_.size(); ++i) {
      const Card* card = cards_[i];
      if (i) {
	cout << ' ';
      }
      if (card != face_down_card_) {
	cout << card->ToString();
      } else {
	cout << "XX";
      }
    }
}

void Player::DealCard(const Card* card, bool face_down) {
    cards_.push_back(card);
    if (face_down) {
      face_down_card_ = card;
    }
    card_hand_.AddCard(*card);
}

void Player::FlipFaceDownCard() {
  if (face_down_card_ != NULL) {
    face_down_card_ = NULL;
  }
}

const Card& Player::GetDealerFaceUpCard() const {
  const Card* face_up_card = NULL;
  for (unsigned int i = 0; i < cards_.size(); ++i) {
    if (cards_[i] != face_down_card_) {
      face_up_card = cards_[i];
    }
  }
  if (face_up_card == NULL) {
    cerr << "Player::GetDealerFaceUpCard: cannot find dealer face up card.\n";
    exit(0);
  }
  return *face_up_card;
}

// Random Player
Player::Action RandomPlayer::AskPlayerAction(Game* game) {
  // Random player makes decision using a random number generator.
  // Note: Player::Action is defined as:
  // STAND_ACTION = 0
  // HIT_ACTION = 1
  return Player::Action(rand() % 2);
}

// Fixed Strategy Player
Player::Action FixedStrategyPlayer::AskPlayerAction(Game* game) {
  // Fixed Stragegy:
  // HIT if my card points < 17.
  // STAND if my card points >= 17.

  int points = GetPoints();
  return points < 17 ? HIT_ACTION : STAND_ACTION;
}
