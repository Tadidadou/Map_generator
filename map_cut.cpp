#include "map_cut.hpp"


Map_cut::Map_cut(sf::VertexArray earth_m, int nb_prov) {
    this->earth_map = earth_m;
    this->nb_prov = nb_prov;
    this->prov_map.resize(WIN_WIDTH);
    for(auto & v : this->prov_map)
        v.resize(WIN_HEIGHT);
}


Map_cut::~Map_cut() {
    std::cout << "They killed Map_cut !" << std::endl;
}


///Generating the provinces_map
void Map_cut::prov_map_generation(noise::utils::NoiseMap heightMap, float earth_percent) {
    float earth, grass, dirt, hill, mountain;
    int x, y;
    x = 0;
    y = 0;

    earth = 1 - ((earth_percent / 100) * 2);
    dirt = earth + (0.2000 * (earth_percent / 100) * 2);
    hill = earth + (0.5000 * (earth_percent / 100) * 2);
    mountain = earth + (0.7500 * (earth_percent / 100) * 2);

    for(x=0; x < WIN_WIDTH; x++) {
        for(y=0; y < WIN_HEIGHT; y++) {
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
            prov_map[x][y].num_prov = 0;
        }
    }
}


///Generating all the province
std::vector<Province> Map_cut::provinces_generation(noise::utils::NoiseMap heightMap, float earth_percent) {
    int i, x, y, num;
    IterationResult iterationResult;
    srand(time(0));

    prov_map_generation(heightMap, earth_percent);

    ///Choosing the first pixel of each province
    for(i=0; i < nb_prov; i++) {
        x = rand()%WIN_WIDTH;
        y = rand()%WIN_HEIGHT;
        if(prov_map[x][y].type != WATER && prov_map[x][y].num_prov == 0) {
            prov_map[x][y].num_prov = i + 1;
        }
        else
            i--;
    }

    int cpt = 0;
    ///Adding every earth pixel to a province
    iterationResult.changes = 1;
    while(iterationResult.changes != 0) {
        iterationResult = connexity(prov_map, 50);

        for (auto& rp : iterationResult.newRegionPoints)
            prov_map[rp.x][rp.y].num_prov = rp.id;
        cpt++;
        std::cout << "Iteration n." << cpt << std::endl;
    }


    ///Completing the remaining earth
    /// TODO ///

    std::cout << "Provinces generation done" << std::endl;
    return this->all_provinces;
}
