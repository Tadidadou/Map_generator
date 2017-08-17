#include "map_cut.hpp"


Map_cut::Map_cut(sf::VertexArray earth_m, int nb_prov, Map_dimensions dim) {
    this->earth_map = earth_m;
    this->map_dimensions.width = dim.width;
    this->map_dimensions.height = dim.height;
    this->nb_prov = nb_prov;
    this->prov_map.resize(map_dimensions.width);
    for(auto & v : this->prov_map)
        v.resize(map_dimensions.height);
}


Map_cut::~Map_cut() {
    std::cout << "They killed Map_cut !" << std::endl;
}


///Generating the vertex array of the provinces
sf::VertexArray Map_cut::generate_vertex_prov_map() {
    int x = 0;
    int y = 0;
    vertex_prov_map = sf::VertexArray(sf::Points, map_dimensions.width * map_dimensions.height);

    for(unsigned int i=0; i < map_dimensions.width * map_dimensions.height; i++) {
        vertex_prov_map[i].position = sf::Vector2f(x, y);
        if(prov_map[x][y].type != WATER)
            vertex_prov_map[i].color = all_provinces[prov_map[x][y].num_prov].GetColor();
        else
            vertex_prov_map[i].color = earth_map[i].color;

        x++;
        if(x == map_dimensions.width) {
            y++;
            x = 0;
        }
    }

    return vertex_prov_map;
}


/// If the pixel is on a boarder between provinces, return the id of the connected province
int Map_cut::isBorder(int x, int y) {
    if(y != 0) {
        if(prov_map[x][y].num_prov != prov_map[x][y-1].num_prov)
            return prov_map[x][y-1].num_prov;
    }
    if(y != map_dimensions.height - 1) {
        if(prov_map[x][y].num_prov != prov_map[x][y+1].num_prov)
            return prov_map[x][y+1].num_prov;
    }
    if(x == 0) {
        if(prov_map[x][y].num_prov != prov_map[map_dimensions.width-1][y].num_prov)
            return prov_map[map_dimensions.width-1][y].num_prov;
    }
    // Problem with the modulo operation : seems like (-1 mod map_dimensions.width) == -1 ==> bug
    /*if(prov_map[x][y].num_prov != prov_map[(x-1)%map_dimensions.width][y].num_prov)
            return true;*/
    if(prov_map[x][y].num_prov != prov_map[(x+1)%map_dimensions.width][y].num_prov)
        return prov_map[(x+1)%map_dimensions.width][y].num_prov;

    return -1;
}


/// Return true if the pixel is the center of gravity of the province - debug use
/*bool Map_cut::isGc(int x, int y) {
    int id = prov_map[x][y].num_prov;
    if((x == all_provinces[id].GetGc().x) && (y == all_provinces[id].GetGc().y)){
        std::cout << "GC found !" << std::endl;
        return true;
    }

    return false;
}*/


/// Generating the vertex array of the naked map with boarders
sf::VertexArray Map_cut::generate_vertex_prov_borders_map() {
    int x = 0;
    int y = 0;
    vertex_prov_borders_map = sf::VertexArray(sf::Points, map_dimensions.width * map_dimensions.height);

    for(int i=0; i < map_dimensions.width * map_dimensions.height; i++) {
        vertex_prov_borders_map[i].position = sf::Vector2f(x, y);

        if(prov_map[x][y].type != WATER && isBorder(x, y) > 0)
            vertex_prov_borders_map[i].color = sf::Color(0, 0, 0);
        else
            vertex_prov_borders_map[i].color = earth_map[i].color;

        x++;
        if(x == map_dimensions.width) {
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
    sf::VertexArray result_map(sf::Points, map_dimensions.width * map_dimensions.height);

    for(unsigned int i=0; i < map_dimensions.width * map_dimensions.height; i++) {
        result_map[i].position = sf::Vector2f(x, y);
        if(prov_map[x][y].num_prov == id_prov)
            result_map[i].color = all_provinces[prov_map[x][y].num_prov].GetColor();
        else
            result_map[i].color = earth_map[i].color;

        x++;
        if(x == map_dimensions.width) {
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

    for(x=0; x < map_dimensions.width; x++) {
        for(y=0; y < map_dimensions.height; y++) {
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


/// Determine the neighbours of each province
std::vector<Neighbour> Map_cut::determine_neighbours(int id_prov) {
    Province current_prov = all_provinces[id_prov];
    std::vector<coord_terrain> coords = current_prov.GetCoords();
    int destination;
    std::vector<Neighbour> current_neighbours;
    bool flag;

    for(int i=0; i < coords.size(); i++) {
        //std::cout << "Ok" << std::endl;
        destination = isBorder(coords[i].coord.x, coords[i].coord.y);
        if(destination > 0) {
            flag = false;

            for(int j=0; j < current_neighbours.size(); j++) {
                if(current_neighbours[j].dest == destination) {
                    flag = true;
                    break;
                }
            }

            if(!flag) {
                Neighbour n;
                n.dest = destination;
                n.distance = calculate_distance(current_prov.GetGc(), all_provinces[destination].GetGc(), map_dimensions);
                current_neighbours.push_back(n);
            }
        }
    }

    /*for(int i = 0; i < current_neighbours.size(); i++) {
        std::cout << "Prov num " << current_neighbours[i].dest << std::endl;
    }*/
    return current_neighbours;
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
            x = rand()%map_dimensions.width;
            y = rand()%map_dimensions.height;
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
            iterationResult = connexity(prov_map, 50, map_dimensions);

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
        for(x=0; x<map_dimensions.width; x++) {
            for(y=0; y<map_dimensions.height; y++) {
                if(prov_map[x][y].type != WATER && prov_map[x][y].num_prov == 0) {
                    remaining_earth++;
                }
            }
        }

        nb = remaining_earth / 1000;
        if(nb == 0) {
            changes = false;
        }
    }
    std::cout << std::endl << "Number of loops : " << n_loop << std::endl;

    for(i = 0; i < all_provinces.size(); i++) {
        all_provinces[i].determine_type();
        all_provinces[i].determine_gc();
    }
    generate_vertex_prov_map();
    generate_vertex_prov_borders_map();

    std::cout << "Provinces generation done" << std::endl;

    std::cout << "Neighbours determination..." << std::endl;
    for(int i=0; i < all_provinces.size() - 1; i++) {
        all_provinces[i+1].SetNeighbours(determine_neighbours(i+1));
    }
    std::cout << "Neighbours determination done" << std::endl;
    return this->all_provinces;
}
