//
// Created by maxim on 30.11.2020.
//

#ifndef STE_NCURSESADAPTER_H
#define STE_NCURSESADAPTER_H

#include <iostream>
#include <vector>
#include <ncurses.h>

#include "GraphicLibAdapter.h"

class NCursesAdapter : public GraphicLibAdapter {
public:
    NCursesAdapter();
    ~NCursesAdapter();

    void crate_block(int x, int y, int width, int height, char c_gorizontal, char c_vertical) override;
    void set_cursor(int x, int y) override;

    void print(int x, int y, const char* str, int color_pair_id) const override;
    void print(int line, const char* str, int color_pair_id) const;
    void print(int x, int y, int d, int color_pair_id) const override;
    void print(int line, int d, int color_pair_id) const;
    void print(int x, int y, char c, int color_pair_id) const override;

    void create_color_id(short id, short text_color, short background_color) const override;

    int get_terminal_height() const override;
    int get_terminal_width() const override;

private:
    /* In this vector will be saved all pointers
     * to windows, creating in method create_block().
     * In destructor we should delete all this windows*/
    std::vector<WINDOW*> windows;

    int terminal_height, terminal_width;

};


#endif //STE_NCURSESADAPTER_H
