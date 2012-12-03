// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#include <stdio.h>
#include "card.h"

// Convert a char representing the card value to the card enum value.
static Card::Value ParseCardValue(char value_char) {
  // Handle all non-numeric cases.
  switch(value_char) {
  case 'T':
    return Card::TEN;
  case 'J':
    return Card::JACK;
  case 'Q':
    return Card::QUEEN;
  case 'K':
    return Card::KING;
  case 'A':
    return Card::ACE;
  }

  // Now handle the numeric cases.
  if (value_char >= '2' && value_char <= '9') {
    return Card::Value(value_char - '2' + 2);
  }

  cerr << "Error: ParseCardValue: bad value_char=" << value_char << endl;
  return Card::TWO;
}

// Convert a char representing the card suite to the card suit enum value.
static Card::Suit ParseCardSuit(char suit_char) {
  switch(suit_char) {
  case 'C':
    return Card::CLUBS;
  case 'D':
    return Card::DIAMONDS;
  case 'H':
    return Card::HEARTS;
  case 'S':
    return Card::SPADES;
  }

  cerr << "Error: ParseCardSuit: bad suit_char=" << suit_char << endl;
  return Card::CLUBS;
}

// Construct the card from its string representation (e.g. 2C, AH, 10D).
Card::Card(const string& card_string) {
  if (card_string.size() >= 2) {
    // Last char is the card suite.
    suit_ = ParseCardSuit(card_string[card_string.size() - 1]);

    // card_string[0] or card_string[0..1] (case of 10D) is the card value.
    // We use char 'T' to represent value 10.
    value_ = ParseCardValue(card_string.size() == 2 ? card_string[0]:
			    'T');
  }
}

string Card::ToString() const {
  char suit_char = "CDHS"[int(suit_)];
  // String representation has 3 chars at most.
  char buf[4];
  if (value_ < Card::JACK) {
    sprintf(buf, "%d%c", int(value_), suit_char);
  } else {
    sprintf(buf, "%c%c", "JQKA"[value_ - Card::JACK], suit_char);
  }
  return string(buf);
}

int Card::GetBlackJackPoint() const {
  // Return 1 for ACE, though the point can be changed to 11 depends
  // on which way it better for user.
  if (value_ == Card::ACE) {
    return 1;
  }
  // Return 10 for Jack, Queen and King.
  // Return face value for other numer-valued cards.
  return value_ < Card::TEN ? int(value_): 10;
}
