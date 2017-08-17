#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "constants.hpp"

class Generator {
public:
    Generator();
    Generator(float);
    Generator(float, int, int);
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
    Map_dimensions GetMap_dimensions() {
        return this->dimensions;
    }

private:
    sf::VertexArray terrain_rendering(sf::VertexArray);

    Map_dimensions dimensions;
    noise::utils::NoiseMap heightMap;
    float earth_percent;
    sf::VertexArray earth_map;
};
