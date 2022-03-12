//
// Created by maxim on 15.11.2020.
//

#ifndef EVENTS_MAIN_H
#define EVENTS_MAIN_H

#include <string>
#include <list>
#include <fstream>

class Event {
public:
    Event() = default;

    Event(const Event& other) {
        *this = other;
    }

    bool operator== (const Event& other) const {
        return (description == other.description)
               && (place == other.place) && (priority == other.priority);
    }

    Event& operator= (const Event& other) {
        place = other.place;
        description = other.description;
        day = other.day;
        priority = other.priority;

        return* this;
    }

    unsigned int day{};
    std::string place, description;
    int priority{};
};


std::ifstream& operator>> (std::ifstream& is, Event& event) {
    std::string line;
    getline(is,line);

    if (line.empty()) {
        return is;
    }

    // line: "{[description],[place],[day],[priority]}"
    int pos_1 = line.find(',');
    event.description = line.substr(1, pos_1 - 1);

    int pos_2 = line.find(',', pos_1 + 1);
    event.place = line.substr(pos_1 + 1, pos_2 - pos_1 - 1);

    int pos_3 = line.find(',', pos_2 + 1);
    event.day = std::stoi(line.substr(pos_2 + 1, pos_3 - pos_2 - 1));

    int pos_4 = line.find('}', pos_3 + 1);
    event.priority = std::stoi(line.substr(pos_3 + 1, pos_4 - pos_3 - 1));


    return is;
}

std::ofstream& operator<< (std::ofstream& is, Event& event) {
    // line: "{[description],[place],[day],[priority]}"
    is << "{" << event.description << "," << event.place << "," << event.day << "," << event.priority << "}\n";
    return is;
}


class Menu  {
public:
    Menu();
    ~Menu();

public:
    static Event* cin_get_elem();

    void insert_handler();
    void remove_handler();
    void show_handler(int cmd_num);
    void remove_all_handler();

    void handler(int cmd_num);

    int getc();

    void show_all_events();
    Event* list_at(int num);

    const int exit = 10;

private:
    //here only pointers to events to organize sorting by different keys
    Tree <Event*, int>          tree_sort_prio;
    Tree <Event*, unsigned int> tree_sort_date;
    Tree <Event*, std::string>  tree_sort_place;

    std::list <Event*>           events_all;         //events here

    static std::string journal_path;

    void print_actions();
};

std::string Menu::journal_path = "events.txt";

int main();

#endif //EVENTS_MAIN_H
