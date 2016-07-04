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

    sf::VertexArray GetEarth_map() {
        return this->earth_map;
    }
    noise::utils::NoiseMap GetHeightMap () {
        return this->heightMap;
    }
    float GetEarth_percent() {
        return this->earth_percent;
    }

private:
    sf::VertexArray terrain_rendering(sf::VertexArray);
    noise::utils::NoiseMap heightMap;
    float earth_percent;
    sf::VertexArray earth_map;
};
