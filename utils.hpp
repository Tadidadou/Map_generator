#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <vector>
#include <cstdlib>

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

int mod(int, int);

#endif // UTILS_HPP_INCLUDED
