#include "View.h"

void View::BlockText::update(const SendData& data) {
    if (source == nullptr) {
        return;
    }

    MyString spaces(width() - 2, ' ');      // for clearing

    /*  Printing lines which are in the screen
     *   in var FIRST_LINE_INDEX stored index of the
     *   first line which must be in 1st line in screen
     */

    auto iter = source->begin();                        // first file-line
    std::advance(iter, data.first_line_index);       // line which should be first in screen

    // Draw lines
    int i;
    for (i = 0; (i < height() - 2) && (iter != source->end()); ++i, ++iter) {
        this->print(i, spaces);
        this->print(i, *iter);
    }

    // If file is all but in screen there is place - clear this place
    for (i; i < height() - 2; ++i) {
        this->print(i, spaces);
    }

    set_cursor(data.x, data.y);
}

void View::BlockNumbering::update(const SendData& data) {
    if (source == nullptr) {
        return;
    }

    MyString spaces("   ");
    int i;
    for (i = 0; (i + data.first_line_index < source->number_lines()) && (i < height() - 2); ++i) {
        this->print(i, spaces);
        this->print(i, i + data.first_line_index + 1);
    }

    // If file is all but in screen there is place - clear this place
    for (i; i < height() - 2; ++i) {
        this->print(i, spaces);
    }
}

void View::BlockStatusBar::update(const SendData& data) {
    if (source == nullptr) {
        return;
    }

    adapter->clear_line(this->x, 1 + this->y,width(), color_id);
    adapter->clear_line(this->x, 2 + this->y,width(), color_id);

    //clear_line(3);

    MyString output("FullPath: %s");
    int t = width() - source->longpath.size() - 23;
    if (t > 0) {
        output.append(t, ' ');
    }

    output.append(source->is_saved ? "    [saved]" : "[not saved]");

    // ????????????? adapter!!!!
    mvprintw(1 + y, 1 + x, output.c_str(), source->longpath.c_str());
    mvprintw(2 + y, 1 + x, "FileName: %s", source->name.c_str());
    mvprintw(3 + y, 1 + x, "Position: %d : %d/%d     ", data.x, data.y + data.first_line_index + 1, source->number_lines());
}


void View::BlockModeCmd::update(const SendData& data) {
    /*
     *      [Mode]          [cmd]
     *           |<------------>|  <- this area exist only if KERNEL_MODE == CMD_INPUT
     *
     * */


    adapter->clear_line(this->x, this->y + 1,width(), color_id);

    this->print(0, data.kernel_mode);

    if (data.kernel_mode == MyString("Cmd")) {
        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 2, this->y + 1,
                       ':', color_id);

        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 1, this->y + 1,
            data.cur_cmd.c_str(), color_id);

        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 1 + data.cur_cmd.size(),
           this->y + 1, ' ', color_id);
    }
    else if (data.kernel_mode == MyString("Search1") ||
            data.kernel_mode == MyString("Search2")) {
        char c = (data.kernel_mode == MyString("Search1") ? '/' : '?');

        adapter->print(this->x + 7, this->y + 1, ' ', color_id);

        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 2, this->y + 1,
                       c, color_id);

        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 1, this->y + 1,
                       data.substr_for_search.c_str(), color_id);

        adapter->print(this->x + this->width() \
            - Kernel::MAX_SYMBOLS_IN_CMD - 1 + data.substr_for_search.size(),
                       this->y + 1, ' ', color_id);
    }
}

