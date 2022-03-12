//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_PLAYER_H
#define POKERAPP_PLAYER_H


#include "User.h"

class Player : public User {
    Game* current_game;
    Card* cards[2];

    /**
        Actions in game
     */
    void bet(money_t rate);
    void call();
    void raise(money_t rate);
    void fold();
    void check();

};


#endif //POKERAPP_PLAYER_H
