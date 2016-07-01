#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "province.hpp"

class Continent {
public:
    Continent(std::vector<Province>, int);
    ~Continent();

    void add_province(Province);

    std::string GetName() {
        return this->name;
    }
    sf::Color GetColor() {
        return this->color;
    }
    int GetId() {
        return this->id;
    }
    void SetName(std::string new_name) {
        this->name = new_name;
    }

private:
    int id;
    std::string name;
    sf::Color color;
    std::vector<Province> provinces;
};
