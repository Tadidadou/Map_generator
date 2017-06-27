#include "connexity.hpp"
#include <iostream>

IterationResult connexity(std::vector<std::vector<provinces_map> > pMap, int sharpness)
{
    IterationResult result;
    for (int x=0; x < WIN_WIDTH; ++x)
    {
        for (int y=1; y < WIN_HEIGHT-1; ++y)
        {
            provinces_map currentP = pMap[x][y];
            if (!currentP.num_prov && currentP.type != WATER)
            {
                if (pMap[mod((x+1), WIN_WIDTH)][y].num_prov && isOK(sharpness))
                {
                    result.changes++;
                    result.addRP(x, y, pMap[mod((x+1), WIN_WIDTH)][y].num_prov);
                }

                else if (pMap[mod((x-1), WIN_WIDTH)][y].num_prov && isOK(sharpness))
                {
                    result.changes++;
                    result.addRP(x, y, pMap[mod((x-1), WIN_WIDTH)][y].num_prov);
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
