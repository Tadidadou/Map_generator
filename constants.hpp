#pragma once

enum terrain_type {
    WATER,
    GRASS,
    FOREST, //in prevision
    DIRT,
    HILL,
    MOUNTAIN
};

struct Map_dimensions {
    int width;
    int height;
};
