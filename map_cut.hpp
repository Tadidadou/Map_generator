#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "noiseutils.h"
#include "province.hpp"
#include "continent.hpp"
#include "connexity.hpp"
#include "constants.hpp"
#include "utils.hpp"

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
    std::vector<std::vector<provinces_map>> prov_map;

private:
    Province new_province(int, sf::VertexArray);
    void prov_map_generation(noise::utils::NoiseMap, float);
    int explore(int, int);

    int nb_prov;

    std::vector<Province> all_provinces;
    std::vector<Continent> all_continents;
    sf::VertexArray earth_map;
};
