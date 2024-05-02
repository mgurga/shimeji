#include <iostream>
#include <string>

#include "Manager.hpp"

int main() {
    float scale = 2;

    Manager m(scale);
    m.create_shimeji(3);

    while (true) { m.update(); }
}
