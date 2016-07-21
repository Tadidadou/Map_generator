#include "province.hpp"


Province::Province(coord_terrain pixel, int id) {
    this->id = id;
    this->coords.push_back(pixel);
    this->color.r = rand()%255;
    this->color.g = rand()%255;
    this->color.b = rand()%255;
    this->name = search_name();
}


Province::~Province() {
    //Nothing to do and I don't write a cout because it's called a very amount of times
}


///Return a random name from a file
std::string Province::search_name() {
    int line;
    std::string name = "";

    line = rand();

    int i = 0;
    std::ifstream file("utils/name_list.txt");
    while (getline(file, name) && i < line)
    {
        i++;
    }
    return name;
}
