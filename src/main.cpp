#include <chrono>
#include <iostream>
#include <string>

#include "Manager.hpp"

int main() {
    float scale = 2;

    Manager m(scale);
    m.create_shimeji(3);

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        m.update();

        auto end = std::chrono::high_resolution_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 17) {
            end = std::chrono::high_resolution_clock::now();
        }
    }
}
