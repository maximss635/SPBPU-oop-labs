//
// Created by maxim on 21.11.2020.
//

#ifndef STE_FILE_H
#define STE_FILE_H

#include <istream>
#include <list>
#include <cstdlib>

#include "MyStringAdditions.h"
#include "Journal.h"

class File {
public:
    using Path              = MyString;
    using StringList        = std::list<MyString>;
    using StringListPointer = StringList *;

    StringList  lines;
    Path        longpath;
    bool        is_saved;
    Path        path;

private:
    const Path sample_path = Path("__SAMPLE__.txt");



public:
    explicit File(MyString& path);

    void load();                 // load file from disk to RAM-memory (StringArray ALL_LINES_FILE)
    void save();                 // save file from RAM-memory (StringArray ALL_LINES_FILE) to disk
    void create();               // create empty file fith PATH

    inline int number_lines() const { return lines.size(); }

    inline StringList::iterator begin() { return lines.begin(); }

    inline void push_back(MyString& line) { lines.push_back(line); }
};


#endif //STE_FILE_H
