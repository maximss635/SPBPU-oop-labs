//
// Created by maxim on 17.11.2020.
//

#ifndef STE_CONTROLLER_H
#define STE_CONTROLLER_H

#include "Kernel.h"
#include "NCursesAdapter.h"

class Controller {
    using key_t = int;
    using key_list = std::initializer_list<key_t>;

    Kernel& kernel;

    class Handler {
    protected:
        Kernel& kernel;
        key_list exit_codes;

    public:
        Handler(Kernel& kernel, key_list exit_codes) : kernel(kernel), exit_codes(exit_codes) { }
        virtual ~Handler() = default;

        virtual void handler(key_t code) = 0;
        int start_loop();
    };

    class ArrowHandler : public Handler {
    public:
        ArrowHandler(Kernel& kernel, key_list exit_codes) : Handler(kernel, exit_codes) { }
        virtual void handler(key_t code);
    };

    class SymbolHandler : public ArrowHandler {
    public:
        SymbolHandler(Kernel& kernel, key_list exit_codes) : ArrowHandler(kernel, exit_codes) { }
        virtual void handler(key_t code);
    };

    class NavigationHandler : public ArrowHandler {
    public:
        NavigationHandler(Kernel& kernel, key_list exit_codes) : ArrowHandler(kernel, exit_codes) { }
        virtual void handler(key_t code);
    };


public:
    explicit Controller(Kernel& kernel);
    ~Controller() = default;

    void start();

private:

    const static key_t MY_KEY_EXIT      = 0x71;
    const static key_t MY_KEY_BACKSPACE = 0x7f;
    const static key_t MY_KEY_ESC       = 0x1b;
    const static key_t MY_KEY_DEl       = 0x014a;

    const static key_t CTRL_RIGHT = 0x0231;
    const static key_t CTRL_LEFT  = 0x0222;
    const static key_t CTRL_UP    = 0x0237;
    const static key_t CTRL_DOWN  = 0x020e;

    const static key_t TERMINAL_RESIZED = 0xffffff9a;

    void change_kernel_mode(int exit_code);

    static bool is_printable(key_t code);
};

#endif //STE_CONTROLLER_H
