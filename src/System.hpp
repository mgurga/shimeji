#pragma once

// clang-format off
#include <SFML/Graphics.hpp>

#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
// clang-format on

#include <limits.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "Surface.hpp"
#include "Utils.hpp"

class System {
public:
    static std::vector<Surface> get_window_surfaces() {
        std::vector<Surface> out;
        Display* d = XOpenDisplay(NULL);
        Screen* s = DefaultScreenOfDisplay(d);
        if (d == NULL) return {};
        Window root = RootWindow(d, 0);

        auto add_surface = [&out](int x, int y, int width, int height) -> void {
            // std::cout << "added surfaces for window " << x << ", " << y << " " << width << "x"
            //           << height << std::endl;
            out.push_back(Surface(x, y, width, 10, SurfaceType::Floor));
            out.push_back(Surface(x, y + 1, 10, height - 1, SurfaceType::LeftWall));
            out.push_back(Surface(x, y + width - 10, 10, height, SurfaceType::RightWall));
        };

        Atom window_type_atom = XInternAtom(d, "_NET_WM_WINDOW_TYPE", false);

        // iterate all windows
        // std::cout << "got " << get_all_windows(d, root).size() << " total windows" << std::endl;
        for (Window w : get_all_windows(d, root)) {
            int winx, winy;
            unsigned int winwidth, winheight, trash;
            int status =
                XGetGeometry(d, w, &w, &winx, &winy, &winwidth, &winheight, &trash, &trash);
            if (status == 0) {
                std::cout << "couldnt get win geometry" << std::endl;
                return {};
            }

            std::vector<std::string> atom_list;
            int atom_num;
            Atom* atom_list_ptr = XListProperties(d, w, &atom_num);
            for (int i = 0; i < atom_num; i++) {
                auto an = XGetAtomName(d, atom_list_ptr[i]);
                atom_list.push_back(std::string(an));
            }

            bool to_add = true;

            to_add = to_add && winx > 0 && winy > 0 && winwidth > 100 && winheight > 100;
            // to_add = to_add && Utils::contains(atom_list, std::string("_NET_DESKTOP_VIEWPORT"));
            // to_add = to_add && Utils::contains(atom_list, std::string("_NET_CURRENT_DESKTOP"));

            if (to_add) add_surface(winx, winy, winwidth, winheight);
        }

        XCloseDisplay(d);
        return out;
    }

    static bool setShape(Window wnd, const sf::Image& image) {
        Display* display = XOpenDisplay(NULL);

        int event_base;
        int error_base;
        if (!XShapeQueryExtension(display, &event_base, &error_base)) {
            XCloseDisplay(display);
            return false;
        }

        const sf::Uint8* pixelData = image.getPixelsPtr();

        Pixmap pixmap = XCreatePixmap(display, wnd, image.getSize().x, image.getSize().y, 1);
        GC gc = XCreateGC(display, pixmap, 0, NULL);

        XSetForeground(display, gc, 1);
        XFillRectangle(display, pixmap, gc, 0, 0, image.getSize().x, image.getSize().y);

        XSetForeground(display, gc, 0);
        bool transparentPixelFound = false;
        unsigned int rectLeft = 0;
        for (unsigned int y = 0; y < image.getSize().y; y++) {
            for (unsigned int x = 0; x < image.getSize().x; x++) {
                const bool isTransparentPixel =
                    (pixelData[y * image.getSize().x * 4 + x * 4 + 3] == 0);
                if (isTransparentPixel && !transparentPixelFound) {
                    transparentPixelFound = true;
                    rectLeft = x;
                } else if (!isTransparentPixel && transparentPixelFound) {
                    XFillRectangle(display, pixmap, gc, rectLeft, y, x - rectLeft, 1);
                    transparentPixelFound = false;
                }
            }

            if (transparentPixelFound) {
                XFillRectangle(display, pixmap, gc, rectLeft, y, image.getSize().x - rectLeft, 1);
                transparentPixelFound = false;
            }
        }

        XShapeCombineMask(display, wnd, ShapeBounding, 0, 0, pixmap, ShapeSet);

        XFreeGC(display, gc);
        XFreePixmap(display, pixmap);
        XFlush(display);
        XCloseDisplay(display);
        return true;
    }

    static bool setTransparency(Window wnd, unsigned char alpha) {
        Display* display = XOpenDisplay(NULL);
        unsigned long opacity = (0xffffffff / 0xff) * alpha;
        Atom property = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", false);
        if (property != None) {
            XChangeProperty(display, wnd, property, XA_CARDINAL, 32, PropModeReplace,
                            (unsigned char*)&opacity, 1);
            XFlush(display);
            XCloseDisplay(display);
            return true;
        } else {
            XCloseDisplay(display);
            return false;
        }
    }

    static void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage) {
        const sf::Vector2u originalImageSize{originalImage.getSize()};
        const sf::Vector2u resizedImageSize{resizedImage.getSize()};
        for (unsigned int y{0u}; y < resizedImageSize.y; ++y) {
            for (unsigned int x{0u}; x < resizedImageSize.x; ++x) {
                unsigned int origX{static_cast<unsigned int>(
                    static_cast<double>(x) / resizedImageSize.x * originalImageSize.x)};
                unsigned int origY{static_cast<unsigned int>(
                    static_cast<double>(y) / resizedImageSize.y * originalImageSize.y)};
                resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
            }
        }
    }

    static void set_always_on_top(Window w, bool enabled = true) {
        Display* d = XOpenDisplay(NULL);
        static Atom wmStateAbove = XInternAtom(d, "_NET_WM_STATE_ABOVE", 1);
        static Atom wmNetWmState = XInternAtom(d, "_NET_WM_STATE", 1);

        if (wmStateAbove) {
            XClientMessageEvent emsg;
            memset(&emsg, 0, sizeof(emsg));
            emsg.type = ClientMessage;
            emsg.window = w;
            emsg.message_type = wmNetWmState;
            emsg.format = 32;
            emsg.data.l[0] = enabled;
            emsg.data.l[1] = wmStateAbove;
            XSendEvent(d, RootWindow(d, 0), false,
                       SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&emsg);
        }
        XCloseDisplay(d);
    }

private:
    static std::vector<Window> get_all_windows(Display* d, Window start) {
        std::vector<Window> out;

        for (auto w : get_window_children(d, start)) {
            out.push_back(w);
            for (auto c : get_all_windows(d, w)) out.push_back(c);
        }

        return out;
    }

    static std::vector<Window> get_window_children(Display* d, Window w) {
        std::vector<Window> out;
        Window parent;
        Window* children;
        Window root_return;
        unsigned int children_num;

        int status = XQueryTree(d, w, &root_return, &parent, &children, &children_num);
        if (status == 0) {
            std::cout << "failed to get window children" << std::endl;
            return {};
        }

        for (unsigned int i = 0; i < children_num; i++) out.push_back(children[i]);

        return out;
    }
};

#undef None