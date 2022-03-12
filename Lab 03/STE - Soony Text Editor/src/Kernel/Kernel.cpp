//
// Created by maxim on 17.11.2020.
//

#include "Kernel.h"

Kernel::Kernel(File::Path&& path_main_file, int terminal_width, int terminal_height) :
    off(false),
    first_line_index(0),
    terminal_width(terminal_width),
    terminal_height(terminal_height)
{
    file = new File(path_main_file);

    if (file->localpath != file->sample_path) {
        // User entered file name
        try {
            file->load();
        }
        catch (std::exception& e) {
            // No such file : [path_main_file] -> create new file

            file->create();
        }

    }
    else {
        // User dont entered file name
        file->create();
    }

    cursor = new CursorModel(0, 0, *this);
    cursor_runner = new CursorRunner(cursor, *this);
    clip_board = new ClipBoard(*this, cursor);

    // screen updating
    send_update();
}

Kernel::~Kernel() {
    delete file;

    delete cursor;
    delete cursor_runner;
    delete clip_board;
}


// ********** ENTER ********************

Kernel& Kernel::enter() {
    if (this->get_kernel_mode() == KernelMode::text_input) {
        enter_text();
    }
    else if (this->get_kernel_mode() == KernelMode::cmd_input) {
        enter_cmd();
    }
    else if (this->get_kernel_mode() == KernelMode::direct_search ||
            this->get_kernel_mode() == KernelMode::reverse_search) {
        enter_search();
    }

    // screen updating
    send_update();

    return* this;
}

Kernel& Kernel::enter_text()
{
    file->is_saved = false;

    if (cursor->get_y() >= cursor->get_max_y()) {
        this->scroll_up();
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

    int size_cur_line = cursor->get_cur_line()->size();

    auto num_situation {[](int size_cur_line, int lines_total, int cursor_x, int cursor_y) -> int {
        if (cursor_x == size_cur_line) {
            if (cursor_y == lines_total - 1) {
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

    int num = num_situation(size_cur_line, file->number_lines(), cursor->get_x(), cursor->get_absolute_y());

    if (num == 1) {
        // add empty string
        MyString empty_line;
        file->lines.push_back(empty_line);

        cursor->newline(true);
    }

    else if (num == 2) {
        cursor->newline(true);

        // find place for inserting empty string
        auto iter = file->begin();
        std::advance(iter, cursor->get_absolute_y());        // y? or y_absolute ???

        // insert empty string
        MyString empty_line;
        iter = file->lines.insert(iter, empty_line);
    }

    else if (num == 3) {
        auto iter = file->begin();
        std::advance(iter, cursor->get_absolute_y());

        // now iter is point to line which gona split
        MyString splited_line = *iter;

        if (cursor->get_x() != 0) {
            MyString substr = splited_line.substr(cursor->get_x());

            file->lines.insert(++iter, substr);

            --iter;
            --iter;
            iter->erase(cursor->get_x() , iter->size() - cursor->get_x());

            cursor->newline(true);
        }
        else {
            MyString empty_line;
            file->lines.insert(iter, empty_line);
            cursor->newline(false);
        }
    }

    return* this;
}

Kernel& Kernel::enter_cmd() {
    command_handler(this->cmd);
    return* this;
}

Kernel& Kernel::enter_search() {
    if (cursor->get_absolute_y() == 0 &&
        cursor->get_absolute_x() == 0
        && get_kernel_mode() == KernelMode::reverse_search)
    {
        this->substr_for_search.clear();
        return* this;
    }

    static MyString last_substr("");
    bool is_direct = (this->get_kernel_mode() == KernelMode::direct_search);
    int x = cursor->get_absolute_x();
    int y = cursor->get_absolute_y();

    if (this->substr_for_search.empty() &&
        !last_substr.empty()) {
        // repeat last search
        if (is_direct) {
            cursor->right();
        }
        else {
            cursor->left();
        }

        x = cursor->get_absolute_x();
        y = cursor->get_absolute_y();

        this->file->search(last_substr, x, y, is_direct);
        if (x == -1 || y == -1) {
            // not find substr
            if (is_direct) {
                cursor->left();
            }
            else {
                cursor->right();
            }
            return* this;
        }

        cursor_runner->got_to(x, y);
        send_update();

        return* this;
    }

    last_substr = this->substr_for_search;

    this->file->search(this->substr_for_search, x, y, is_direct);

    this->substr_for_search.clear();

    if (x == -1 || y == -1) {
        // not find substr
        return* this;
    }

    cursor_runner->got_to(x, y);
    send_update();

    return* this;
}


// ********** INSERT CHAR ********************

Kernel& Kernel::insert_char(char c) {
    if (this->get_kernel_mode() == KernelMode::text_input) {
        insert_char_to_text(c);
    }
    else if (this->get_kernel_mode() == KernelMode::cmd_input) {
        insert_char_to_cmd(c);
    }
    else if (this->get_kernel_mode() == KernelMode::direct_search ||
             this->get_kernel_mode() == KernelMode::reverse_search) {
        insert_char_to_search(c);
    }

    // screen updating
    send_update();

    return* this;
}

Kernel &Kernel::insert_char_to_text(char c) {
    if (c == '\t') {
        // tabulation is two spaces
        return this->insert_char(' ').insert_char(' ');
    }

    file->is_saved = false;

    // add this symbol to StringArray ALL_LINES_FILE
    auto line_under_cursor = cursor->get_cur_line();
    line_under_cursor->insert(cursor->get_x(), 1, c);

    // change cursor position
    cursor->inc_x();

    return* this;
}

Kernel &Kernel::insert_char_to_cmd(char c) {
    if (cmd.size() < MAX_SYMBOLS_IN_CMD) {
        cmd.append(1, c);
    }
    return* this;
}

Kernel& Kernel::insert_char_to_search(char c) {
    if (substr_for_search.size() < MAX_SYMBOLS_IN_CMD) {
        substr_for_search.append(1, c);
    }
    return* this;
}



// ********** BACKSPACE ********************

Kernel &Kernel::backspace() {
    if (this->get_kernel_mode() == KernelMode::text_input) {
        return backspace_text();
    }
    else if (this->get_kernel_mode() == KernelMode::cmd_input) {
        return backspace_cmd();
    }
    else if (this->get_kernel_mode() == KernelMode::direct_search ||
             this->get_kernel_mode() == KernelMode::reverse_search) {
        return backspace_search();
    }

    return* this;
}

Kernel& Kernel::backspace_text() {
    file->is_saved = false;

    if (cursor->get_x() != 0) {
        remove_char(cursor->get_x() - 1);
    }
    else {
        remove_enter();
    }

    return* this;
}

Kernel &Kernel::backspace_cmd() {
    if (cmd.empty()) {
        return* this;
    }

    cmd.erase(cmd.size() - 1, 1);

    // update screen
    send_update();

    return* this;
}

Kernel& Kernel::backspace_search() {
    if (substr_for_search.empty()) {
        return* this;
    }

    substr_for_search.erase(substr_for_search.size() - 1, 1);

    // update screen
    send_update();

    return* this;
}



// ********** DELETE ********************

Kernel& Kernel::del() {
    file->is_saved = false;

    if ((this->get_kernel_mode() == KernelMode::text_input) ||
            (this->get_kernel_mode() == KernelMode::navigation)) {
        return del_text();
    }

    return* this;
}

Kernel& Kernel::del_text() {
    if (cursor->get_x() != cursor->get_cur_line()->size()) {
        remove_char(cursor->get_x());
        cursor->right();
    }
    else {
        cursor->right();
        remove_enter();
    }

    return* this;
}

// ********** REMOVE CHAR ********************

void Kernel::remove_char(int index)
{
    // in LINE_UNDER_CURSOR symbol .at(INDEX_OF_DELETED_SYMBOL) should be deleted
    cursor->get_cur_line()->erase(index, 1);

    // change cursor position
    cursor->dec_x();

    // screen updating
    send_update();
}

void Kernel::remove_enter() {
        /*    LINE_UP               ->      123              123456|
         *    LINE_UNDER_CURSOR     ->     |456      -->     789
         *                                  789
         * */

    if (cursor->get_absolute_y() == 0) {
        return;
    }

    const auto line_up = --(cursor->get_cur_line());   // line which upper than current
    int line_up_size = line_up->size();
    MyString result_line = *line_up + *cursor->get_cur_line();

    // remove 2 lines and insert 1
    auto tmp = line_up;
    file->lines.insert(tmp, result_line);
    tmp = file->lines.erase(tmp);
    file->lines.erase(tmp);

    if (cursor->get_y() != 0) {
        cursor->dec_y();
    }
    else {
        cursor->dec_absolute_y();
        this->scroll_down();
        send_update();
    }

    // we should update iterator in cursor
    cursor->update_cur_line();
    for (int i = 0; i < line_up_size; ++i) {
        cursor->inc_x();
    }

    // screen updating
    send_update();
}

void Kernel::view_resize() const {
    // TODO
}

// *******************************************


void Kernel::update_logo() const {
    // called in loop
    // freq is set in Controller::Controller()
    // TODO some

}

void Kernel::send_update() {
    // screen updating
    MyString mode_s;
    if (this->get_kernel_mode() == KernelMode::navigation) {
        mode_s = "Nav";
    }

    else if (this->get_kernel_mode() == KernelMode::text_input) {
        mode_s = "Ins";
    }

    else if (this->get_kernel_mode() == KernelMode::cmd_input) {
        mode_s = "Cmd";
    }

    else if (this->get_kernel_mode() == KernelMode::direct_search) {
        mode_s = "Search1";
    }
    else if (this->get_kernel_mode() == KernelMode::reverse_search) {
        mode_s = "Search2";
    }

    this->notify_update(
        SendData(
            cursor->get_x(),
            cursor->get_y(),
            first_line_index,
            mode_s,
            cmd,
            substr_for_search
        )
    );
}

void Kernel::reload_file(MyString& to) {
    /*
     *    file --> ******            <--
     *                                   \
     *    tmp ---> ******   //if create /
     *
     * */
    this->cursor_runner->file_begin();

    // try to reload file
    File* tmp = new File(to);

    try {
        tmp->load();
    } catch (std::exception&) {
        tmp->create();
    }

    this->first_line_index = 0;
    *file = *tmp;
    delete tmp;

    // update screen
    this->send_update();
}

void Kernel:: command_handler(MyString cmd_line) {
    this->cmd.clear();

    MyString cmd, arg;

    if (cmd_line[1] == ' ') {
        arg = cmd_line.substr(2);

        cmd.append(1, cmd_line.at(0));
    }
    else {
        cmd = cmd_line;
    }

    if (cmd == MyString("o")) {
        // open file ARGS
        this->reload_file(arg);
    }
    else if (cmd == MyString("w")) {
        // save cur file
        if (arg.empty()) {
            this->file->save();
        }
        else {
            File* tmp = new File(arg);
            *tmp = *(this->file);
            tmp->localpath = arg;
            tmp->save();
            delete tmp;
        }
    }
    else if (cmd == MyString("h")) {
        // show help
        MyString help_txt("help.txt");
        this->reload_file(help_txt);
    }
    else if ((cmd[0] >= '0') && (cmd[0] <= '9')) {
        // go to line with num
        this->cursor_runner->to_line(cmd.to_int());
    }

    // Program exit
    else if (cmd == MyString("x") || cmd == MyString("wq!")) {
        // save cur file and exit
        file->save();
        this->off = true;
    }
    else if (cmd == MyString("q!")) {
        this->off = true;
    }
    else if (cmd == MyString("q")) {
        if (!file->is_saved) {
            return;
        }

        this->off = true;
    }
}

void Kernel::change_one_symbol(int sym) {
    // this function change only 1 symbol
    // not change kernel mode
    this->del_text();
    this->insert_char_to_text(sym);
    this->send_update();
}

