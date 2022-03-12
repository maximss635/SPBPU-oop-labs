//
// Created by maxim on 17.11.2020.
//

#ifndef STE_KERNEL_H
#define STE_KERNEL_H

#include <list>

#include "Observer.h"
#include "File.h"


/* Model part in MVC pattern */

class Kernel : public Observable {
    using StrIterator = File::StringList::iterator;

public:
    Kernel(File::Path&& path_main_file, int terminal_width, int terminal_height);
    ~Kernel();

private:
    // global on-off flag
    bool off;
public:
    inline bool is_on() const {
        return !off;
    }

    enum class KernelMode {
        cmd_input, text_input, navigation,
        direct_search, reverse_search
    };

    inline KernelMode get_kernel_mode() const {
        return this->mode;
    }

    inline void set_kernel_mode(KernelMode kernel_mode) {
        this->mode = kernel_mode;
        send_update();
    }

private:
    KernelMode mode = KernelMode::navigation;

    // loaded file
    File* file;

    /* index in std::list<MyString> FILE.LINES
     *  of line which is first in screen*/
    int first_line_index;

    // terminal size
    int terminal_width, terminal_height;

    // current cmd which input in CMD_INPUT mode
    MyString cmd;
    void command_handler(MyString cmd_line);

    // for search mode
    MyString substr_for_search;

public:
    static const int MAX_SYMBOLS_IN_CMD = 50;

private:
    class CursorModel;
    class CursorRunner;
    class ClipBoard;

    CursorModel* cursor;

public:
    CursorRunner* cursor_runner;
    ClipBoard* clip_board;

    void send_update();

public:
    // I don't like this method and think it is unsafely
    inline File* get_file_pointer() {
        return file;
    }

    void update_logo() const;

public:
    // Adding new symbol
    Kernel& insert_char(char c);
    Kernel& enter();
private:
    Kernel& insert_char_to_text(char c);
    Kernel& insert_char_to_cmd(char c);
    Kernel& insert_char_to_search(char c);

    Kernel& enter_text();
    Kernel& enter_cmd();
    Kernel& enter_search();

public:
    // Removing symbol
    Kernel& backspace();
    Kernel& del();


private:
    Kernel& backspace_text();
    Kernel& backspace_cmd();
    Kernel& backspace_search();
    Kernel& del_text();

public:
    void view_resize() const;

private:
    void remove_char(int index);
    void remove_enter();

    inline void scroll_up() {
        ++first_line_index;
    }

    inline void scroll_down() {
        --first_line_index;
    }

public:
    /*   ARROW HANDLERS  */
    inline Kernel& arrow_up()    {
        cursor->up();
        return* this;
    }

    inline Kernel& arrow_down() {
        cursor->down();
        return* this;
    }

    inline Kernel& arrow_left() {
        cursor->left();
        return* this;
    }

    inline Kernel& arrow_right() {
        cursor->right();
        return* this;
    }



private:
    class CursorModel {
    public:
        CursorModel(int x, int y, Kernel& kernel) :
                x_absolute(x), x_real(x), y_absolute(y), y_real(y),
                kernel(kernel),
                x_max(kernel.terminal_width - 3),
                y_max(kernel.terminal_height - 10),
                line_under(kernel.file->lines.begin()) { }

    private:
        Kernel& kernel;
        const int x_max, y_max;
        int x_absolute, y_absolute;     // in memory
        int x_real, y_real;             // in screen
        StrIterator line_under;         // iterator to line which under cursor

    public:
        StrIterator get_cur_line() const;
        MyString get_cur_word() const;

        char get_left_char() const;
        char get_right_char() const;


    public:
        void left();
        void right();
        void up();
        void down();
        void newline(bool change_line);

    public:
        void update_cur_line() {
            line_under = kernel.file->begin();
            std::advance(line_under, get_absolute_y());
        };

        inline int get_x() const {
            // need to set cursor so real
            return x_real;
        }

        inline int get_y() const {
            return y_real;
        }

        inline int get_max_y() const {
            return y_max;
        }

        inline int get_absolute_y() const {
            return y_absolute;
        }

        inline int get_absolute_x() const {
            return x_absolute;
        }

        inline void inc_x() {
            ++x_absolute;
            ++x_real;
            x_real = std::min(x_max, x_real);
        }

        inline void inc_y() {
            ++y_absolute;
            ++y_real;
            y_real = std::min(y_max, y_real);
        }

        inline void dec_x() {
            --x_absolute, --x_real;
        }

        inline void dec_y() {
            --y_absolute, --y_real;
        }


        inline void inc_real_y() {
            ++y_real;
        }

        inline void dec_real_y() {
            --y_real;
        }

        inline void inc_absolute_y() {
            ++y_absolute;
        }

        inline void dec_absolute_y() {
            --y_absolute;
        }
    };

    class CursorRunner {
    public:
        CursorRunner(Kernel::CursorModel* cursor, Kernel& kernel) : cursor(cursor), kernel(kernel) { }
        void line_begin();
        void line_end();
        void word_end();
        void word_begin();

        void to_line(unsigned long long int line);
        void got_to(int x, int y);

        void page_up();
        void page_down();
        void file_end();
        void file_begin();

    private:
        Kernel::CursorModel* cursor;
        Kernel& kernel;
    };

    class ClipBoard {
    public:
        explicit ClipBoard(Kernel& kernel, CursorModel* cursor)
            : kernel(kernel), cursor(cursor) { }

        ~ClipBoard() = default;

        /* Hard functions */
        void copy_cur_word();
        void copy_cur_line();
        void cut_cur_line();
        void cut_cur_word();
        void paste_after_cursor();

        void reset_buf();

    private:
        /* Simple functions */
        void copy(MyString&& str);
        void copy(MyString& str);
        MyString paste() const;

    private:
        std::vector<char> buffer;

        Kernel& kernel;
        CursorModel* cursor;
    };

private:
    void reload_file(MyString& to);

public:
    void change_one_symbol(int sym);

public:
    // scroll but without cursor
    void lines_down() {
        if (first_line_index > 0) {
            scroll_down();
            if (cursor->get_y() != cursor->get_max_y()) {
                cursor->inc_real_y();
            }
            else {
                cursor->dec_absolute_y();
            }
            send_update();
        }
    }

    void lines_up() {
        if (first_line_index < file->number_lines() - 1) {
            scroll_up();
            if (cursor->get_y() != 0) {
                cursor->dec_real_y();
            }
            else {
                cursor->inc_absolute_y();
            }
            send_update();
        }
    }
};


#endif //STE_KERNEL_H
