#include "continent.hpp"


Continent::Continent(std::vector<Province> continent_m, int id) {
    this->id = id;
    this->provinces = continent_m;
    srand(time(0));
    this->color.r = rand()%255;
    this->color.g = rand()%255;
    this->color.b = rand()%255;
    this->name = "Ouarzazate";
}


Continent::~Continent() {
    std::cout << "continent has been one-shot" << std::endl;
}

///Add a vertex to an existing continent
void Continent::add_province(Province province) {
    this->provinces.push_back(province);
}
