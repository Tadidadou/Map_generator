#pragma once
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "neighbour.hpp"
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
    std::vector<coord_terrain> GetCoords() {
        return this->coords;
    }
    terrain_type GetType() {
        return this->type;
    }
    sf::Vector2i GetGc() {
        return this->gc;
    }
    std::vector<Neighbour> GetNeighbours() {
        return this->neighbours;
    }
    void SetName(std::string new_name) {
        this->name = new_name;
    }
    void addNeighbour(Neighbour n) {
        this->neighbours.push_back(n);
    }
    void addPixel(coord_terrain pixel) {
        this->coords.push_back(pixel);
    }


    void determine_type();
    void determine_gc();
    void determine_neihbours(std::vector<std::vector<provinces_map> >);

private:
    std::string search_name();
    terrain_type type;
    int id;
    std::string name;
    sf::Color color;
    std::vector<coord_terrain> coords;
    sf::Vector2i gc;
    std::vector<Neighbour> neighbours;
};
