//
// Created by maxim on 17.11.2020.
//

#include "Controller.h"
#include "Test.h"


Controller::Controller(Kernel &kernel) : kernel(kernel) {
    //  Set delay for getch - 1 msec
    halfdelay(1);

    // arrow on
    keypad(stdscr, true);
}

void Controller::start() {
    //Test test(kernel);

    key_t code;
    int exit_code = 0;

    Handler* handler = nullptr;

    while (kernel.is_on()) {
        if (handler) {
            delete handler;
        }

        if (kernel.get_kernel_mode() == Kernel::KernelMode::navigation) {
            // 'i', 'I', 'A', 'S' are exit because it is start of TEXT_INPUT mode
            // ':' is exit because it is start of CMD_INPUT  mode
            // '/', '?' are exit because it is start if SEARCH mode

            handler = new NavigationHandler(kernel, {'i', 'I', 'A', 'S', ':', '/', '?'});
        }

        else if (kernel.get_kernel_mode() == Kernel::KernelMode::text_input) {
            // ESC is exit because it is start of NAVIGATION mode

            handler = new SymbolHandler(kernel, {MY_KEY_ESC});
        }

        else if (kernel.get_kernel_mode() == Kernel::KernelMode::cmd_input) {
            // ESC is exit because it is start of NAVIGATION mode

            handler = new SymbolHandler(kernel, {MY_KEY_ESC});
        }

        else if (kernel.get_kernel_mode() == Kernel::KernelMode::direct_search ||
                kernel.get_kernel_mode() == Kernel::KernelMode::reverse_search) {

            // ESC is exit because it is start of NAVIGATION mode
            handler = new SymbolHandler(kernel, {MY_KEY_ESC});
        }

        // program loop in some mode
        exit_code = handler->start_loop();

        // loop is end - change kernel mode
        change_kernel_mode(exit_code);
    }

    delete handler;
}

bool Controller::is_printable(key_t code) {
    if (code >= 'a' && code <= 'z') {
        return true;
    }
    if (code >= 'A' && code <= 'Z') {
        return true;
    }
    if (code >= '0' && code <= '9') {
        return true;
    }

    // TODO: all this list
    std::list<char> punctuation_marks = {
            '.', ',', '!', '?', '@', '\'',
            '\"', '-', '+', '=', '/', '\\',
            ')', '(', ' ', '#', '<', '>',
            '}', '{', '[', ']', '$', '%',
            '^', '&', '*', '~', '`', '|',
            '_', ':', ';', '\t'
    };

    for (auto c : punctuation_marks) {
        if (c == code) {
            return true;
        }
    }

    return false;
}

void Controller::change_kernel_mode(int exit_code) {
    // to text input mode
    if (exit_code == 'i') {
        kernel.set_kernel_mode(Kernel::KernelMode::text_input);
    }
    else if (exit_code == 'I') {
        kernel.set_kernel_mode(Kernel::KernelMode::text_input);
        kernel.cursor_runner->line_begin();
    }
    else if (exit_code == 'A') {
        kernel.set_kernel_mode(Kernel::KernelMode::text_input);
        kernel.cursor_runner->line_end();
    }
    else if (exit_code == 'S') {
        kernel.set_kernel_mode(Kernel::KernelMode::text_input);
        kernel.cursor_runner->line_begin();

        kernel.clip_board->cut_cur_line();
        kernel.clip_board->reset_buf();
    }

    // to navigation mode
    else if (exit_code == MY_KEY_ESC) {
        kernel.set_kernel_mode(Kernel::KernelMode::navigation);
    }

    // to cmd input mode
    else if (exit_code == ':') {
        kernel.set_kernel_mode(Kernel::KernelMode::cmd_input);
    }

    // to search mode
    else if (exit_code == '/') {
        kernel.set_kernel_mode(Kernel::KernelMode::direct_search);
    }
    else if (exit_code == '?') {
        kernel.set_kernel_mode(Kernel::KernelMode::reverse_search);
    }
}
