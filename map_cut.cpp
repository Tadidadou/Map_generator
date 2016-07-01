#include "map_cut.hpp"
#include "constants.hpp"


Map_cut::Map_cut(sf::VertexArray earth_m) {
    this->earth_map = earth_m;
}


Map_cut::~Map_cut() {
    std::cout << "They killed Map_cut !" << std::endl;
}


///Generating all the province
std::vector<Province> Map_cut::provinces_generation(noise::utils::NoiseMap heightMap) {
    noise::module::Perlin perlin;
    noise::utils::NoiseMap genMap;
    noise::utils::NoiseMapBuilderCylinder heightMapBuilder;
    int y, i;

    srand(time(0));
    y = rand()%35000;

///Generating the height map
    heightMapBuilder.SetSourceModule(perlin);
    heightMapBuilder.SetDestNoiseMap(genMap);
    heightMapBuilder.SetDestSize(WIN_WIDTH, WIN_HEIGHT);
    heightMapBuilder.SetBounds(-180.0, 180.0, y, y + 3.5);
    heightMapBuilder.Build();

    for(i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        std::cout << "Ok" << std::endl;
    }

    std::cout << "Provinces generation done" << std::endl;
    return this->all_provinces;
}
