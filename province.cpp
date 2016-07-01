#include "province.hpp"


Province::Province(sf::VertexArray province_m, int id) {
    this->id = id;
    this->province_map = province_m;
    srand(time(0));
    this->color.r = rand()%255;
    this->color.g = rand()%255;
    this->color.b = rand()%255;
    this->name = "Ouarzazate";
}


Province::~Province() {
    std::cout << "Province has been one-shot" << std::endl;
}


///Add a vertex to an existing province
void Province::add_vertex(sf::Vertex vertice) {
    this->province_map.append(vertice);
}
