#include "map_cut.hpp"
#include "province.hpp"
#include "generator.hpp"
#include "constants.hpp"

using namespace std;

int main() {
    sf::VertexArray earth;
    sf::RenderWindow win(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Map generator", sf::Style::Close);
    std::vector<Province> all_provinces;

    Generator generator(40.0);
    earth = generator.cylindric_map_generation();

    win.clear();
    win.draw(earth);
    win.display();

    cout << "Ok" << endl;
    Map_cut map_cut(earth, 20);
    all_provinces = map_cut.provinces_generation(generator.GetHeightMap(), generator.GetEarth_percent());

    for(int x=0; x < WIN_WIDTH; x++) {
        for(int y=0; y < WIN_HEIGHT; y++) {
            if(map_cut.prov_map[x][y].num_prov) {
                srand(map_cut.prov_map[x][y].num_prov);
                earth[x * WIN_HEIGHT + y].position = sf::Vector2f(x, y);
                earth[x * WIN_HEIGHT + y].color = sf::Color(rand()%255, rand()%255, rand()%255);
            }
        }
    }
    win.clear();
    win.draw(earth);
    win.display();

    while(win.isOpen()) {
        sf::Event event;
        while(win.pollEvent(event)) {
            ///Events management
            switch(event.type) {
                ///Window closed
                case sf::Event::Closed:
                    win.close();
                    break;
                ///One mouse button pressed
                case sf::Event::MouseButtonPressed:

                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}
