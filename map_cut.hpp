#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "noiseutils.h"
#include "province.hpp"
#include "continent.hpp"

enum terrain_type {
    WATER,
    GRASS,
    FOREST, //in prevision
    DIRT,
    HILL,
    MOUNTAIN
};

struct provinces_map {
    terrain_type type;
    int num_prov;
};

class Map_cut {
public:
    Map_cut(sf::VertexArray, int);
    ~Map_cut();

    std::vector<Province> provinces_generation(noise::utils::NoiseMap, float);
    std::vector<Continent> continents_generation();
    sf::VertexArray draw_provinces();
    sf::VertexArray draw_continents();

    std::vector<Province> GetProvinces() {
        return this->all_provinces;
    }
    std::vector<Continent> GetContinents() {
        return this->all_continents;
    }

private:
    Province new_province(int, sf::VertexArray);
    void prov_map_generation(noise::utils::NoiseMap, float);

    int nb_prov;
    std::vector<std::vector<provinces_map>> prov_map;
    std::vector<Province> all_provinces;
    std::vector<Continent> all_continents;
    sf::VertexArray earth_map;
};
