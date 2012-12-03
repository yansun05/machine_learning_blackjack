// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include "learning.h"
#include "game.h"

Player::Action QLearningPlayer::AskPlayerAction(Game* game) {
  // Update the state_
  if (state_value_ == NULL) {
    // It is the first state of the player in the begining of the game.
    // Init state_
    state_ = QState(GetCardHand(),
		    game->GetDealer()->GetDealerFaceUpCard());
    state_value_ = &(state_map_[state_]);
  } else if (last_action_ == Player::HIT_ACTION) {
    // Last action is HIT, I have one new card and I am not busted.
    // So Create a new state now.
    QState new_state(QState(GetCardHand(),
			    game->GetDealer()->GetDealerFaceUpCard()));
		     
    QStateValue* new_state_value = &(state_map_[new_state]);
    // Update current state qvalues based on the new state.
    UpdateQValue(state_value_, last_action_, new_state_value);

    // Switch to new state.
    state_ = new_state;
    state_value_ = new_state_value;
  }

  last_action_ = ChooseBestAction();
  return last_action_;
}

Player::Action QLearningPlayer::ChooseBestAction() const {
  // Compare two action and choose ones with bigger rewards.

  double stand_action_value =
    state_value_->action_values_[Player::STAND_ACTION].q_value_;

  double hit_action_value =
    state_value_->action_values_[Player::HIT_ACTION].q_value_;
  
  return stand_action_value >= hit_action_value ? Player::STAND_ACTION:
    Player::HIT_ACTION;
}

void QLearningPlayer::UpdateQValue(QStateValue* state_value,
				   Player::Action action,
				   QStateValue* child_state_value) {
  QStateValue::ActionValue& action_value =
    state_value->action_values_[action];
  ++action_value.count_;
  double max_q = max(child_state_value->action_values_[0].q_value_,
		     child_state_value->action_values_[1].q_value_);

  action_value.q_value_ = QLearningPlayer::QFunction(action_value.q_value_,
						     action_value.count_,
						     max_q);
}

void QLearningPlayer::TellResultFeedback(int game_result) {
  if (state_value_ == NULL) {
    // This only happens when the player's first 2 cards are busted or 21 points.
    // So this is a terminal state without any parent states.
    // No value in learing from this state. So we just return here.
    return;
  }
  state_value_->action_values_[last_action_].UpdateReward(game_result);
}

void QLearningPlayer::Reset() {
  state_value_ = NULL;
  last_action_ = Player::STAND_ACTION;

  Player::Reset();  
}


void QStateValue::ActionValue::UpdateReward(int game_result) {
  ++count_;

  // Calculate the reward
  int r;
  if (game_result < 0) {
    r = -1;
  } else if (game_result == 0) {
    r = 0;
  } else {
    r = 1;
  }

  q_value_ = QLearningPlayer::QFunction(q_value_, count_, r);
}

// Helper functions for calculating with Q-Learning formula.

// Alpha computes the learning rate.
static double Alpha(int n) {
  return 0.01;
}

// Discount factor.
static double gammar() {
  return 1.0;
}

double QLearningPlayer::QFunction(double q_value, int n, double max_q) {
  // Calculate using Q-Learning formula
  double alpha = Alpha(n);
  return q_value +  alpha * (0 +  gammar() * max_q - q_value);
}

