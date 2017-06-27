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
    //Nothing to do and I don't write a cout because it's called a very big amount of times in a row
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


void Province::determine_type() {
    int cpt_grass = 0;
    int cpt_dirt = 0;
    int cpt_hill = 0;
    int cpt_mountain = 0;
    terrain_type intermed1, intermed2;
    for (int i = 0; i < coords.size(); i++) {
        if(coords[i].type == GRASS)
            cpt_grass++;
        else if(coords[i].type == DIRT)
            cpt_dirt++;
        else if(coords[i].type == HILL)
            cpt_hill++;
        else if(coords[i].type == MOUNTAIN)
            cpt_mountain++;
    }
    if(cpt_grass > cpt_dirt) {
        if(cpt_grass > cpt_hill) {
            if(cpt_grass > cpt_mountain)
                type = GRASS;
            else
                type = MOUNTAIN;
        }
        else {
            if(cpt_hill > cpt_mountain)
                type = HILL;
            else
                type = MOUNTAIN;
        }
    }
    else {
        if(cpt_dirt > cpt_hill) {
            if(cpt_dirt > cpt_mountain)
                type = DIRT;
            else
                type = MOUNTAIN;
        }
        else {
            if(cpt_hill > cpt_mountain)
                type = HILL;
            else
                type = MOUNTAIN;
        }
    }
}


void Province::determine_gc() {
    int sum_x = 0;
    int sum_y = 0;

    for(int i = 0; i < coords.size(); i++) {
        sum_x += coords[i].coord.x;
        sum_y += coords[i].coord.y;
    }

    this->gc.x = sum_x / coords.size();
    this->gc.y = sum_y / coords.size();
}
