#include "generator.hpp"
#include "constants.hpp"

using namespace std;

int main() {
    sf::VertexArray earth;
    sf::RenderWindow win(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Map generator", sf::Style::Close);
    sf::Texture texture;

    Generator generator(40.0);
    earth = generator.cylindric_map_generation();

    win.clear();
    win.draw(earth);
    win.display();

    while(win.isOpen()) {
        sf::Event event;
        while(win.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                win.close();
        }
    }
    return 0;
}