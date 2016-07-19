#include "map_cut.hpp"
#include "province.hpp"
#include "generator.hpp"
#include "constants.hpp"

using namespace std;

int main() {
    sf::VertexArray earth;
    sf::RenderWindow win(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Map generator", sf::Style::Close);
    win.setFramerateLimit(60);
    std::vector<Province> all_provinces;

    Generator generator(40.0);
    earth = generator.cylindric_map_generation();

    win.clear();
    win.draw(earth);
    win.display();

    Map_cut map_cut(earth, 500);
    all_provinces = map_cut.provinces_generation(generator.GetHeightMap(), generator.GetEarth_percent());

     for(int y=0; y < WIN_HEIGHT; y++) {
        for(int x=0; x < WIN_WIDTH; x++) {
            if(map_cut.prov_map[x][y].num_prov) {
                srand(map_cut.prov_map[x][y].num_prov);
                earth[y * WIN_WIDTH + x].position = sf::Vector2f(x, y);
                earth[y * WIN_WIDTH + x].color = sf::Color(rand()%255, rand()%255, rand()%255);
            }
        }
    }

    int offsetX = 0;
    double zoom = 1;
    sf::Vector2i clicPos;

    sf::Transform slide;

    while(win.isOpen()) {
        sf::Vector2i localPosition = sf::Mouse::getPosition(win);
        sf::Transform loopSlide;

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
                    clicPos = localPosition;
                    break;
                case sf::Event::MouseWheelScrolled:
                {
                    double zm = 1+event.mouseWheelScroll.delta/10.0;
                    zoom *= zm;
                    slide.scale(zm, zm);
                    //std::cout << zoom << std::endl;
                    break;
                }
                default:
                    break;
            }
        }

        //loopSlide.scale(zoom, zoom);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int move = localPosition.x-clicPos.x;
            int moveY = localPosition.y-clicPos.y;

            offsetX += move/zoom;

            if (offsetX > WIN_WIDTH)
            {
                offsetX -= WIN_WIDTH;
                slide.translate(sf::Vector2f(-WIN_WIDTH, 0));
            }
            else if (offsetX <= -WIN_WIDTH)
            {
                offsetX += WIN_WIDTH;
                slide.translate(sf::Vector2f(WIN_WIDTH, 0));
            }

            slide.translate(sf::Vector2f(move/zoom, moveY/zoom));

            clicPos = localPosition;

            std::cout << offsetX << std::endl;
        }

        loopSlide = slide;

        if (offsetX > 0)
            loopSlide.translate(sf::Vector2f(-WIN_WIDTH, 0));
        else
            loopSlide.translate(sf::Vector2f(WIN_WIDTH, 0));

        win.clear();

        win.draw(earth, slide);
        win.draw(earth, loopSlide);

        win.display();
    }
    return 0;
}
