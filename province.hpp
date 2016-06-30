#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Province {
public:
    Province(sf::VertexArray);
    ~Province();

    std::vector<Province> generation(float);

private:
    Province create(int);
    bool add_vertex(int, sf::Vertex);

    int id;
    std::string name;
    sf::Color color;
    sf::VertexArray earth_map;
};
