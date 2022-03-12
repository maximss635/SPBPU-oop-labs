//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_CARD_H
#define POKERAPP_CARD_H


class Card {
public:
    typedef enum {
        Diamonds, Hearts
        Spades, Clubs
    } suit_t;

    typedef enum {
        Ace,  King, Queen, Jack,
        ten, nine, eight, seven, six, five, four, three, two
    } rank_t;

    Card(rank_t rank, suit_t suit) : rank(rank), suit(suit) { }
private:
    rank_t rank;
    suit_t suit;
};


#endif //POKERAPP_CARD_H
