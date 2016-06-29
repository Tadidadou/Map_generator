#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"

class Generator {
public:
    Generator();
    Generator(float, int, int, int);
    ~Generator();

    sf::VertexArray random_map_generation();
    sf::VertexArray libnoise_based_map_generation();
    void province_generation();
    void nations_generation();

private:
    float earth_percent;
    int nations_number;
    int macro_nations_number;
    int techno_groups_number;
};
