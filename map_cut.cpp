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


///Generating the vertex array of the provinces
sf::VertexArray Map_cut::generate_vertex_prov_map() {
    int x = 0;
    int y = 0;
    vertex_prov_map = sf::VertexArray(sf::Points, WIN_WIDTH * WIN_HEIGHT);

    for(unsigned int i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        vertex_prov_map[i].position = sf::Vector2f(x, y);
        if(prov_map[x][y].type != WATER)
            vertex_prov_map[i].color = all_provinces[prov_map[x][y].num_prov].GetColor();
        else
            vertex_prov_map[i].color = earth_map[i].color;

        x++;
        if(x == WIN_WIDTH) {
            y++;
            x = 0;
        }
    }

    return vertex_prov_map;
}


///Return true if the pixel is on a boarder between provinces
bool Map_cut::isBorder(int x, int y) {
    if(y != 0) {
        if(prov_map[x][y].num_prov != prov_map[x][y-1].num_prov)
            return true;
    }
    if(y != WIN_HEIGHT - 1) {
        if(prov_map[x][y].num_prov != prov_map[x][y+1].num_prov)
            return true;
    }
    if(prov_map[x][y].num_prov != prov_map[(x-1)%WIN_WIDTH][y-1].num_prov)
        return true;
    else if(prov_map[x][y].num_prov != prov_map[(x+1)%WIN_WIDTH][y+1].num_prov)
        return true;
    else
        return false;
}


///Generating the vertex array of the naked map with boarders
sf::VertexArray Map_cut::generate_vertex_prov_borders_map() {
    int x = 0;
    int y = 0;
    vertex_prov_borders_map = sf::VertexArray(sf::Points, WIN_WIDTH * WIN_HEIGHT);

    for(unsigned int i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        vertex_prov_borders_map[i].position = sf::Vector2f(x, y);
        if(prov_map[x][y].type != WATER && isBorder(x, y))
            vertex_prov_borders_map[i].color = sf::Color(0, 0, 0);
        else
            vertex_prov_borders_map[i].color = earth_map[i].color;

        x++;
        if(x == WIN_WIDTH) {
            y++;
            x = 0;
        }
    }

    return vertex_prov_borders_map;
}


///Return the vertex array to show only one province, specified by user
sf::VertexArray Map_cut::show_specified_province(int id_prov) {
    int x = 0;
    int y = 0;
    sf::VertexArray result_map(sf::Points, WIN_WIDTH * WIN_HEIGHT);

    for(unsigned int i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        result_map[i].position = sf::Vector2f(x, y);
        if(prov_map[x][y].num_prov == id_prov)
            result_map[i].color = all_provinces[prov_map[x][y].num_prov].GetColor();
        else
            result_map[i].color = earth_map[i].color;

        x++;
        if(x == WIN_WIDTH) {
            y++;
            x = 0;
        }
    }

    return result_map;
}


///Generating the provinces_map
int Map_cut::prov_map_generation(noise::utils::NoiseMap heightMap, float earth_percent) {
    float earth, dirt, hill, mountain;
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
    return prov_map.size();
}


///Generating all the province
std::vector<Province> Map_cut::provinces_generation(noise::utils::NoiseMap heightMap, float earth_percent) {
    int i, x, y, nb, remaining_earth, n_loop, cpt, total_earth;
    bool changes = true;
    IterationResult iterationResult;
    srand(time(0));

    ///Calculating the number of earth pixel
    total_earth = prov_map_generation(heightMap, earth_percent);


    ///Creating the provinces
    n_loop = 0;
    cpt = 0;
    nb = nb_prov;
    int num_province = 1;
    std::cout << "Province generation...  ";
    while(changes) {
        n_loop++;
        remaining_earth = 0;

        ///Choosing the first pixel of each province
        for(i = 0; i < nb; i++) {
            x = rand()%WIN_WIDTH;
            y = rand()%WIN_HEIGHT;
            if(prov_map[x][y].type != WATER && prov_map[x][y].num_prov == 0) {
                prov_map[x][y].num_prov = num_province;

                coord_terrain pixel;
                pixel.coord = sf::Vector2f(x, y);
                pixel.type = prov_map[x][y].type;
                all_provinces.push_back(Province(pixel, num_province++));
            }
            else
                i--;
        }

        ///Adding every earth pixel to a province
        iterationResult.changes = 1;
        while(iterationResult.changes != 0) {
            iterationResult = connexity(prov_map, 50);

            for (auto& rp : iterationResult.newRegionPoints) {
                prov_map[rp.x][rp.y].num_prov = rp.id;

                coord_terrain pixel;
                pixel.coord = sf::Vector2f(rp.x, rp.y);
                pixel.type = prov_map[rp.x][rp.y].type;
                all_provinces[rp.id-1].addPixel(pixel);
                cpt++;
            }
        }

        ///Searching remaining earth
        for(x=0; x<WIN_WIDTH; x++) {
            for(y=0; y<WIN_HEIGHT; y++) {
                if(prov_map[x][y].type != WATER && prov_map[x][y].num_prov == 0) {
                    remaining_earth++;
                }
            }
        }

        ///Loading indicator
        std::cout << " - " << ((((float) remaining_earth / (float) total_earth) * 100.0) - 100) * (-1) << "%";

        nb = remaining_earth / 1000;
        if(nb == 0) {
            changes = false;
        }
    }
    std::cout << std::endl << "Number of loops : " << n_loop << std::endl;

    for(i = 0; i < all_provinces.size(); i++) {
        all_provinces[i].determine_type();
    }
    generate_vertex_prov_map();
    generate_vertex_prov_borders_map();

    std::cout << "Provinces generation done" << std::endl;
    return this->all_provinces;
}
