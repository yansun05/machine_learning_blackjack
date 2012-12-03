// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun

#ifndef _DECK_H_
#define _DECK_H_

#include "card.h"

// Number of cards in the deck.
#define NUM_CARDS 52

// Representation of the deck of cards used in the game.
class Deck {
public:

    // Constructor of deck 
    Deck();  
  
    // Shuffle the deck of cards randomly.
    void Shuffle();
    
    // Draw and return pointer to next card from shuffled deck.
    // Return NULL pointer if we have run out of cards
    const Card* DrawNextCard(); 

    // Generate one random card.
    const Card& RandomCard() const;

private:

    // The whole 52 cards used by the game.
    Card cards_[NUM_CARDS];

    // Current position for drawing the card.
    int draw_card_pos_; 
};

#endif // _DECK_H_
