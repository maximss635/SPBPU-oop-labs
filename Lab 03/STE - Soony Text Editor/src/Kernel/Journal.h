//
// Created by maxim on 15.11.2020.
//

#ifndef STE_DEBUGGER_H
#define STE_DEBUGGER_H

#include <fstream>
#include <ctime>

#include "MyString.h"

/* This class write some actions to journal  with methods LOG      */

class Journal {
public:
    /*
    *   write message MSG to file with path LOG_FILE_PATH
    *   so we will have file with logs
    */

    inline static MyString get_cur_time() {
        time_t seconds = time(nullptr);
        return MyString(asctime(localtime(&seconds)));
    }

    static void log(const char *msg) {
        // Get current date and time
        MyString cur_date_and_time = get_cur_time();

        // last symbol is '\n', we do not need it
        cur_date_and_time.erase(cur_date_and_time.size() - 1, 1);

        // writing new note in journal
        std::ofstream file;
        file.open("journal.txt", std::ios::app);
        file << cur_date_and_time << " | " << msg << "\n";
        file.close();
    }

    inline static void log(MyString && msg) { log(msg.c_str()); }
    inline static void log(const MyString & msg) { log(msg.c_str()); }
    inline static void log(unsigned int num) { log(MyString(num)); }
};


#endif //STE_DEBUGGER_H
