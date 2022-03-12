//
// Created by maxim on 15.11.2020.
//

#ifndef STE_NCURSESADAPTER_H
#define STE_NCURSESADAPTER_H

#include <ncurses.h>
#include <fstream>
#include <list>

#include "Journal.h"
#include "File.h"

/*
 *      View part in MVC pattern
 *      Adapter for ncurses library
 */


class View {

public:
    View();
    ~View();

    inline void set_cursor(int x, int y) const { text_area->set_cursor(x, y); }

private:
    /* INTERFACE */
    class Block {
    protected:
        const int height_, width_, x, y;

    private:
        const short text_color, background_color;
        WINDOW *window;

    public:
        Block(int height, int width, int x, int y, short text_color, short background_color, char c1, char c2);
        ~Block();

        virtual void update(int line_start, int line_end) = 0;

        inline int height() const { return height_; }
        inline int width()  const { return width_; }

        inline void print(int line_num, const MyString& str) { mvprintw(1 + y + line_num, x + 1, str.c_str()); }
        inline void print(int line_num, MyString&& str)      { mvprintw(1 + y + line_num, x + 1, str.c_str()); }
        inline void clearline(int n) { print(n, MyString(width() - 2, ' ')); }

    };


    /* REALISATIONS */

    class BlockText : public Block {
    private:
        File::StringListPointer lines_pointer_;    // pointer to loaded file
        int first_line_index;

    public:
        BlockText(int height, int width, int x, int y, char c1, char c2) :
            Block(height, width, x, y, COLOR_RED, COLOR_BLACK, c1, c2),
            first_line_index(0),
            lines_pointer_(nullptr) { }

        void update(int line_start, int line_end) override;

        inline void set_file_pointer(File::StringListPointer lines_pointer) { this->lines_pointer_ = lines_pointer; }
        inline void set_cursor(int x, int y) { move(y + this->y + 1, x + this->x + 1); }

        void scroll_up() {
            first_line_index++;
            update(0, height() - 3);
        }

        void scroll_down() {
            first_line_index--;
            update(0, height() - 3);
        }

    };

    class BlockLogo : public Block {
    public:
        BlockLogo(int height, int width, int x, int y) :
                Block(height, width, x, y, COLOR_RED, COLOR_BLACK, '|', '-') { }

        void update (int line_start, int line_end) override;
    };

    class BlockState : public Block {
    public:
        BlockState(int height, int width, int x, int y) :
                Block(height, width, x, y, COLOR_RED, COLOR_BLACK, '|', '-') { }

        void update (int line_start, int line_end) override;

        inline void set_file_pointer(File* file_pointer) { this->file_pointer_ = file_pointer; }

    private:
        File* file_pointer_;
    };

public:
    BlockText*     text_area;
    BlockText*     numb_area;
    BlockLogo*     logo;
    BlockState*    state_str;


    File::StringList num_list;  // for numb_area
};




#endif //STE_NCURSESADAPTER_H
