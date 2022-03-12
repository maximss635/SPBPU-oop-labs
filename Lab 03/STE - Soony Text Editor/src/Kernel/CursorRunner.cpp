//
// Created by maxim on 02.12.2020.
//

#include "Kernel.h"

void Kernel::CursorRunner::word_end() {
    if (cursor->get_x() == cursor->get_cur_line()->size()) {
        if (cursor->get_absolute_y() == kernel.file->number_lines() - 1) {
            return;
        }
        cursor->right();
        return;
    }

    if (cursor->get_right_char() == ' ') {
        while (cursor->get_x() != cursor->get_cur_line()->size() &&
               cursor->get_right_char() == ' ') {
            cursor->right();
        }
    }

    while (cursor->get_x() != cursor->get_cur_line()->size() &&
           cursor->get_right_char() != ' ') {
        cursor->right();
    }
}

void Kernel::CursorRunner::word_begin() {
    if (cursor->get_x() == 0) {
        if (cursor->get_absolute_y() == 0) {
            return;
        }
        cursor->left();
        return;
    }

    if (cursor->get_left_char() == ' ') {
        while (cursor->get_x() != 0 &&
               cursor->get_left_char() == ' ') {
            cursor->left();
        }
    }

    while (cursor->get_x() != 0 &&
           cursor->get_left_char() != ' ') {
        cursor->left();
    }
}

void Kernel::CursorRunner::to_line(unsigned long long int line) {
    if (line > kernel.file->number_lines()) {
        return;
    }

    --line;     // cursor numbering starting with 0

    if (line == cursor->get_absolute_y()) {
        return;
    }

    if (line > cursor->get_absolute_y()) {
        while (cursor->get_absolute_y() != line) {
            cursor->down();
        }
    } else {
        while (cursor->get_absolute_y() != line) {
            cursor->up();
        }
    }
}

void Kernel::CursorRunner::line_end() {
    if (cursor->get_x() != cursor->get_cur_line()->size()) {
        while (cursor->get_x() != cursor->get_cur_line()->size()) {
            cursor->right();
        }
    }
    else {
        cursor->right();
    }
}

void Kernel::CursorRunner::line_begin() {
    if (cursor->get_x() != 0) {
        while (cursor->get_x() != 0) {
            cursor->left();
        }
    }
    else {
        cursor->left();
    }
}

void Kernel::CursorRunner::page_up() {
    int cnt = cursor->get_y() + kernel.terminal_height - 9;
    for (int i = 0; i < cnt; ++i) {
        cursor->up();
    }
}

void Kernel::CursorRunner::page_down() {
    int cnt = 2 * kernel.terminal_height - cursor->get_y() - 19;
    for (int i = 0; i < cnt; ++i) {
        cursor->down();
    }
}

void Kernel::CursorRunner::file_end() {
    to_line(kernel.file->number_lines());
}

void Kernel::CursorRunner::file_begin() {
    to_line(1);
    while (cursor->get_x() != 0) {
        cursor->left();
    }
}

void Kernel::CursorRunner::got_to(int x, int y) {
    this->to_line(y + 1);

    if (x > this->cursor->get_x()) {
        while (x != this->cursor->get_x()) {
            this->cursor->right();
        }
    }
    else {
        while (x != this->cursor->get_x()) {
            this->cursor->left();
        }
    }
}
