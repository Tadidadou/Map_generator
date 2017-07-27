#pragma once
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "constants.hpp"
#include "utils.hpp"

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
    int GetPopulation() {
        return this->population;
    }
    int GetWealth() {
        return this->wealth;
    }
    int GetMajCulture() {
        return this->maj_culture_id;
    }
    int GetMajReligion() {
        return this->maj_religion_id;
    }
    std::vector<float> GetCultureRepartition() {
        return this->culture_percentages;
    }
    std::vector<float> GetReligionRepartition() {
        return this->religion_percentages;
    }
    void SetName(std::string new_name) {
        this->name = new_name;
    }
    void addNeighbour(Neighbour n) {
        this->neighbours.push_back(n);
    }
    void SetNeighbours(std::vector<Neighbour> neighbours) {
        this->neighbours = neighbours;
    }
    void addPixel(coord_terrain pixel) {
        this->coords.push_back(pixel);
    }
    int SetPopulation(int pop) {
        return this->population;
    }
    int SetWealth(int wealth) {
        return this->wealth;
    }

    void determine_type();
    void determine_gc();

private:
    std::string search_name();
    terrain_type type;
    int id;
    std::string name;
    sf::Color color;
    std::vector<coord_terrain> coords;
    sf::Vector2i gc;
    std::vector<Neighbour> neighbours;

    int population;
    int wealth;
    int maj_culture_id;
    std::vector<float> culture_percentages;
    int maj_religion_id;
    std::vector<float> religion_percentages;
};
