//
// Created by maxim on 17.11.2020.
//

#include "Kernel.h"

Kernel::Kernel(View& screen, File::Path&& path_main_file) :
    screen(screen),
    file(path_main_file)
{
    Journal::log("Kernel init");

    screen.text_area->set_file_pointer(&file.lines);

    if (!file.path.empty()) {
        // User entered file name
        try {
            file.load();
        }
        catch (std::exception& e) {
            // No such file : [path_main_file] -> create new file

            // add empty string to StringArray
            MyString empty_line;
            file.push_back(empty_line);

        }

    }
    else {
        // User dont entered file name
        file.create();

        // add empty string to StringArray
        MyString empty_line;
        file.push_back(empty_line);
    }

    screen.state_str->set_file_pointer(&file);
    screen.state_str->update(0, 0);

    cursor = new Cursor(0, 0, *this);
    screen.set_cursor(cursor->get_x(), cursor->get_y());


    Journal::log("qw");
    Journal::log(file.longpath);
    Journal::log(file.path);

}

Kernel::~Kernel() {
    Journal::log("Kernel deinit");

    file.save();

    delete cursor;

}





void Kernel::insert_enter() {

    if (cursor->get_y() >= screen.text_area->height() - 3) {
        // values ??? + encapsulation
        screen.text_area->scroll_up();
    }

    /*    THERE ARE 3 SITUATIONS:
     *
     *    1)   abcdef
     *         123123213
     *         ghijkl<-*cursor*
     *
     *    2)  abcdefg
     *        123122332
     *        hijklmno<-*cursor*
     *        123123123
     *
     *   3)  abcdefg
     *       hdfjk(*cursor*)efkldsfh
     *       dfhkdhldklfd
     */

    int size_cur_line = get_cur_line()->size();
    int lines_total = file.lines.size();

    auto num_situation {[](int size_cur_line, int lines_total, int cursor_x, int cursor_y, int y_max) -> int {
        if (cursor_x == size_cur_line) {
            if ((cursor_y == lines_total - 1) || (cursor_y == y_max)) {
                return 1;
            }
            else {
                return 2;
            }
        }
        else {
            return 3;
        }
    }};


    int num = num_situation(size_cur_line, lines_total, cursor->get_x(), cursor->get_y(), screen.text_area->height() - 3);


    if (num == 1) {

        // add empty string
        MyString empty_line;
        file.lines.push_back(empty_line);

        cursor->newline();

        // update screen
        screen.numb_area->update(cursor->get_y(), cursor->get_y());

    }

    else if (num == 2) {
        cursor->newline();

        // find place for inserting empty string
        auto iter = file.begin();
        for (int i = 0; i < cursor->get_y(); ++i) {
            ++iter;
        }

        // insert empty string
        MyString empty_line;
        iter = file.lines.insert(iter, empty_line);

        // Update screen
        screen.text_area->update(cursor->get_y() - 1, screen.text_area->height());

    }

    else if (num == 3) {
        auto iter = file.begin();
        for (int i = 0; i < cursor->get_y(); ++i) {
            ++iter;
        }

        // now iter is point to line which gona split
        MyString splited_line = *iter;

        if (cursor->get_x() != 0) {
            MyString substr_1 = splited_line;
            substr_1.erase(cursor->get_x(), substr_1.size() - cursor->get_x());
            MyString substr_2 = splited_line.substr(cursor->get_x());

            // insert two lines and remove one
            file.lines.insert(iter, substr_1);
            file.lines.insert(iter, substr_2);

            file.lines.erase(iter);
        }
        else {
            MyString empty_line;
            file.lines.insert(iter, empty_line);
        }

        --iter;


        cursor->newline();

        // update screen
        screen.text_area->update(cursor->get_y() - 1, screen.text_area->height());
    }

    // update line numbering
    MyString numb;
    to_string(numb, file.number_lines() - 1);
    screen.num_list.push_back(numb);
    screen.numb_area->update(file.number_lines() - 1, file.number_lines() - 1);

    // update cursor
    screen.set_cursor(cursor->get_x(), cursor->get_y());

}

Kernel& Kernel::insert_char(char c) {

    // add this symbol to StringArray ALL_LINES_FILE
    auto line_under_cursor = get_cur_line();
    line_under_cursor->insert(cursor->get_x(), 1, c);


    // change cursor position
    cursor->inc_x();


    // update this line in screen
    screen.text_area->update(cursor->get_y(), cursor->get_y());

    screen.set_cursor(cursor->get_x(), cursor->get_y());

    return* this;
}

void Kernel::delete_char(bool left) {
    // left == true  ->  delete char which list left of cursor (backspace)
    // left == false ->  delete char which list right of cursor (del)

    auto deleted_symbol_is_not_enter {[](bool left, int cursor_x, int line_size) -> bool {
        if (left) {
            return (cursor_x > 0);
        }
        else {
            return (cursor_x != line_size);
        }
    }};

    auto line_under_cursor = get_cur_line();
    int cur_line_size = line_under_cursor->size();

    if (deleted_symbol_is_not_enter(left, cursor->get_x(), cur_line_size)) {
        // delete symbol, not '\n' -> easy situation

        int index_of_deleted_symbol;
        if (left) {
            index_of_deleted_symbol= cursor->get_x() - 1;
        }
        else {
            index_of_deleted_symbol= cursor->get_x();
        }

        // in LINE_UNDER_CURSOR symbol .at(INDEX_OF_DELETED_SYMBOL) should be deleted
        line_under_cursor->erase(index_of_deleted_symbol, 1);

        // change cursor position
        cursor->dec_x();

        // change line in console
        screen.text_area->update(cursor->get_y(), cursor->get_y());
        screen.set_cursor(cursor->get_x(), cursor->get_y());
    }

    else {
        if (left) {
            /*    LINE_UP               ->      123              123456|
             *    LINE_UNDER_CURSOR     ->     |456      -->     789
             *                                  789
             *
             * */

            if (cursor->get_y() == 0) {
                return;
            }

            auto line_up = file.lines.begin();
            std::advance(line_up, cursor->get_y() - 1);

            int magic_size = line_up->size();

            MyString result_line = *line_up + *line_under_cursor;

            auto iter = file.lines.begin();
            for (int i = 0; i < cursor->get_y() - 1; ++i) {
                ++iter;
            }

            // Now iter refer to LINE_UP

            // remove 2 lines and insert 1
            iter = file.lines.erase(iter);
            iter = file.lines.erase(iter);
            file.lines.insert(iter, result_line);

            cursor->dec_y();

            //update screen
            screen.text_area->update(0, screen.text_area->height());


            for (int i = 0; i < magic_size; ++i) {
                cursor->inc_x();
            }

            screen.set_cursor(cursor->get_x(), cursor->get_y());
        }

        else {
            // Not implemented yet
        }
    }


}




/* CURSOR OPERATIONS */

void Kernel::Cursor::left()  {
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
        kernel.screen.text_area->scroll_down();
    }

    kernel.screen.set_cursor(get_x(), get_y());
}

void Kernel::Cursor::right() {
    int size_cur_line = kernel.get_cur_line()->size();

    if (x_absolute < size_cur_line) {
        inc_x();
    }
    else if (y_real < y_max) {
        inc_y();
        ++line_under;
        x_absolute = x_real = 0;
    }
    else if (y_absolute < kernel.file.number_lines() - 1) {
        ++y_absolute;
        ++line_under;
        x_absolute = x_real = 0;
        kernel.screen.text_area->scroll_up();
    }

    // Update screen
    kernel.screen.set_cursor(get_x(), get_y());
}

void Kernel::Cursor::up() {
    if (y_real <= 0) {
        if (y_absolute > 0) {
            kernel.screen.text_area->scroll_down();
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

    // Update screen
    kernel.screen.set_cursor(get_x(), get_y());
}

void Kernel::Cursor::down() {
    if (y_real >= y_max) {
        if (y_absolute < kernel.file.number_lines() - 1) {
            mvprintw(20,20,"*");
            kernel.screen.text_area->scroll_up();
        }

        else return;
    }
    else {
        ++y_real;
    }

    ++y_absolute;
    ++line_under;

    if (x_absolute > line_under->size()) {
        x_absolute = x_real = line_under->size();
    }

    // Update screen
    kernel.screen.set_cursor(get_x(), get_y());
}

void Kernel::Cursor::newline() {
    x_absolute = x_real = 0;
    inc_y();
    ++line_under;

    // Update screen
    kernel.screen.set_cursor(get_x(), get_y());
}

