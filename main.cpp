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

    Map_cut map_cut(earth, 2000);
    all_provinces = map_cut.provinces_generation(generator.GetHeightMap(), generator.GetEarth_percent());

    int offsetX = 0;
    double zoom = 1;
    sf::Vector2i clicPos;

    sf::Transform slide;
    int mode = 0;

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
                    break;
                }
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Num1)
                        mode = 0;
                    if(event.key.code == sf::Keyboard::Num2)
                        mode = 1;
                    if(event.key.code == sf::Keyboard::Num3)
                        mode = 2;
                    break;
                default:
                    break;
            }
        }

        //loopSlide.scale(zoom, zoom);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
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
        }

        loopSlide = slide;

        if (offsetX > 0)
            loopSlide.translate(sf::Vector2f(-WIN_WIDTH, 0));
        else
            loopSlide.translate(sf::Vector2f(WIN_WIDTH, 0));

        win.clear();

        if(mode == 1) {
            win.draw(map_cut.GetProvincesBordersMap(), slide);
            win.draw(map_cut.GetProvincesBordersMap(), loopSlide);
        }
        else if(mode == 2) {
            win.draw(map_cut.GetProvincesMap(), slide);
            win.draw(map_cut.GetProvincesMap(), loopSlide);
        }
        else {
            win.draw(earth, slide);
            win.draw(earth, loopSlide);
        }

        win.display();
    }
    return 0;
}
