//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_TABLE_H
#define POKERAPP_TABLE_H


#include "common.h"
#include "Card.h"

class Table {
private:
    Card deck[54];
    Card* cards[5];
    money_t money;

public:
    Table() {
        for (auto& card : deck) {
            // deck init

            // now all deck is with the same cards
            card = Card(/* rank , suit */  Card::rank_t::Ace, Card::suit_t::Clubs );
        }
    };

    void spread_cards() {
        // init CARDS member
    }
};


#endif //POKERAPP_TABLE_H
