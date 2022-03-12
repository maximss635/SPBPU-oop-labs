#include "Controller.h"

int Controller::Handler::start_loop() {
    key_t code;
    while (kernel.is_on()) {
        // generate code
        code = getch();
        Journal::log(code);

        // no keyb-pressed in timeout
        if (code == -1) {
            kernel.update_logo();
        }

        // if generated code is in list of exit codes - loop out
        for (auto& exit_code : exit_codes) {
            if (code == exit_code) {
                return code;
            }
        }

        // handle code
        handler(code);
    }

    return code;
}

void Controller::ArrowHandler::handler(key_t code) {
    if (code == KEY_RIGHT) {
        kernel.arrow_right();
    }
    else if (code == KEY_LEFT) {
        kernel.arrow_left();
    }
    else if (code == KEY_UP) {
        kernel.arrow_up();
    }
    else if (code == KEY_DOWN) {
        kernel.arrow_down();
    }

    else if (code == CTRL_RIGHT) {
        kernel.cursor_runner->word_end();
    }
    else if (code == CTRL_LEFT) {
        kernel.cursor_runner->word_begin();
    }
    else if (code == CTRL_UP) {
        kernel.lines_down();
    }
    else if (code == CTRL_DOWN) {
        kernel.lines_up();
    }
}

void Controller::SymbolHandler::handler(key_t code) {
    ArrowHandler::handler(code);

    if (code == '\n') {
        kernel.enter();
    }

    else if (code == MY_KEY_BACKSPACE) {
        kernel.backspace();
    }

    else if (code == MY_KEY_DEl) {
        kernel.del();
    }


    else if (is_printable(code)) {
        kernel.insert_char(code);
    }
}

void Controller::NavigationHandler::handler(key_t code) {
    ArrowHandler::handler(code);

    if (code == 'w') {
        kernel.cursor_runner->word_end();
    }
    else if (code == 'b') {
        kernel.cursor_runner->word_begin();
    }

    else if ((code == KEY_HOME) || (code == '^')) {
        kernel.cursor_runner->line_begin();
    }
    else if ((code == KEY_END) || (code == '$')) {
        kernel.cursor_runner->line_end();
    }

    else if (code == KEY_PPAGE) {    // page up
        kernel.cursor_runner->page_up();
    }
    else if (code == KEY_NPAGE) {    // page down
        kernel.cursor_runner->page_down();
    }

    // hot keys in navigation mode
    else if (code == 'G') {
        kernel.cursor_runner->file_end();
    }
    else if (code == 'g') {
        kernel.cursor_runner->file_begin();
    }
    else if (code == 'x') {
        kernel.del();
    }
    else if (code == 'c') {
        // copy word under cursor
        kernel.clip_board->copy_cur_word();
    }
    else if (code == 'y') {
        // copy line under cursor
        kernel.clip_board->copy_cur_line();
    }
    else if (code == 'p') {
        // paste after cursor
        kernel.clip_board->paste_after_cursor();
    }
    else if (code == 'd') {
        kernel.clip_board->cut_cur_line();
    }
    else if (code == 't') {
        kernel.clip_board->cut_cur_word();
    }

    else if (code == 'r') {
        // change 1 symbol under cursor
        key_t new_code = -1;

        /* loop need because getch() can return -1
            if there is no press */
        do {
            new_code = getch();
        } while(new_code == -1);

        kernel.change_one_symbol(new_code);
    }
}
