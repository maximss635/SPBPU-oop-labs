//
// Created by maxim on 26.11.2020.
//

#ifndef STE_OBSERVER_H
#define STE_OBSERVER_H

#include <vector>
#include <cassert>

#include "MyString.h"

struct SendData {
    SendData(int x, int y, int first_line_index,
             MyString& kernel_mode, MyString& cur_cmd, MyString& substr_for_search) :
        x(x), y(y),
        first_line_index(first_line_index),
        kernel_mode(kernel_mode),
        cur_cmd(cur_cmd),
        substr_for_search(substr_for_search) { }

    int x, y;
    int first_line_index;
    MyString kernel_mode;
    MyString cur_cmd;
    MyString substr_for_search;

private:
    // prohibited default constructors
    SendData() { assert(false); };
};



class Observer {
public:
    virtual void update(const SendData& data) = 0;
};



class Observable {
protected:
    std::vector<Observer*> observers;

public:
    inline void add_observer(Observer *observer) {
        observers.push_back(observer);
    }

    virtual void notify_update(const SendData& data) {
        for (auto& observer : observers) {
            observer->update(data);
        }
    }
};

#endif //STE_OBSERVER_H
