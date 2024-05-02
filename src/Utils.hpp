#pragma once

#include <X11/Xatom.h>
#include <X11/extensions/shape.h>

#include "Surface.hpp"

class Utils {
public:
    static int get_screen_width() {
        Display* display = XOpenDisplay(NULL);
        auto s = DefaultScreenOfDisplay(display);
        XCloseDisplay(display);
        return s->width;
    }

    static int get_screen_height() {
        Display* display = XOpenDisplay(NULL);
        auto s = DefaultScreenOfDisplay(display);
        XCloseDisplay(display);
        return s->height;
    }

    template <typename T>
    static bool contains(std::vector<T>& v, T val) {
        for (T it : v)
            if (val == it) return true;
        return false;
    }
};