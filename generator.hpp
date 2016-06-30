#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"

class Generator {
public:
    Generator();
    Generator(float);
    ~Generator();

    sf::VertexArray plane_map_generation();
    sf::VertexArray cylindric_map_generation();

private:
    sf::VertexArray terrain_rendering(noise::utils::NoiseMap, sf::VertexArray);

    float earth_percent;
};