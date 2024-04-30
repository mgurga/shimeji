#include "Shimeji.hpp"

void Shimeji::update() {
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

void Shimeji::update_frame(int num) {
    auto base_img = get_texture().copyToImage();
    sf::Image scaled_img;
    scaled_img.create(base_img.getSize().x * scale, base_img.getSize().y * scale);
    Utils::resizeImage(base_img, scaled_img);

    Utils::setShape(window->getSystemHandle(), scaled_img);
    Utils::setTransparency(window->getSystemHandle(), 255);

    spr.setTexture(get_texture(), true);
    spr.setPosition(0, 0);
    spr.setScale({scale, scale});
}