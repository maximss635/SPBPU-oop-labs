//
// Created by maxim on 22.11.2020.
//

#include "MyStringAdditions.h"

void to_string(MyString& res, unsigned int num)
{
    if (num == 0) {
        res = "0";
        return;
    }

    while (num != 0) {
        res.insert(0, 1, '0' + num % 10);
        num /= 10;
    }

}