//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_GAME_H
#define POKERAPP_GAME_H

#include <vector>
#include "Player.h"
#include "Table.h"

class Game {
public: 
    virtual void start();
    virtual void end(Player* winner);

    Player* winner;
    std::vector<Player*> players;

}

class SingleGame : public Game {
public:
    SingleGame() : table() {}

private:
    Table                   table;
    money_t                 last_rate;
    
    const int max_players = 5;
public:
    void start();
    void end(Player* winner);

    bool add_player(Player* player) {
        if (players.size() < max_players) {
            players.push_back(player);
            return true;
        }

        return false;
    }
};


#endif //POKERAPP_GAME_H
