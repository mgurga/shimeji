#include "Shimeji.hpp"

bool Shimeji::tick() {
    if (window->isOpen()) {
        System::set_always_on_top(window->getSystemHandle());
        auto surfs = System::get_window_surfaces();

        bool can_fall = true;
        for (Surface& s : surfs) {
            if (s.get_type() == SurfaceType::Floor) {
                if (s.get_x() < get_x() && s.get_x() + s.get_width() > get_x()) {
                    if (s.get_y() <= get_y() + get_size() &&
                        s.get_y() + s.get_height() >= get_y() + get_size()) {
                        can_fall = false;
                        y = s.get_y() - get_size();
                    }
                }
            }
        }

        if (can_fall) {
            y += 4;
            update_frame(3);
        } else {
            update_frame(0);
        }

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window->close();
                return false;
            }
        }

        window->clear(sf::Color::Transparent);
        window->draw(spr);
        window->display();
    }

    window->setPosition({x, y});

    return true;
}

void Shimeji::update_frame() {
    auto base_img = get_texture().copyToImage();
    sf::Image scaled_img;
    scaled_img.create(base_img.getSize().x * scale, base_img.getSize().y * scale);
    System::resizeImage(base_img, scaled_img);

    System::setShape(window->getSystemHandle(), scaled_img);
    System::setTransparency(window->getSystemHandle(), 255);

    spr.setTexture(get_texture(), true);
    spr.setPosition(0, 0);
    spr.setScale({scale, scale});
}