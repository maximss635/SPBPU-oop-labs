//
// Created by maxim on 21.11.2020.
//

#include "File.h"
#include "Journal.h"

File::File(MyString &path) : path(path), is_saved(false) {
    //Journal::log("file init");

    if (path.empty()) {
        this->path = sample_path;
    }

    char* tmp = new char[128];

    Journal::log(path);
    realpath(this->path.c_str(), tmp);
    Journal::log(tmp);

    longpath = tmp;

    delete [] tmp;
}

// load file from disk to RAM-memory (StringArray ALL_LINES_FILE)
void File::load() {
    Journal::log("start loading file");

    std::ifstream file_input(path.c_str());
    if (!file_input.is_open()) {
        // No such file
        // May be without exceptions ?
        throw std::exception();
    }

    MyString line;
    while (!file_input.eof()) {
        // read and save all lines

        file_input >> line;
        lines.push_back(line);
    }
    file_input.close();


    Journal::log("end loading file");
}


// save file from RAM-memory (StringArray ALL_LINES_FILE) to disk
void File::save() {
    Journal::log("start saving file");
    Journal::log(path);

    std::ofstream file_output(path.c_str());

    // write all lines from StringArray to file
    for (MyString& line : lines) {
        file_output << line << "\n";
    }

    file_output.close();
    is_saved = true;

    Journal::log("end saving file");
}

void File::create() {
    Journal::log("create");
    std::ofstream file(path.c_str());
    file.close();
}
