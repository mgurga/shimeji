#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <vector>

#include "Shimeji.hpp"
#include "ShimejiPack.hpp"

class Manager {
public:
    Manager(float scale) : scale(scale) {
        rng = std::mt19937(time(NULL));

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
            Shimeji s(&packs[0], scale, &rng);
            shimejis.push_back(s);
        }
    }

    void update() {
        for (auto it = shimejis.begin(); it != shimejis.end(); it++) {
            if (!(*it).tick()) {
                it = shimejis.erase(it);
                it--;
            }
        }
    }

    Shimeji& get_shimeji(int n) { return shimejis.at(n); }

    float get_scale() { return scale; }

private:
    std::mt19937 rng;
    std::vector<Shimeji> shimejis;
    const std::filesystem::path shimeji_pack_dirs = {"."};
    std::vector<ShimejiPack> packs;
    const float scale;
};