// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#ifndef _LERNING_H_
#define _LERNING_H_

#include <hash_map>
#include "player.h"

using namespace __gnu_cxx; // For using hash_map

// State in Q-Learning algorithm.
class QState {
 public:
  QState() {}

  QState(const CardHand& my_hand, const Card& dealer_faceup_card):
    my_hand_(my_hand), dealer_faceup_card_value_(dealer_faceup_card.GetValue()) {
  }

  bool operator==(const QState& other) const {
    return my_hand_ == other.my_hand_ && dealer_faceup_card_value_ ==
      other.dealer_faceup_card_value_;
  }

  unsigned long GetHash() const {
    unsigned long h = dealer_faceup_card_value_;
    return (h << 32) | my_hand_.GetHash();
  }


  // Data members

  // My card hand.
  CardHand my_hand_;

  unsigned int dealer_faceup_card_value_;
};

namespace __gnu_cxx
{
  template<> struct hash<QState>
  {
    size_t operator()(const QState& qstate) const
    {
      return hash<long unsigned int>()(qstate.GetHash());
    }
  };
}

// Value associated with each QState object.
class QStateValue {
public:
  struct ActionValue {
    // q_value (reward) value for the state/action pair.
    double q_value_;
    // Count of times this action has been taken on the state.
    int count_;

    ActionValue(): q_value_(0), count_(0) {
    }

    void UpdateReward(int game_result);
  };

 // action_values_[STAND_ACTION] is the q_value for
  // STAND_ACTION;
  // action_values_[HIT_ACTION] is the q_value for HIT_ACTION.
  ActionValue action_values_[2];
};

// Representation of a player driven by QLearning Algorithm.
class QLearningPlayer: public Player {
public:
  QLearningPlayer(bool is_dealer): Player(is_dealer), state_value_(NULL),
				   last_action_(Player::STAND_ACTION) {
  }

  // Ask player what action to perform.
  Action AskPlayerAction(Game* game);

  void TellResultFeedback(int game_result);

  // Reset the player after a game.
  virtual void Reset();

  // Calculating QFunction.
  static double QFunction(double q_value, int n, double max_q);

private:

  // Choose best action according to current state.
  Action ChooseBestAction() const;

  // Update the q_value for pair of state and action based on child state.
  void UpdateQValue(QStateValue* state_value, Player::Action action,
		    QStateValue* child_state_value);

  // HashMap which maps State to StateValue.
  typedef hash_map<QState, QStateValue> StateMap;
  StateMap state_map_;  

  // Current State.
  QState state_;
  // Pointer to state_map[state_];
  QStateValue* state_value_;

  // Last action the player takes.
  Player::Action last_action_;
};


#endif // _LEARNING_H_
