#include "BasicServer.h"

class MainServer : public BasicServer {
public:
    MainServer() {
        game_server = new GameServer();
        authorization_server = new AuthorizationServer();
    }

    ~MainServer() {
        delete game_server;
        delete authorization_server;
    }

    void exec() override {
        while (true) {
            // package acceptance and processing
        }
    }

    void login_user(User* user) {
        authorization_server->login_user(user);
    }

private:
    AuthorizationServer* authorization_server; 
    GameServer* game_server;
}