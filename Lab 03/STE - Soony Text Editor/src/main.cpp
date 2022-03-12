#include "Controller.h"
#include "View.h"

#include "ArgParser.h"

int main(int argc, char** argv)
{
    // Create object which check console-args
    ArgParser arg_parser(argc, argv);


    /* Adapter for library ncurses
     * if in some platform there is no library ncurses
     * we must create new adapter, inherit it from class GraphicLibAdapter
     * and create here  */
    GraphicLibAdapter* adapter = new NCursesAdapter();

    // get terminal sizes - Kernel need them in his model
    int terminal_width = adapter->get_terminal_width();
    int terminal_height =  adapter->get_terminal_height();


    // Create main model - program-kernel                               (M)
    Kernel kernel(arg_parser.get_file_path(), terminal_width, terminal_height);


    // Create screen-view-object - adapter for library ncurses          (V)
    View screen(kernel, adapter);


    // Create controller which work when keyboard presses               (C)
    Controller controller(kernel);


    // start working
    controller.start();


    delete adapter;

    return 0;
}

