//
// Created by maxim on 17.11.2020.
//

#ifndef STE_KERNEL_H
#define STE_KERNEL_H

#include <list>

#include "View.h"
#include "File.h"

/* Model part in MVC pattern */

class Kernel {
    using StrIterator = File::StringList::iterator;

private:
    View& screen;          // pointer to view-part
    File  file;


public:
    Kernel(View& screen, File::Path&& path_main_file);
    ~Kernel();

    inline StrIterator get_cur_line() const { return cursor->get_line(); }

    inline void update_logo() const {
        screen.logo->update(0, 0);
        screen.set_cursor(cursor->get_x(), cursor->get_y());
    }

    Kernel& insert_char(char c);
    void insert_enter();
    void delete_char(bool left);



private:
    class Cursor {
        Kernel& kernel;

        int x_absolute, y_absolute;     // in memory
        int x_real, y_real;             // in screen
        const int x_max, y_max;

        StrIterator line_under;         // iterator to line which under cursor

    public:
        inline StrIterator get_line() const { return line_under; }
        inline int get_x() const { return x_real; }     // need to set cursor so real
        inline int get_y() const { return y_real; }
        inline void inc_x() { ++x_absolute, x_real = std::min(x_max, x_absolute); }
        inline void inc_y() { ++y_absolute, y_real = std::min(y_max, y_absolute); }
        inline void dec_x() { --x_absolute, --x_real; }
        inline void dec_y() { --y_absolute, --y_real; }

    public:
        Cursor(int x, int y, Kernel& kernel) :
            x_absolute(x), x_real(x), y_absolute(y), y_real(y),
            kernel(kernel),
            x_max(kernel.screen.text_area->width() - 3),
            y_max(kernel.screen.text_area->height() - 3),
            line_under(kernel.file.lines.begin()) { }

    public:
        void left();
        void right();
        void up();
        void down();

        void newline();
    };

public:
    Cursor* cursor;
};



#endif //STE_KERNEL_H
