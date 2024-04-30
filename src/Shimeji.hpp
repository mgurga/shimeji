#pragma once

// clang-format off
#include <SFML/Graphics.hpp>

#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
// clang-format on
#include <iostream>

#include "ShimejiPack.hpp"

class Shimeji {
public:
    Shimeji(ShimejiPack& imgs, float scale) : image_pack(imgs), x(0), y(0), scale(scale) {
        window = new sf::RenderWindow(
            sf::VideoMode(imgs.get_size() * scale, imgs.get_size() * scale), "shimeji");
        window->setFramerateLimit(60);

        update_frame(0);
    }

    void update() {
        sf::Vector2i grabbedOffset;
        bool grabbedWindow = false;
        if (window->isOpen()) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed ||
                    event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window->close();
            }

            window->clear(sf::Color::Transparent);
            window->draw(spr);
            window->display();
        }
    }

    void update_frame(int num) {
        auto base_img = get_texture().copyToImage();
        sf::Image scaled_img;
        scaled_img.create(base_img.getSize().x * scale, base_img.getSize().y * scale);
        resizeImage(base_img, scaled_img);

        setShape(window->getSystemHandle(), scaled_img);
        setTransparency(window->getSystemHandle(), 255);

        spr.setTexture(get_texture(), true);
        spr.setPosition(0, 0);
        spr.setScale({scale, scale});
    }

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

    bool setShape(Window wnd, const sf::Image& image);
    bool setTransparency(Window wnd, unsigned char alpha);
    void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage);
};