// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include "deck.h"
#include <stdlib.h>

Deck::Deck():
    draw_card_pos_(0) {
    // Initialize the 52 cards in cards_ with right suits and values.
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 13; ++j)
        {
            cards_[i * 13 + j].SetValue(Card::Value(2 + j));   
            cards_[i * 13 + j].SetSuit(Card::Suit(i));
        }
    }
}

// Implement the Knuth shuffle algorithm:
// 	http://en.wikipedia.org/wiki/Random_permutation
void Deck::Shuffle() {
    for (int i = 0; i < NUM_CARDS; ++i)
    {
	// Generate a random number in range [0, NUM_CARDS - i).
	int random_num  = rand() % (NUM_CARDS - i);

	// Swap the card[random_num] and card[NUM_CARDS - 1 - i].
        Card temp = cards_[NUM_CARDS - 1 - i]; 
	cards_[NUM_CARDS - 1 - i ] = cards_[random_num];
        cards_[random_num] = temp;
    }
    // Reset next draw card position.
    draw_card_pos_ = 0;
}

const Card* Deck::DrawNextCard() {
  if (draw_card_pos_ > NUM_CARDS - 1) {
      cerr << "Player::DealCard: used up all cards. Byebye!\n";
      exit(0);
  }
  return &cards_[draw_card_pos_++];
}

const Card& Deck::RandomCard() const
{
    // Generate a random number in range [0, NUM_CARDS).
    int random_num  = rand() % NUM_CARDS;
    return cards_[random_num];
}
