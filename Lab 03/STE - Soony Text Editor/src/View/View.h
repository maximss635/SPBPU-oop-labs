//
// Created by maxim on 15.11.2020.
//

#ifndef STE_VIEW_H
#define STE_VIEW_H

#include "Kernel.h"
#include "NCursesAdapter.h"

/* View part in MVC pattern */

class View : public Observable, public Observer {
public:
    View(Kernel& kernel, GraphicLibAdapter* adapter);
    ~View();

    void update(const SendData& data) override {
        /* when view recive message from kernel to update,
         * view should say to update his aggregants */
        notify_update(data);
    }

    inline int text_area_width() const {
        return text_area->width();
    }

    inline int text_area_height() const {
        return text_area->height();
    }

    inline void set_cursor(int x, int y) const {
        text_area->set_cursor(x, y);
    }

private:
    class Block : public Observer {
    public:
        Block(GraphicLibAdapter* adapter, int x, int y, int width, int height,
              short color_id, short color_text,
              short color_background, char c_gorizontal, char c_vertical) :

                x(x), y(y),
                width_(width), height_(height),
                color_id(color_id),
                source(nullptr),
                adapter(adapter)
        {
            adapter->create_color_id(color_id, color_text, color_background);
            adapter->crate_block(x, y, width, height, c_gorizontal, c_vertical);
        }

        inline int width() const {
            return width_;
        }

        inline int height() const {
            return height_;
        }

        inline void set_source(File* file_pointer) {
            source = file_pointer;
        }


        // wrapping around adapter
        inline void set_cursor(int _x, int _y) const {
            adapter->set_cursor(this->x + _x + 1, this->y + _y + 1);
        }

        inline void print(int line, const MyString& str) const {
            adapter->print(this->x + 1, line + this->y + 1, str.c_str(), this->color_id);
        }

        inline void print(int line, const int d) const {
            adapter->print(this->x + 1, line + this->y + 1, d, this->color_id);
        }

        inline void clear_line(int line) const {
            adapter->clear_line(0, line + this->y + 1, this->width(), this->color_id);
        }


    protected:
        int x, y, width_, height_;
        short color_id;
        File* source;

        // adapter to connect with graphic lib
        GraphicLibAdapter* adapter;
    };


    class BlockText : public Block {
    public:
        BlockText(GraphicLibAdapter* adapter,
                  int x, int y, int width, int height, short color_text, short color_background) :
            Block(adapter, x, y, width, height, 2, color_text, color_background, '-', '|') { }

        void update(const SendData& data) override;
    };

    class BlockNumbering : public Block {
    public:
        BlockNumbering(GraphicLibAdapter* adapter,
                       int x, int y, int width, int height, short color_text, short color_background) :
                Block(adapter, x, y, width, height, 3, color_text, color_background, '-', '|') { }

        void update(const SendData& data) override;
    };

    class BlockStatusBar : public Block {
    public:
        BlockStatusBar(GraphicLibAdapter* adapter,
                       int x, int y, int width, int height, short color_text, short color_background) :
                Block(adapter, x, y, width, height, 4, color_text, color_background, '-', '|') { }

        void update(const SendData& data) override;
    };

    class BlockModeCmd : public Block {
    public:
        BlockModeCmd(GraphicLibAdapter* adapter,
                     int x, int y, int width, int height, short color_text, short color_background) :
                Block(adapter, x, y, width, height, 5, color_text, color_background, ' ', ' ') { }

        void update(const SendData& data) override;
    };

    BlockText*      text_area;
    BlockNumbering* num_area;
    BlockStatusBar* status_bar;
    BlockModeCmd*   mode_cmd;

    // adapter to graphic lib
    GraphicLibAdapter* adapter;
};

#endif //STE_VIEW_H
