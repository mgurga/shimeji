#pragma once

enum class SurfaceType { Floor, LeftWall, RightWall };

class Surface {
public:
    Surface() {}
    Surface(int x, int y, int w, int h, SurfaceType t) : x(x), y(y), width(w), height(h), type(t) {}

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    SurfaceType get_type() { return type; }

private:
    int x, y, width, height;
    SurfaceType type;
};