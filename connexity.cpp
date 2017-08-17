#include "connexity.hpp"
#include <iostream>

IterationResult connexity(std::vector<std::vector<provinces_map> > pMap, int sharpness, Map_dimensions map_dimensions)
{
    IterationResult result;
    for (int x=0; x < map_dimensions.width; ++x)
    {
        for (int y=1; y < map_dimensions.height-1; ++y)
        {
            provinces_map currentP = pMap[x][y];
            if (!currentP.num_prov && currentP.type != WATER)
            {
                if (pMap[mod((x+1), map_dimensions.width)][y].num_prov && isOK(sharpness))
                {
                    result.changes++;
                    result.addRP(x, y, pMap[mod((x+1), map_dimensions.width)][y].num_prov);
                }

                else if (pMap[mod((x-1), map_dimensions.width)][y].num_prov && isOK(sharpness))
                {
                    result.changes++;
                    result.addRP(x, y, pMap[mod((x-1), map_dimensions.width)][y].num_prov);
                }

                else if (pMap[x][y+1].num_prov && isOK(sharpness))
                {
                   result.changes++;
                   result.addRP(x, y, pMap[x][y+1].num_prov);
                }

                else if (pMap[x][y-1].num_prov && isOK(sharpness))
                {
                    result.changes++;
                    result.addRP(x, y, pMap[x][y-1].num_prov);
                }
            }
        }
    }

    return result;
}
