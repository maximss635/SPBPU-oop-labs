//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_TOURNAMENT_H
#define POKERAPP_TOURNAMENT_H


#include "Game.h"

class Tournament : public Game {
    std::vector<User*> participants;
    std::vector<Game*> games;

public:
    bool registate(User* user) { participants.push_back(user); }
    void start();
    void end(User* winner);
};


#endif //POKERAPP_TOURNAMENT_H
