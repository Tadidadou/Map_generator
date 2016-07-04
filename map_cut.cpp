#include "map_cut.hpp"
#include "constants.hpp"


Map_cut::Map_cut(sf::VertexArray earth_m, int nb_prov) {
    this->earth_map = earth_m;
    this->nb_prov = nb_prov;
}


Map_cut::~Map_cut() {
    std::cout << "They killed Map_cut !" << std::endl;
}


///Generating the provinces_map
void Map_cut::prov_map_generation(noise::utils::NoiseMap heightMap, float earth_percent) {
    float earth, grass, dirt, hill, mountain;
    int x, y;
    x = y =0;

    earth = 1 - ((earth_percent / 100) * 2);
    dirt = earth + (0.2000 * (earth_percent / 100) * 2);
    hill = earth + (0.5000 * (earth_percent / 100) * 2);
    mountain = earth + (0.7500 * (earth_percent / 100) * 2);

    for(x=0; x < WIN_HEIGHT; x++) {
        for(y=0; y < WIN_WIDTH; y++) {
            if(heightMap.GetValue(x, y) < earth)
                prov_map[x][y].type = WATER;
            else {
                if(heightMap.GetValue(x, y) < dirt)
                    prov_map[x][y].type = GRASS;
                else if (heightMap.GetValue(x, y) < hill)
                    prov_map[x][y].type = DIRT;
                else if(heightMap.GetValue(x, y) < mountain)
                    prov_map[x][y].type = HILL;
                else
                    prov_map[x][y].type = MOUNTAIN;
            }
        }
    }
}


///Generating all the province
std::vector<Province> Map_cut::provinces_generation(noise::utils::NoiseMap heightMap, float earth_percent) {


    std::cout << "Provinces generation done" << std::endl;
    return this->all_provinces;
}
