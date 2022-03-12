#include "BasicServer.h"

class GameServer : public BasicServer {
    std::vector<Game*> active_games;


public:

    void add_user_to_game(User* user, SingleGame* game) {
        game->add_player(user);
    }

    void remove_user_from_game(User* user, SingleGame* game);
    void buy_chips(User* user);
}