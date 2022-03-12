//
// Created by maxim on 21.11.2020.
//

#ifndef STE_FILE_H
#define STE_FILE_H

#include <istream>
#include <list>
#include <cstdlib>

#include "Journal.h"

class File {
public:
    using Path              = MyString;
    using StringList        = std::list<MyString>;

    StringList  lines;
    MyString    name;
    Path        longpath, localpath;
    bool        is_saved;

    const Path sample_path = Path("__SAMPLE__.txt");

public:
    explicit File(MyString& path);

    void load();                 // load file from disk to RAM-memory (StringArray LINES)
    void save();                 // save file from RAM-memory (StringArray LINES) to disk
    void create();               // create empty file fith PATH


    // Search SUBSTR int LINES and put result int X, Y
    void search(const MyString& substr, int& x, int& y, bool is_direct);
private:
    void direct_search(const MyString& substr, int& x, int& y);
    void reverse_search(const MyString& substr, int& x, int& y);

public:
    inline int number_lines() const {
        return lines.size();
    }

    inline StringList::iterator begin() {
        return lines.begin();
    }

    inline StringList::iterator end() {
        return lines.end();
    }


    inline void push_back(MyString& line) {
        lines.push_back(line);
    }

    File& operator= (const File& other);
};


#endif //STE_FILE_H

