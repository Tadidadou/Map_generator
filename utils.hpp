#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <math.h>

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


int mod(int, int);
float calculate_distance(sf::Vector2i, sf::Vector2i);

#endif // UTILS_HPP_INCLUDED
