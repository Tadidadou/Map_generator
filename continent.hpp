#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "province.hpp"

class Continent {
private:
    Continent(std::vector<Province>);
    ~Continent();

    std::vector<Continent> generation();

public:
    int id;
    std::string name;
    sf::Color color;
    std::vector<Province> provinces;
};
