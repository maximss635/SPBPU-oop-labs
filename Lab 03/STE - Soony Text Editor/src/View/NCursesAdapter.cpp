//
// Created by maxim on 02.12.2020.
//

#include "NCursesAdapter.h"

NCursesAdapter::NCursesAdapter() {
    initscr();
    noecho();
    refresh();

    // scan terminal sizes
    getmaxyx(stdscr, terminal_height, terminal_width);

    if (terminal_height == -1) {
        terminal_height = 15;
        terminal_width = 60;
        // FOR DEBUG
    }

    // color init
    start_color();
}

NCursesAdapter::~NCursesAdapter() {
    for (auto& window : windows) {
        delwin(window);
    }

    endwin();
}

void NCursesAdapter::crate_block(int x, int y, int width, int height, char c_gorizontal, char c_vertical) {
    WINDOW* new_window = newwin(height, width, y, x);
    box(new_window, c_vertical, c_gorizontal);
    wrefresh(new_window);
    refresh();

    // Add all creating windows to vector
    // in destructor all oof them will be deleting
    windows.push_back(new_window);
}

void NCursesAdapter::set_cursor(int x, int y) {
    move(y, x);
}

void NCursesAdapter::print(int x, int y, const char *str, int color_pair_id) const {
    attron(COLOR_PAIR(color_pair_id));
    mvprintw(y, x, "%s", str);
    attroff(COLOR_PAIR(color_pair_id));
}

void NCursesAdapter::print(int line, const char *str, int color_pair_id) const {
    print(0, line, str, color_pair_id);
}

void NCursesAdapter::print(int x, int y, int d, int color_pair_id) const {
    attron(COLOR_PAIR(color_pair_id));
    mvprintw(y, x, "%d", d);
    attroff(COLOR_PAIR(color_pair_id));
}

void NCursesAdapter::print(int line, int d, int color_pair_id) const {
    print(0, line, d, color_pair_id);
}

void NCursesAdapter::print(int x, int y, char c, int color_pair_id) const {
    attron(COLOR_PAIR(color_pair_id));
    mvprintw(y, x, "%c", c);
    attroff(COLOR_PAIR(color_pair_id));
}

void NCursesAdapter::create_color_id(short id, short text_color, short background_color) const {
    init_pair(id, text_color, background_color);
}

int NCursesAdapter::get_terminal_height() const {
    return this->terminal_height;
}

int NCursesAdapter::get_terminal_width() const {
    return this->terminal_width;
}

