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
    void draw_continents(sf::RenderWindow&);
    sf::VertexArray show_specified_province(int);

    sf::VertexArray GetProvincesMap() {
        return this->vertex_prov_map;
    }
    sf::VertexArray GetProvincesBordersMap() {
        return this->vertex_prov_borders_map;
    }
    std::vector<Province> GetProvinces() {
        return this->all_provinces;
    }
    std::vector<Continent> GetContinents() {
        return this->all_continents;
    }
    int GetSelectedProvId(int x, int y) {
        return this->prov_map[x][y].num_prov;
    }

private:
    int isBorder(int, int);
    bool isGc(int, int);
    Province new_province(int, sf::VertexArray);
    int prov_map_generation(noise::utils::NoiseMap, float);
    sf::VertexArray generate_vertex_prov_map();
    sf::VertexArray generate_vertex_prov_borders_map();
    void determine_neighbours();

    int nb_prov;
    std::vector<std::vector<provinces_map> > prov_map;
    std::vector<Province> all_provinces;
    std::vector<Continent> all_continents;
    sf::VertexArray earth_map;
    sf::VertexArray vertex_prov_map;
    sf::VertexArray vertex_prov_borders_map;
};
