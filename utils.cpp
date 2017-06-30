#include "utils.hpp"

bool isOK(int percent)
{
    return rand()%100 < percent;
}

RegionPoint::RegionPoint(int arg_x, int arg_y, int arg_id) : x(arg_x), y(arg_y), id(arg_id) {}

IterationResult::IterationResult() : changes(0) {}
void IterationResult::addRP(int x, int y, int id) {newRegionPoints.push_back(RegionPoint(x, y, id));}

///New definition of mod (normal one doesn't work with negative numbers)
int mod(int a, int b) {
    if(a >= b)
        return a - b;
    else if(a < 0)
        return a + b;
    else
        return a;
}

/// Calculates the distance between two 2D-coordinates
float calculate_distance(sf::Vector2i a, sf::Vector2i b) {
    return sqrt((a.x - b.x)^2 + (a.y - b.y)^2);
}
