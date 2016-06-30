#include "province.hpp"
#include "constants.hpp"

Province::Province(sf::VertexArray earth_m) {
    this->earth_map = earth_m;
}


Province::~Province() {
    std::cout << "Province has been one-shot" << std::endl;
}


///Add a vertex to an existing province
bool Province::add_vertex(int id, sf::Vertex vertice) {
    bool stop = false;


    return stop;
}


///Create a new province
Province Province::create(int id) {
    Province new_province;
    int r, g, b;
    new_province.id = id;
    srand(time(0));
    r = rand()%255;
    g = rand()%255;
    b = rand()%255;
    new_province.color = sf::Color(r, g, b);
    new_province.name = "Ouarzazate";
    /// TODO ///
    return new_province;
}


///Generating all the province
std::vector<Province> Province::generation(float earth_percent) {
    std::vector<Province> all_provinces;
    sf::Vertex current;
    int i, cpt;

    for(i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        current = earth_map[i];
    }

    std::cout << "Provinces generation done" << std::endl;
    return all_provinces;
}
