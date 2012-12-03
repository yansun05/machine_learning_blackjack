// Author: Yan Sun
// Copyright (c) 2011 All Right Reserved, Yan Sun.

#ifndef _CARD_H_
#define _CARD_H_

#include <iostream>

using namespace std; 

// Representation of a card in poker game.
class Card
{
/*
  CARDS

  There are 13 values. They are as follows when represented by an integer and by a character:
  2=2, 3=3, 4=4, 5=5, 6=6, 7=7, 8=8, 9=9, 10=T, 11=J, 12=Q, 13=K, 14=A
  */
public:
    // Set TWO to be integer value 2 so that the value of a card
    // can be compared directly vith face value.
    enum Value {TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN= 7,
                EIGHT = 8, NINE = 9, TEN = 10,
                JACK = 11, QUEEN = 12, KING = 13, ACE = 14};

    enum Suit {CLUBS = 0, DIAMONDS = 1, HEARTS = 2, SPADES = 3};

    //overloading << operator so can send Card object to standard output
   // friend ostream& operator<<(ostream& os, const Card& card);
   //    Card(Value r, Suit s):  value_(r), suit_(s) {
   // }
 
    Card() {
        value_ = TWO;
        suit_ = CLUBS;
    }

    // Construct the card from its string representation (e.g. 2C, AH, 10D).
    Card(const string& card_string);

    // Convert the card to a string representation (e.g. 2C, AH, 10D).
    string ToString() const;
  
    Value GetValue() const {
	return value_;
    } 

    Suit GetSuit() const {
	return suit_;
    }
  
    void SetValue(Value cvalue) {
        value_ = cvalue;
    }

    void SetSuit(Suit csuit) {
        suit_ = csuit;
    }

  bool operator==(const Card& card) const {
    return value_ == card.value_ && suit_ == card.suit_;
  }

  // Return the point of this card in the BlackJack game.
  int GetBlackJackPoint() const;

private:

    // Face vaule of the card.
    Value value_;

    // Suit of the card.
    Suit suit_;
};

#endif // _CARD_H_
