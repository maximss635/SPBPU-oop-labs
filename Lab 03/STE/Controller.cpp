//
// Created by maxim on 17.11.2020.
//

#include "Controller.h"


void Controller::main_loop() {
    char c;
    SymbolType type;

    while (true) {
        c = getch();

        if (c == ERR) {
            kernel.update_logo();
            continue;
        }

        type = type_handler(c);


        if (type == SymbolType::symbol) {
            // add char to memory
            kernel.insert_char(static_cast<char>(c));
        }

        else if (type == SymbolType::enter) {
            kernel.insert_enter();
        }

        else if (type == SymbolType::arrow) {
            arrow_handler(c);
        }

        else if (type == SymbolType::backspace) {
            kernel.delete_char(true);
        }

        else if (type == SymbolType::del) {
            // Not implemented yet
        }

        else if (type == SymbolType::exit) {
            break;
        }
    }
}


Controller::SymbolType Controller::type_handler(int code) const {
    if (code == MY_KEY_UP || code == MY_KEY_DOWN || code == MY_KEY_LEFT || code == MY_KEY_RIGHT) {
        return Controller::SymbolType::arrow;
    }

    if (code == MY_KEY_BACKSPACE) {
        return Controller::SymbolType::backspace;
    }

    if (code == MY_KEY_DELETE) {
        return Controller::SymbolType::del;
    }

    if (code == MY_KEY_EXIT) {
        return Controller::SymbolType::exit;
    }

    if (code == '\n') {
        return Controller::SymbolType::enter;
    }

    if (code != 0x1b && code != 0x5b) {     // ????
        return Controller::SymbolType::symbol;
    }

    return Controller::SymbolType::nothing;
}


void Controller::arrow_handler(int code) {
    if (code == MY_KEY_UP) {
        kernel.cursor->up();
    }
    else if (code == MY_KEY_DOWN) {
        kernel.cursor->down();
    }
    else if (code == MY_KEY_RIGHT) {
        kernel.cursor->right();
    }
    else if (code == MY_KEY_LEFT) {
        kernel.cursor->left();
    }

}