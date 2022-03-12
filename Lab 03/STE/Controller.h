//
// Created by maxim on 17.11.2020.
//

#ifndef STE_CONTROLLER_H
#define STE_CONTROLLER_H

#include "Kernel.h"


class Controller {
    Kernel& kernel;

    enum class SymbolType {
        symbol, arrow, enter, backspace, del, exit, nothing
    };

public:
    explicit Controller(Kernel& kernel) : kernel(kernel) {
        halfdelay(1);         //  Set delay for getch - 1 msec
        Journal::log("Controller init");
    }

    ~Controller() {
        Journal::log("Controller deinit");
    }

    void main_loop();



private:
    SymbolType type_handler(int code) const;
    void arrow_handler(int code);

    //  first symbols is 0x1b, 0x5b
    const int MY_KEY_UP    = 0x41;
    const int MY_KEY_DOWN  = 0x42;
    const int MY_KEY_RIGHT = 0x43;
    const int MY_KEY_LEFT  = 0x44;

    const int MY_KEY_EXIT      = 0x71;
    const int MY_KEY_BACKSPACE = 0x7f;
    const int MY_KEY_DELETE    = 0x7e;


};


#endif //STE_CONTROLLER_H
