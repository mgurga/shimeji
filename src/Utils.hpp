#pragma once

// clang-format off
#include <SFML/Graphics.hpp>

#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
// clang-format on

class Utils {
public:
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
};

#undef None