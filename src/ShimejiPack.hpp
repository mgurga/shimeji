#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class ShimejiPack {
public:
    ShimejiPack() {}

    // directory should contain "shimeji1.png", "shimeji2.png", "shimeji3.png", ...
    bool load_pack_from_directory(std::filesystem::path dir) {
        if (!valid_pack_directory(dir)) {
            std::cout << "invalid shimeji directory, " << dir.string() << std::endl;
            return false;
        }

        for (int i = 1; i < 46; i++) {
            std::string shimeji_file_name = "shime" + std::to_string(i) + ".png";
            sf::Texture img;
            if (!img.loadFromFile((dir / shimeji_file_name).c_str())) {
                std::cout << "failed to load texture, " << (dir / shimeji_file_name).string()
                          << std::endl;
            }
            shimejis.push_back(img);
        }

        std::cout << "valid shimeji pack loaded, " << dir.string() << std::endl;
        return true;
    }

    bool valid_pack_directory(std::filesystem::path dir) {
        if (!std::filesystem::exists(dir / "shime1.png")) return false;
        if (!std::filesystem::exists(dir / "shime2.png")) return false;
        if (!std::filesystem::exists(dir / "shime3.png")) return false;
        return true;
    }

    int get_size() { return shimejis.at(0).getSize().x; }

    sf::Texture& get_texture(int num) { return shimejis.at(num); }

private:
    std::vector<sf::Texture> shimejis;
};