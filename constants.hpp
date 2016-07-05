#pragma once

int const WIN_WIDTH = 1600;
int const WIN_HEIGHT = 900;

enum terrain_type {
    WATER,
    GRASS,
    FOREST, //in prevision
    DIRT,
    HILL,
    MOUNTAIN
};

struct provinces_map {
    terrain_type type;
    int num_prov;
};
