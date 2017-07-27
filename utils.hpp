#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <math.h>
#include "constants.hpp"

bool isOK(int percent);

struct RegionPoint
{
    int x, y, id;
    RegionPoint(int arg_x, int arg_y, int arg_id);
};

struct IterationResult
{
    int changes;
    std::vector<RegionPoint> newRegionPoints;

    IterationResult();

    void addRP(int x, int y, int id);
};

struct Neighbour
{
    int dest;
    float distance;
};

struct provinces_map {
    terrain_type type;
    int num_prov;
};

struct coord_terrain {
    terrain_type type;
    sf::Vector2f coord;
};


int mod(int, int);
float calculate_distance(sf::Vector2i, sf::Vector2i);

#endif // UTILS_HPP_INCLUDED
