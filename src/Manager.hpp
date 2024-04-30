#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

#include "Shimeji.hpp"
#include "ShimejiPack.hpp"

class Manager {
public:
    Manager(float scale) : scale(scale) {
        std::cout << "loading shimeji packs..." << std::endl;
        for (auto dir : shimeji_pack_dirs) {
            for (auto const& dir_entry : std::filesystem::directory_iterator{dir}) {
                if (dir_entry.is_directory()) {
                    ShimejiPack p;
                    if (p.load_pack_from_directory(dir_entry)) packs.push_back(p);
                }
            }
        }
    }

    void create_shimeji(int num) {
        for (int i = 0; i < num; i++) {
            Shimeji s(packs[0], scale);
            shimejis.push_back(s);
        }
    }

    void update() {
        for (auto& s : shimejis) { s.update(); }
    }

    Shimeji& get_shimeji(int n) { return shimejis.at(n); }

    float get_scale() { return scale; }

private:
    std::vector<Shimeji> shimejis;
    const std::filesystem::path shimeji_pack_dirs = {"."};
    std::vector<ShimejiPack> packs;
    const float scale;
};