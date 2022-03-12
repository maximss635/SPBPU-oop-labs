#include "BasicServer.h"

class AuthorizationServer : public BasicServer {
public:
    str_t login_user(User* user) {
        // make new notes in database
        // look for information about this user
        // if find - return Token 

        this->repository(/*....*/);
        this->repository(/*....*/);

        this->repository(/*....*/);
    }

    void logout_user(User* user);
}