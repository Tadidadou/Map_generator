#pragma once
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>

class Province {
public:
    Province(sf::VertexArray, int);
    ~Province();

    void add_vertex(sf::Vertex);

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
    sf::VertexArray province_map;
};
