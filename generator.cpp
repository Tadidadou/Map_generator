#include "generator.hpp"
#include "constants.hpp"
#include "noiseutils.h"
#include <vector>


Generator::Generator() {
    this->earth_percent = 40.00;
    this->nations_number = 1;
    this->macro_nations_number = 1;
    this->techno_groups_number = 1;
    srand(time(0));
}


Generator::Generator(float earth_p, int nations_n, int macro_nations_n, int techno_groups_n) {
    if (earth_p >= 10.0 && earth_p < 80.0)
        this->earth_percent = earth_p;
    else {
        std::cout << "The earth-percentage can't be less than 10\% or more than 80\%, it has been set to 45\% as it is by default" << std::endl;
        this->earth_percent = 45.00;
    }
    this->nations_number = nations_n;
    this->macro_nations_number = macro_nations_n;
    this->techno_groups_number = techno_groups_n;
    srand(time(0));
}


Generator::~Generator() {
    std::cout << "Oh my god, generator is dying !" << std::endl;
}


///Totally random version of the map generator (first one)
sf::VertexArray Generator::random_map_generation() {
    int i, x, y, total_earth;
    sf::VertexArray earth_map(sf::Points, WIN_WIDTH * WIN_HEIGHT);

    x = y = 0;
    total_earth = (WIN_WIDTH * WIN_HEIGHT) * (this->earth_percent / 100);

    for(i=0; i < WIN_WIDTH * WIN_HEIGHT; i++) {
        earth_map[i].position = sf::Vector2f(x, y);
        if((total_earth > 0) && (rand()%100 + 1 < this->earth_percent)) {
            earth_map[i].color = sf::Color::Green;
            total_earth--;
        }
        else
            earth_map[i].color = sf::Color::Blue;
        y++;
        if(y == WIN_HEIGHT) {
            y = 0;
            x++;
        }
    }
    printf("Earth pixels still available : %d\n", total_earth);
    printf("Map generation done\n");
    return earth_map;
}


///Libnoise-based version of the map generator (third one)
sf::VertexArray Generator::libnoise_based_map_generation() {
    ///Different types of noises
    noise::module::Perlin perlin;
    noise::module::RidgedMulti moutainNoise;
    noise::module::Select finalTerrain;

    ///Other variables
    sf::VertexArray earth_map(sf::Points, WIN_WIDTH * WIN_HEIGHT);
    noise::utils::NoiseMap heightMap;
    noise::utils::NoiseMapBuilderPlane heightMapBuilder;
    RendererVertex renderer(earth_map);
    noise::utils::Image image;
    noise::utils::WriterBMP writer;
    int x, y, i;
    float shallow, shore, sand, grass, dirt, rock;

    x = rand()%1000000000;
    y = rand()%1000000000;
    std::cout << "Map coordinates : " << std::endl << "x = " << x << std::endl << "y = " << y << std::endl;

    ///Generating the terrain with the noise modules
    perlin.SetOctaveCount(10);
    perlin.SetPersistence(0.56);
    perlin.SetFrequency(0.9);
    moutainNoise.SetOctaveCount(10);
    moutainNoise.SetLacunarity(2);
    moutainNoise.SetFrequency(2);

    finalTerrain.SetSourceModule(0, perlin);
    finalTerrain.SetSourceModule(1, moutainNoise);
    finalTerrain.SetControlModule(perlin);
    finalTerrain.SetBounds(0.7 / ((earth_percent / 100) * 2), 1000);
    finalTerrain.SetEdgeFalloff(1);
    std::cout << "Moutain limit = " << 0.7 / ((earth_percent / 100) *2) << std::endl;

    ///Calculating the values of the gradient color
    shore = 1 - ((earth_percent / 100) * 2);
    shallow = shore - (0.25 * (earth_percent / 100) * 2);
    sand = shore + (0.0625 * (earth_percent / 100) * 2);
    grass = shore + (0.1250 * (earth_percent / 100) * 2);
    dirt = shore + (0.5000 * (earth_percent / 100) * 2);
    rock = shore + (0.7500 * (earth_percent / 100) * 2);
    std::cout << "shallow = " << shallow << std::endl;
    std::cout << "shore = " << shore << std::endl;
    std::cout << "sand = " << sand << std::endl;
    std::cout << "grass = " << grass << std::endl;
    std::cout << "dirt = " << dirt << std::endl;
    std::cout << "rock = " << rock << std::endl;

    ///Generating the height map
    heightMapBuilder.SetSourceModule(finalTerrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(WIN_WIDTH, WIN_HEIGHT);
    heightMapBuilder.SetBounds(x, x + 6.0, y, y + 3.375);
    heightMapBuilder.Build();
    std::cout << "Map building done" << std::endl;

    ///Generating the image and the vertex array
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
    renderer.AddGradientPoint(shallow, utils::Color(0, 0, 255, 255)); // shallow
    renderer.AddGradientPoint(shore, utils::Color(0, 128, 255, 255)); // shore
    renderer.AddGradientPoint(sand, utils::Color(240, 240, 64, 255)); // sand
    renderer.AddGradientPoint(grass, utils::Color(32, 160, 0, 255)); // grass
    renderer.AddGradientPoint(dirt, utils::Color(224, 224, 150, 255)); // dirt
    renderer.AddGradientPoint(rock, utils::Color(128, 128, 128, 255)); // rock
    renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(1.6);
    renderer.SetLightAzimuth(0.0);
    earth_map = renderer.Render();
    std::cout << "Image rendering done" << std::endl;

    ///Save the image of the map
    writer.SetSourceImage(image);
    writer.SetDestFilename("exemple3.bmp");
    writer.WriteDestFile();
    std::cout << "Image writing done" << std::endl;

    std::cout << "Map generation done" << std::endl;
    return earth_map;
}
