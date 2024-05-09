#pragma once

#include <iostream>
#include <random>

#include "ShimejiPack.hpp"
#include "System.hpp"
#include "Utils.hpp"

class Shimeji {
public:
    Shimeji() : image_pack(nullptr), x(0), y(0), scale(1), rng(nullptr) {}
    Shimeji(ShimejiPack* imgs, float scale, std::mt19937* rng) :
        image_pack(imgs), scale(scale), rng(rng) {
        window =
            new sf::RenderWindow(sf::VideoMode(imgs->get_size() * scale, imgs->get_size() * scale),
                                 "shimeji", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(60);
        x = rand(0, Utils::get_screen_width() - (imgs->get_size() * scale));
        y = 1;
        window->setPosition({x, y});

        update_frame();
    }

    bool tick();
    void update_frame();
    void update_frame(int framenum) {
        curframe = framenum;
        update_frame();
    }

    sf::Texture& get_texture() { return image_pack->get_texture(curframe); }
    int get_x() { return x; }
    int get_y() { return y; }
    int get_size() { return get_texture().getSize().y * scale; }
    int rand(int min, int max) {
        std::uniform_int_distribution<int> gen(min, max);
        return gen(*rng);
    }

private:
    int curframe = 0;
    int x, y;
    float scale;
    ShimejiPack* image_pack;
    std::mt19937* rng;
    sf::RenderWindow* window;
    sf::Sprite spr;
};