//
// Created by maxim on 21.11.2020.
//

#include "File.h"
#include "Journal.h"
#include <functional>

File::File(MyString &path) : localpath(path), is_saved(false) {
    if (path.empty()) {
        this->localpath = sample_path;
    }

    char* tmp = new char[128];

    realpath(this->localpath.c_str(), tmp);
    longpath = tmp;

    delete [] tmp;


    // "../B/AA/cdcd/AAA.txt"   --->  "AAA.txt"
    int i = 0;
    auto it = localpath.rbegin();
    while ((i < localpath.size()) && (*it != '/')) {
        ++i, ++it;
    }

    if (i == localpath.size()) {
        name = localpath;
        return;
    }

    i = localpath.size() - i;
    this->name = this->localpath.substr(i);

}

// load file from disk to RAM-memory (StringArray ALL_LINES_FILE)
void File::load() {
    Journal::log(MyString("Opening file: ") + this->localpath);

    lines.clear();
    is_saved = true;

    std::ifstream file_input(localpath.c_str());
    if (!file_input) {
        // No such file
        // May be without exceptions ?
        throw std::exception();
    }

    MyString line;
    while (true) {
        // read and save all lines

        file_input >> line;
        if (file_input.eof()) {
            break;
        }
        lines.push_back(line);
    }
    file_input.close();

    if (lines.empty()) {
        MyString empty;
        lines.push_back(empty);
    }
}


// save file from RAM-memory (StringArray ALL_LINES_FILE) to disk
void File::save() {
    Journal::log(MyString("Saving file: ") + this->localpath);

    is_saved = true;
    std::ofstream file_output(localpath.c_str());

    // write all lines from StringArray to file
    for (MyString& line : lines) {
        file_output << line << "\n";
    }

    file_output.close();
    is_saved = true;
}

void File::create() {
    // add empty string to StringArray
    MyString empty_line;
    lines.push_back(empty_line);

    std::ofstream file(localpath.c_str());
    file.close();
}

File &File::operator=(const File &other) {
    this->localpath = other.localpath;
    this->longpath = other.longpath;
    this->is_saved = other.is_saved;
    this->name = other.name;
    this->lines = other.lines;

    return* this;
}

void File::search(const MyString& substr, int& x, int& y, bool is_direct = true) {
    if (substr.empty()) {
        x = -1;
        y = -1;
        return;
    }

    if (is_direct) {
        direct_search(substr,x ,y);
    }
    else {
        reverse_search(substr,x ,y);
    }
}

void File::direct_search(const MyString& substr, int& x, int& y) {
    // get line when search should start
    auto iter = this->begin();
    std::advance(iter, y);

    // search in this line, starting from X-position

    /* not change!!! here must be copy-constructor
     * because of awful libMtString */
    int pos;
    MyString line = *iter;

    if (line.size() >= substr.size()) {
        pos = line.find(substr.c_str(), x);
        if (pos != static_cast<unsigned short>(-1)) {
            x = pos;
            return;
        }
    }

    // search in this line, starting from 0-position
    ++iter;
    for (int num_line = y + 1; iter != this->end(); ++num_line, ++iter) {
        /* not change!!! here must be copying
         * because of awful libMtString */
        line = *iter;

        if (substr.size() > line.size()) {
            continue;
        }

        pos = line.find(substr.c_str());
        if (pos != static_cast<unsigned short int>(-1)) {
            x = pos;
            y = num_line;
            return;
        }
    }

    x = -1;
    y = -1;
}

void File::reverse_search(const MyString &substr, int &x, int &y) {
    if (y == 0 && x == 0) {
        x = -1, y = -1;
        return;
    }

    // get line when search should start
    auto iter = this->begin();
    std::advance(iter, y);

    // search in this line, starting from X-position

    /* not change!!! here must be copy-constructor
     * because of awful libMtString */
    int pos = -1;
    MyString line = *iter;

    if (line.size() >= substr.size()) {
        int last_pos;
        do {
            last_pos = pos;
            pos = line.find(substr.c_str(), last_pos + 1);
        } while (pos != static_cast<unsigned short>(-1) && pos < x);

        pos = last_pos;
        if (pos != static_cast<unsigned short>(-1) && pos != -1
            && (pos < x)) {
            x = pos;
            return;
        }
    }

    if (iter == this->begin()) {
        x = -1, y = -1;
        return;
    }

    // search in this line, starting from 0-position
    --iter;
    for (int num_line = y - 1; iter->c_str() != nullptr; --num_line, --iter) {
        /* not change!!! here must be copying
         * because of awful libMtString */
        line = *iter;

        if (substr.size() > line.size()) {
            continue;
        }

        int last_pos;
        do {
            last_pos = pos;
            pos = line.find(substr.c_str(), last_pos + 1);
        } while (pos != static_cast<unsigned short>(-1));

        pos = last_pos;
        while (pos != static_cast<unsigned short>(-1)) {
            x = pos;
            y = num_line;
            return;
        }
    }

    x = -1, y = -1;
}

