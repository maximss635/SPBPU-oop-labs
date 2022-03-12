//
// Created by maxim on 30.11.2020.
//

#ifndef STE_GRAPHICLIBADAPTER_H
#define STE_GRAPHICLIBADAPTER_H

#include "MyString.h"

/* Interface which must be implement under some graphic lib according to platform */

class GraphicLibAdapter {
public:
    virtual ~GraphicLibAdapter() { };
    virtual void set_cursor(int x, int y) = 0;

    virtual void print(int x, int y, int d, int color_pair_id) const = 0 ;
    virtual void print(int x, int y, char c, int color_pair_id) const = 0;
    virtual void print(int x, int y, const char* str, int color_pair_id) const = 0;
    virtual void create_color_id(short,short,short) const = 0;

    virtual void clear_line(int x, int y, int width, int color_id) const {
        MyString spaces(width, ' ');
        print(x, y, spaces.c_str(), color_id);
    }

    virtual void crate_block(int x, int y, int width, int height, char c_gorizontal, char c_vertical) = 0;

    virtual int get_terminal_height() const = 0;
    virtual int get_terminal_width() const = 0;
};

#endif //STE_GRAPHICLIBADAPTER_H
