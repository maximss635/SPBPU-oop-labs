//
// Created by maxim on 26.11.2020.
//

#ifndef STE_TEST_H
#define STE_TEST_H

#include "Kernel.h"

class Test {
public:
    explicit Test(Kernel& kernel) {
        kernel.arrow_down().arrow_down().arrow_down().arrow_down().arrow_down();
        kernel.lines_up();
    }
};

#endif //STE_TEST_H
