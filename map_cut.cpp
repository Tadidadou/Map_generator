#include "map_cut.hpp"
#include "constants.hpp"


Map_cut::Map_cut(sf::VertexArray earth_m) {
    this->earth_map = earth_m;
}


Map_cut::~Map_cut() {
    std::cout << "They killed Map_cut !" << std::endl;
}


///Generating all the province
std::vector<Province> Map_cut::province_generation() {
    std::vector<Province> all_provinces;
    sf::Vertex current;
    int i, cpt;

    for(i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        current = earth_map[i];
    }

    std::cout << "Provinces generation done" << std::endl;
    return all_provinces;
}
