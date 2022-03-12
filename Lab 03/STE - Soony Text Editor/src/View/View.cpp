//
// Created by maxim on 15.11.2020.
//
#include "View.h"


View::View(Kernel& kernel, GraphicLibAdapter* adapter) : adapter(adapter) {

    // adding screen-blocks

    status_bar = new BlockStatusBar(adapter, 0, 0,
                                    adapter->get_terminal_width(), 5,
                                    COLOR_WHITE, COLOR_BLACK);


    num_area = new BlockNumbering(adapter, 0, 5,
                                  adapter->get_terminal_width(),
                                  adapter->get_terminal_height() - 7,
                                  COLOR_RED, COLOR_BLACK);


    text_area = new BlockText(adapter, 5, 5,
                              adapter->get_terminal_width() - 5,
                              adapter->get_terminal_height() - 7,
                              COLOR_WHITE, COLOR_BLACK);


    mode_cmd = new BlockModeCmd(adapter, 0, adapter->get_terminal_height() - 3,
                                adapter->get_terminal_width(), 3,
                                COLOR_CYAN, COLOR_BLACK);

    add_observer(num_area);
    add_observer(status_bar);
    add_observer(mode_cmd);
    add_observer(text_area);

    text_area->set_source(kernel.get_file_pointer());
    num_area->set_source(kernel.get_file_pointer());
    status_bar->set_source(kernel.get_file_pointer());

    kernel.add_observer(this);
    kernel.send_update();
}

View::~View() {
    delete text_area;
    delete num_area;
    delete status_bar;
    delete mode_cmd;
}

