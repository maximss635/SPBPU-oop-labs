//
// Created by maxim on 15.11.2020.
//

#ifndef STE_ARGPARSER_H
#define STE_ARGPARSER_H

#include <iostream>

#include "Journal.h"

class ArgParser {
    int         argc;
    char**      argv;

    MyString    file_path;

public:
    ArgParser(int argc, char** argv) : argc(argc), argv(argv) {
        if (!args_correct()) {
            std::cerr << "Incorrect arguments, see help\n";
            exit(1);
        }

        if (is_help()) {
            std::cout << "Using: \n";
            std::cout << "\tor\t./STE\n";
            std::cout << "\tor\t./STE [path_to_file]\n";

            exit(2);
        }

        if (argc == 2) {
            file_path = argv[1];
        }
    }

private:
    inline bool args_correct() const {
        return ((argc == 1) || (argc == 2));
    }

    bool is_help() {
        if (argc != 2) {
            return false;
        }

        return (MyString(argv[1]) == MyString("help")) || \
            (MyString(argv[1]) == MyString("--help"));
    }

public:
    inline MyString get_file_path() const { return file_path; }
};


#endif //STE_ARGPARSER_H
