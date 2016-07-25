#pragma once
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "constants.hpp"

class Province {
public:
    Province(coord_terrain pixel, int);
    ~Province();

    std::string GetName() {
        return this->name;
    }
    sf::Color GetColor() {
        return this->color;
    }
    int GetId() {
        return this->id;
    }
    terrain_type GetType() {
        return this->type;
    }
    void SetName(std::string new_name) {
        this->name = new_name;
    }
    void addPixel(coord_terrain pixel) {
        this->coords.push_back(pixel);
    }


    void determine_type();

private:
    std::string search_name();
    terrain_type type;
    int id;
    std::string name;
    sf::Color color;
    std::vector<coord_terrain> coords;
};
