#pragma once

#include <iostream>

#include "ShimejiPack.hpp"
#include "Utils.hpp"

class Shimeji {
public:
    Shimeji(ShimejiPack& imgs, float scale) : image_pack(imgs), x(0), y(0), scale(scale) {
        window = new sf::RenderWindow(
            sf::VideoMode(imgs.get_size() * scale, imgs.get_size() * scale), "shimeji");
        window->setFramerateLimit(60);

        update_frame(0);
    }

    void update();
    void update_frame(int num);

    sf::Texture& get_texture() { return image_pack.get_texture(curframe); }
    int get_x() { return x; }
    int get_y() { return y; }

private:
    int curframe = 0;
    int x, y;
    float scale;
    ShimejiPack& image_pack;
    sf::RenderWindow* window;
    sf::Sprite spr;
};