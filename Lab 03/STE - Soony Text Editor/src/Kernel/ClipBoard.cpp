#include "Kernel.h"

/* Simple functions */

void Kernel::ClipBoard::copy(MyString &str) {
    reset_buf();
    for (char c : str) {
        buffer.push_back(c);
    }
}

void Kernel::ClipBoard::copy(MyString &&str) {
    reset_buf();
    for (char c : str) {
        buffer.push_back(c);
    }
}

MyString Kernel::ClipBoard::paste() const {
    MyString result;
    for (char c : buffer) {
        result.append(1, c);
    }

    return result;
}

void Kernel::ClipBoard::reset_buf() {
    buffer.clear();
}


/* Hard functions */

void Kernel::ClipBoard::copy_cur_line() {
    MyString cur_line = *cursor->get_cur_line();
    this->copy(cur_line);
}

void Kernel::ClipBoard::copy_cur_word() {
    if (cursor->get_x() == kernel.cursor->get_cur_line()->size()) {
        return;
    }

    this->copy(
            cursor->get_cur_word()
    );
}


void Kernel::ClipBoard::cut_cur_line() {
    // copy to clipboard
    this->copy_cur_line();

    // delete all symbols which right of cursor (del)
    while (cursor->get_right_char() != '\0') {
        kernel.del();
    }

    // delete all symbols which left of cursor (backspace)
    while (cursor->get_left_char() != '\0') {
        kernel.backspace();
    }
}

void Kernel::ClipBoard::cut_cur_word() {
    // copy to clipboard
    this->copy_cur_word();

    // delete all symbols in word which right of cursor (del)
    // delete all symbols in word which right of cursor (del)
    while ((cursor->get_right_char() != '\0') &&
            (cursor->get_right_char() != ' ')) {
        kernel.del();
    }

    // and spaces
    while ((cursor->get_right_char() != '\0') &&
           (cursor->get_right_char() == ' ')) {
        kernel.del();
    }

    // delete all symbols which left of cursor (backspace)
    while ((cursor->get_left_char() != '\0') &&
            (cursor->get_left_char() != ' ')) {
        kernel.backspace();
    }
}


void Kernel::ClipBoard::paste_after_cursor() {
    MyString str_paste = this->paste();
    for (char c : str_paste) {
        kernel.insert_char_to_text(c);
    }

    kernel.send_update();
}

