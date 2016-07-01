#include "generator.hpp"
#include "constants.hpp"
#include "noiseutils.h"
#include <vector>


Generator::Generator() {
    this->earth_percent = 40.00;
}


Generator::Generator(float earth_p) {
    if (earth_p >= 20.0 && earth_p < 80.0)
        this->earth_percent = earth_p;
    else {
        std::cout << "The earth-percentage can't be less than 20\% or more than 80\%, it has been set to 40\% as it is by default" << std::endl;
        this->earth_percent = 40.00;
    }
}


Generator::~Generator() {
    std::cout << "Oh my god, generator is dying !" << std::endl;
}


///Rendering the map
sf::VertexArray Generator::terrain_rendering(sf::VertexArray earth_map) {
    RendererVertex renderer(earth_map);
    noise::utils::Image image;
    noise::utils::WriterBMP writer;
    float shore, shallow, sand, coast, grass, dirt, hill, rock;

    ///Calculating the values of the gradient color
    shore = 1 - ((earth_percent / 100) * 2);
    shallow = shore - (0.2500 * (earth_percent / 100) * 2);
    sand = shore + (0.02250 * (earth_percent / 100) * 2);
    coast = shore + (0.0750 * (earth_percent / 100) * 2);
    grass = shore + (0.1250 * (earth_percent / 100) * 2);
    dirt = shore + (0.2000 * (earth_percent / 100) * 2);
    hill = shore + (0.5000 * (earth_percent / 100) * 2);
    rock = shore + (0.7500 * (earth_percent / 100) * 2);
    std::cout << "shallow = " << shallow << std::endl;
    std::cout << "shore = " << shore << std::endl;
    std::cout << "sand = " << sand << std::endl;
    std::cout << "grass = " << grass << std::endl;
    std::cout << "dirt = " << dirt << std::endl;
    std::cout << "rock = " << rock << std::endl;

    ///Generating the image and the vertex array
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.0000, noise::utils::Color(0, 0, 128, 255)); // deeps
    renderer.AddGradientPoint(shallow, noise::utils::Color(0, 0, 255, 255)); // shallow
    renderer.AddGradientPoint(shore, noise::utils::Color(0, 128, 255, 255)); // shore
    renderer.AddGradientPoint(sand, noise::utils::Color(240, 240, 64, 255)); // sand
    renderer.AddGradientPoint(coast, noise ::utils::Color(93, 196, 70, 255)); // coast
    renderer.AddGradientPoint(grass, noise::utils::Color(100, 190, 0, 255)); // grass
    renderer.AddGradientPoint(dirt, noise::utils::Color(149, 213, 17, 255)); // dirt
    renderer.AddGradientPoint(hill, noise::utils::Color(224, 224, 150, 255)); // hill
    renderer.AddGradientPoint(rock, noise::utils::Color(128, 128, 128, 255)); // rock
    renderer.AddGradientPoint(1.0000, noise::utils::Color(255, 255, 255, 255)); // snow
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(1.6);
    renderer.SetLightAzimuth(0.0);
    earth_map = renderer.Render();
    std::cout << "Image rendering done" << std::endl;

    ///Save the image of the map
    writer.SetSourceImage(image);
    writer.SetDestFilename("Map.bmp");
    writer.WriteDestFile();
    std::cout << "Image writing done" << std::endl;

    return earth_map;
}


///Libnoise-based plane map generator
sf::VertexArray Generator::plane_map_generation() {
    ///Different types of noises
    noise::module::Perlin perlin;
    noise::module::RidgedMulti moutainNoise;
    noise::module::Select terrain;

    ///Other variables
    sf::VertexArray earth_map(sf::Points, WIN_WIDTH * WIN_HEIGHT);
    noise::utils::NoiseMapBuilderPlane heightMapBuilder;
    float x, y;

    srand(time(0));
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

    terrain.SetSourceModule(0, perlin);
    terrain.SetSourceModule(1, moutainNoise);
    terrain.SetControlModule(perlin);
    terrain.SetBounds(0.7 / ((earth_percent / 100) * 2), 1000);
    terrain.SetEdgeFalloff(1);
    std::cout << "Moutain limit = " << 0.7 / ((earth_percent / 100) *2) << std::endl;

    ///Generating the height map
    heightMapBuilder.SetSourceModule(terrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(WIN_WIDTH, WIN_HEIGHT);
    heightMapBuilder.SetBounds(x, x + 6.0, y, y + 3.375);
    heightMapBuilder.Build();
    std::cout << "Map building done" << std::endl;

    earth_map = terrain_rendering(earth_map);

    std::cout << "Map generation done" << std::endl;
    return earth_map;
}


///Libnoise-based cylindric map generator
sf::VertexArray Generator::cylindric_map_generation() {
    ///Noises variables
    noise::module::Perlin perlin;
    noise::module::RidgedMulti moutainNoise;
    noise::module::Select terrain;

    ///Other variables
    noise::utils::NoiseMapBuilderCylinder heightMapBuilder;
    sf::VertexArray earth_map(sf::Points, WIN_WIDTH * WIN_HEIGHT);
    RendererVertex renderer(earth_map);
    float y;

    srand(time(0));
    y = rand()%1000000000;
    std::cout << "y = " << y << std::endl;

    ///Generating the terrain with the noise modules
    perlin.SetOctaveCount(10);
    perlin.SetPersistence(0.56);
    perlin.SetFrequency(0.9);
    moutainNoise.SetOctaveCount(10);
    moutainNoise.SetLacunarity(2);
    moutainNoise.SetFrequency(2);

    terrain.SetSourceModule(0, perlin);
    terrain.SetSourceModule(1, moutainNoise);
    terrain.SetControlModule(perlin);
    terrain.SetBounds(0.7 / ((earth_percent / 100) * 2), 1000);
    terrain.SetEdgeFalloff(1);
    std::cout << "Moutain limit = " << 0.7 / ((earth_percent / 100) *2) << std::endl;

    ///Generating the height map
    heightMapBuilder.SetSourceModule(terrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(WIN_WIDTH, WIN_HEIGHT);
    heightMapBuilder.SetBounds(-180.0, 180.0, y, y + 3.5);
    heightMapBuilder.Build();
    std::cout << "Map building done" << std::endl;

    earth_map = terrain_rendering(earth_map);

    std::cout << "Map generation done" << std::endl;
    return earth_map;
}
