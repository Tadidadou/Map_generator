#include "utils.hpp"

bool isOK(int percent)
{
    return rand()%100 < percent;
}

RegionPoint::RegionPoint(int arg_x, int arg_y, int arg_id) : x(arg_x), y(arg_y), id(arg_id) {}

IterationResult::IterationResult() : changes(0) {}
void IterationResult::addRP(int x, int y, int id) {newRegionPoints.push_back(RegionPoint(x, y, id));}
