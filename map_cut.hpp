#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "province.hpp"
#include "continent.hpp"

class Map_cut {
public:
    Map_cut(sf::VertexArray);
    ~Map_cut();

    std::vector<Province> province_generation();
    std::vector<Continent> continent_generation();

    std::vector<Province> GetProvinces() {
        return this->all_provinces;
    }
    std::vector<Continent> GetContinents() {
        return this->all_continents;
    }

private:
    std::vector<Province> all_provinces;
    std::vector<Continent> all_continents;
    sf::VertexArray earth_map;
};
