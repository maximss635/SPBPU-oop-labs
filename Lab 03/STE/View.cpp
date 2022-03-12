//
// Created by maxim on 15.11.2020.
//

#include "View.h"
#include "Journal.h"

#include <vector>

View::View() {
    Journal::log("Console init");

    initscr();
    noecho();

    // VALUES = ???
    text_area = new BlockText(45, 150, 5, 6, '|', '-');
    numb_area = new BlockText(45, 5, 0, 6, '|', '-');
    logo =      new BlockLogo(45, 15, 155, 6);
    state_str = new BlockState(6, 170, 0, 0);


    text_area->set_cursor(0, 0);
    numb_area->set_file_pointer(&num_list);

    MyString zero("0");
    num_list.push_back(zero);

    numb_area->update(0, 0);


    start_color();
}

View::~View() {
    Journal::log("Console deinit");

    delete text_area;
    delete numb_area;
    delete logo;
    delete state_str;

    endwin();
}




View::Block::Block(int height, int width, int x, int y, short text_color, short background_color, char c1, char c2) :
    height_(height), width_(width),
    x(x), y(y),
    text_color(text_color),
    background_color(background_color) {

    window = newwin(height, width, y, x);
    refresh();

    box(window, c1, c2);
    wrefresh(window);

    init_pair(1, text_color, background_color);
}

View::Block::~Block() {
    delwin(window);
}

void View::BlockText::update(int line_start, int line_end) {
    MyString spaces("");
    spaces.append(width() - 2, ' ');

    auto iter = lines_pointer_->begin();
    std::advance(iter, first_line_index + line_start);

    // now ITER refer to line which should be in place LINE_START
    int i;
    for (i = line_start; (i <= line_end) && (i <= height()); ++i) {
        if (iter == lines_pointer_->end()) {
            break;
        }

        print(i, spaces);
        print(i, *iter);

        ++iter;
    }

    while (i <= line_end && line_end <= height()) {
        print(i, spaces);
        ++i;
    }


    /*      FOR EXAMPLE TEXT IN MEMORY:
     *
     *          sdlkhslkg1231
     *          dsfkdsklfhdsklfhsldf
     *          hdslksfdh
     *          dsfklhdsfkldshfk
     *          ldhs23              -------------------------
     *          slkfhklfhsdlkfhksdlf            |           |       <-- FIRST_LINE_INDEX
     *          dsfjksdfgkdjs                   |           |
     *          sdflkhdsflkhds                  |           |       <-- LINE_START (start updating from this line)
     *          sdfklhsdfkds                    |           |       }   update
     *          dfsksdklfbds                    |           |       }   update
     *          dfsksdklfbds                    |  screen   |       }   update
     *          dsfhsdlkfhds                    |           |       }   update
     *          dfsksdklfbds                    |           |       <-- LINE_END -> stop updating
     *          dsfhsdlkfhds                    |           |
     *          dsfhsdlkfhdskfhdskf             |           |
     *          sdlfhdsf            -------------------------
     *          ;hdsfklhsdfkl
     *          dsljfdslkfdsklfh
     *          dsflkhdfl
     *
     * */
}





void View::BlockLogo::update(int line_start, int line_end) {
    static int i = 0;

    /*    ******
     *    *
     *    ******
     *         *
     *    ******
     *
     *    ******
     *      **
     *      **
     *      **
     *      **
     *
     *    ******
     *    *
     *    ******
     *    *
     *    ******
     *
     * */

    static const int num_states = 23;
    MyString gif_logo[num_states];

    gif_logo[0] = "******";     gif_logo[6] =  "******";     gif_logo[12] =  "******";      gif_logo[18] =  "******";
    gif_logo[1] = "*    *";     gif_logo[7] =  "*     ";     gif_logo[13] =  "  **  ";      gif_logo[19] =  "*     ";
    gif_logo[2] = "*    *";     gif_logo[8] =  "******";     gif_logo[14] =  "  **  ";      gif_logo[20] =  "******";
    gif_logo[3] = "*    *";     gif_logo[9] =  "     *";     gif_logo[15] =  "  **  ";      gif_logo[21] =  "*     ";
    gif_logo[4] = "******";     gif_logo[10] = "******";     gif_logo[16] =  "  **  ";      gif_logo[22] =  "******";
    gif_logo[5] = "      ";     gif_logo[11] = "      ";     gif_logo[17] =  "      ";

    for (int j = 0; j < gif_logo[i].size(); ++j) {
        if (gif_logo[i][j] == '*') {
            gif_logo[i][j] = '-';
        }
    }

    int j = 0;
    for (MyString& str : gif_logo) {
        str.insert(0, 3, ' ');      // 3 ???
        print(j++, str);
    }


    i = (i + 1) % num_states;
}

void View::BlockState::update(int line_start, int line_end) {
    MyString output[4];

    output[0] = "LongPath: ";
    output[0] += file_pointer_->longpath;
    output[0].append(67, ' ');   // 67 ??

    output[1] = "File:     ";
    output[1] += file_pointer_->path;

    if (!file_pointer_->is_saved) {
        output[0] += "[not saved]";
    }
    else {
        output[0] += "    [saved]";
    }

    output[2] = "X: ?/?";
    output[3] = "Y: ?/?";

    print(0, output[0]);
    print(1, output[1]);
    print(2, output[2]);
    print(3, output[3]);
}
