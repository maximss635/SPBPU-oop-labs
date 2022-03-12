#include "common.h"
#include "User.h"
#include "Repository.h"

class BasicServer {
public:
    BasicServer() {
        repository = new Repository();
    }  

    virtual ~BasicServer() {
        delete Repository;
    }

    // this method must be overloaded in childrens
    virtual void exec() = 0;

private:
    // address
    ipv4_t ip;

    // class which communicates with the database
    Repository* repository;

    std::vector<User*> connected_users;
}