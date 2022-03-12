#include <iostream>

#include "Tree.h"
#include "main.h"

int main()
{
    Menu menu;

    int c = 0;
    while (c != menu.exit) {
        c = menu.getc();
        menu.handler(c);
    }

    return 0;
}

Menu::Menu() {
    print_actions();

    Event *event;
    std::ifstream is(journal_path);

    if (!is.is_open()) {
        return;
    }

    while (!is.eof()) {
        event = new Event();

        is >> *event;

        if (event->description.empty()) {
            continue;
        }

        events_all.push_back(event);

        tree_sort_place.insert(event, event->place);
        tree_sort_date.insert(event, event->day);
        tree_sort_prio.insert(event, event->priority);
    }

    is.close();
    delete event;
}

void Menu::print_actions() {
    std::cout << "1 - Insert new event\n";
    std::cout << "2 - Remove event\n";
    std::cout << "3 - Show all events (place sort)\n";
    std::cout << "4 - Show all events (date sort)\n";
    std::cout << "5 - Show all events (priority sort)\n";
    std::cout << "6 - Remove all events\n";
    std::cout << "10 - Exit\n";
}


Menu::~Menu() {
    std::ofstream os(journal_path);

    for (auto& event : events_all) {
        os << *event;
        delete event;
    }

    os.close();
}

Event *Menu::cin_get_elem() {
    auto *elem = new Event();

    std::cout << "Place: ";
    std::cin >> elem->place;

    std::cout << "Description: ";
    std::cin >> elem->description;

    std::cout << "Day: ";
    std::cin >> elem->day;

    std::cout << "Priority: ";
    std::cin >> elem->priority;

    return elem;
}

void Menu::insert_handler() {
    Event *elem = cin_get_elem();
    events_all.push_back(elem);

    tree_sort_date.insert(elem, elem->day);
    tree_sort_prio.insert(elem, elem->priority);
    tree_sort_place.insert(elem, elem->place);
}

void Menu::remove_handler() {
    show_all_events();

    int num_in_list;
    std::cout << "Num: ";
    std::cin >> num_in_list;

    Event* elem = list_at(num_in_list);

    tree_sort_place.remove(elem->place);
    tree_sort_prio.remove(elem->priority);
    tree_sort_date.remove(elem->day);

    events_all.remove(elem);

    delete elem;
}

void Menu::show_handler(int cmd_num) {
    if (cmd_num == 3) {
        std::cout << "description | place | day | prio" << "\n";
        tree_sort_place.dfs([](auto node) {
            std::cout << node->description << " | " << node->place << " | " << node->day << " | " << node->priority << "\n";
        });
    }

    if (cmd_num == 4) {
        std::cout << "description | place | day | prio" << "\n";
        tree_sort_date.dfs([](auto node) {
            std::cout << node->description << " | " << node->place << " | " << node->day << " | " << node->priority << "\n";
        });
    }

    if (cmd_num == 5) {
        std::cout << "description | place | day | prio" << "\n";
        tree_sort_prio.dfs([](auto node) {
            std::cout << node->description << " | " << node->place << " | " << node->day << " | " << node->priority << "\n";
        });
    }
}

void Menu::handler(int cmd_num) {
    switch (cmd_num) {
        case 1:
            insert_handler();
            break;

        case 2:
            remove_handler();
            break;

        case 3:
        case 4:
        case 5:
            show_handler(cmd_num);
            break;

        case 6:
            remove_all_handler();
            break;

        default:
            return;
    }
}

int Menu::getc() {
    int c;
    std::cout << "cmd: ";
    std::cin >> c;
    return c;
}

void Menu::remove_all_handler() {
    tree_sort_prio.remove_all();
    tree_sort_date.remove_all();
    tree_sort_place.remove_all();
}

void Menu::show_all_events() {
    std::cout << "i) description | place | day | prio" << "\n";
    int i = 0;
    for (auto& event : events_all) {
        std::cout << ++i << ") ";
        std::cout << event->description << " | " << event->place << " | " << event->day << " | " << event->priority << "\n";
    }
}

Event *Menu::list_at(int num) {
    if (num > events_all.size()) {
        return nullptr;
    }

    int i = 0;
    for (auto& event: events_all) {
        if (++i == num) {
            return event;
        }
    }

    return nullptr;
}
