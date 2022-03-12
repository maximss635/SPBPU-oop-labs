#include "Controller.h"
#include "ArgParser.h"


int main(int argc, char** argv)
{
    // Create object which check console-args
    ArgParser argparser(argc, argv);


    // Create screen-view-object - adapter for library ncurses          (V)
    View screen;


    // Create main model - program-kernel                               (M)
    Kernel kernel(screen, argparser.get_file_path());


    // Create controller which work when keyboard presses               (C)
    Controller controller(kernel);


    // CONTROLLER  --pointer-->  KERNEL  --pointer-->   CONSOLE


    // Now file loaded in RAM -> print it to screen
    screen.text_area->update(0, screen.text_area->height());


    // start working
    controller.main_loop();

    return 0;
}
