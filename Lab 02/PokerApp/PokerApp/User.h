//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_USER_H
#define POKERAPP_USER_H

#include "common.h"

class User {
public:
    class Profile;
    class GameStatistic;

    User() : profile(), statistic() {
        std::cout << "User created\n";
    }

    bool login(str_t& username, str_t& password) {
        if (username == profile.get_username() && \
        hash(password) == profile.get_password_hash()) {
            is_login = true;

            sql_connector.upload_user_from_database(this);
        }
        else {
            is_login = false;
        }

        return is_login;
    }

    void logout() { is_login = false; }

public:
    void add_money(money_t money_added) { this->money += money_added; }

public:
    class Profile {
        str_t       first_name;
        str_t       last_name;
        str_t       email_addr;
        str_t       username;
        hash_t      password_hash;
        short int   age;
    public:
        Profile() { std::cout << "Profile created\n"; }

        str_t get_username() const { return username; }
        hash_t get_password_hash() const { return password_hash; }
    };

    class GameStatistic {
        long int win;
        long int lose;
        long int total;
    public:
        GameStatistic() { std::cout << "GameStatistic created\n"; }

        void update(gameRes_t game_res) {
            total++;
            (game_res == gameRes_t::lose) ? (lose++) : (win++);
        }
    };


private:
    ull             id;
    Profile         profile;
    GameStatistic   statistic;
    bool            is_login;
    money_t         money;
};


#endif //POKERAPP_USER_H
