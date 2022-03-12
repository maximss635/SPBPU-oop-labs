//
// Created by maxim on 02.12.2020.
//

#include "Kernel.h"

/* CURSOR OPERATIONS */


/* Atomic cursor moves */

void Kernel::CursorModel::left()  {
    if (x_absolute > 0) {
        dec_x();
    }
    else if (y_real > 0) {
        dec_y();
        --line_under;
        x_real = x_absolute = line_under->size();
    }
    else if (y_absolute > 0) {
        --y_absolute;
        --line_under;
        x_real = x_absolute = line_under->size();
        kernel.scroll_down();
    }

    kernel.send_update();
}

void Kernel::CursorModel::right() {
    int size_cur_line = get_cur_line()->size();

    if (x_absolute < size_cur_line) {
        inc_x();
    }
    else if (y_real < y_max) {
        if (y_absolute == kernel.file->number_lines() - 1) {
            return;
        }
        inc_y();
        ++line_under;
        x_absolute = x_real = 0;
    }
    else if (y_absolute < kernel.file->number_lines() - 1) {
        ++y_absolute;
        ++line_under;
        x_absolute = x_real = 0;
        kernel.scroll_up();
    }

    kernel.send_update();
}

void Kernel::CursorModel::up() {
    if (y_real <= 0) {
        if (y_absolute > 0) {
            kernel.scroll_down();
        }

        else return;
    }
    else {
        --y_real;
    }

    --y_absolute;
    --line_under;

    if (x_absolute > line_under->size()) {
        x_real = x_absolute = line_under->size();
    }

    kernel.send_update();
}

void Kernel::CursorModel::down() {
    if (y_real >= y_max) {
        if (y_absolute < kernel.file->number_lines() - 1) {
            kernel.scroll_up();
        }

        else return;
    }
    else if (y_absolute == kernel.file->number_lines() - 1) {
        return;
    }
    else if (y_real == kernel.file->number_lines() - 1) {
        return;
    }
    else {
        ++y_real;
    }

    ++y_absolute;
    ++line_under;

    if (x_absolute > line_under->size()) {
        x_absolute = x_real = line_under->size();
    }

    kernel.send_update();
}

void Kernel::CursorModel::newline(bool change_line) {
    x_absolute = x_real = 0;
    inc_y();

    if (change_line) {
        ++line_under;
    }

    kernel.send_update();
}


/* Getters */

Kernel::StrIterator Kernel::CursorModel::get_cur_line() const {
    return line_under;
}

MyString Kernel::CursorModel::get_cur_word() const {
    /*  Example:
     *     CUR_WORD_LEFT_BORDER -> |         | <- CUR_WORD_RIGHT_BORDER
     *                   one two   so|me_word     four  five six
     *                               | <- cursor
     *
     *    method should return "some_word"
     */

    MyString cur_line = *this->get_cur_line();

    int cur_word_left_border = this->get_x();
    while ((cur_word_left_border != 0) &&
        (cur_line.at(cur_word_left_border - 1) != ' ')) {
        --cur_word_left_border;
    }

    int cur_word_right_border = this->get_x();
    while ((cur_word_right_border != cur_line.size() - 1) &&
           (cur_line.at(cur_word_right_border + 1) != ' ')) {
        ++cur_word_right_border;
    }

    int cur_word_len = cur_word_right_border - cur_word_left_border + 1;

    if (cur_word_left_border + cur_word_len < cur_line.size()) {
        return cur_line.substr(cur_word_left_border, cur_word_len);
    }
    else {
        return cur_line.substr(cur_word_left_border);
    }
}

char Kernel::CursorModel::get_left_char() const {
    if (this->get_x() == 0) {
        return '\0';
    }

    return this->get_cur_line()->at(this->get_x() - 1);
}

char Kernel::CursorModel::get_right_char() const {
    if (this->get_x() == this->get_cur_line()->size()) {
        return '\0';
    }

    return this->get_cur_line()->at(this->get_x());
}

