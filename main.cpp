#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

struct Region {
    std::string name;
    sf::Color color;
};

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(1920, 1080),"Geoguess");
    sf::Color backgroundColor(100,100,100);
    sf::Image mapImage;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    sf::Image mapMaskImage;
    sf::Texture mapMaskTexture;
    sf::Sprite mapMaskSprite;
    std::string name;
    std::string result;
    int points = 0;
    int maxPoints;

    sf::RectangleShape rectangleBackground;
    rectangleBackground.setSize(sf::Vector2f(400, 60));
    rectangleBackground.setFillColor(sf::Color::White);
    rectangleBackground.setOutlineColor(sf::Color::Black);
    rectangleBackground.setOutlineThickness(2.f);
    rectangleBackground.setPosition(50, 50);

    sf::RectangleShape pointsBackground;
    pointsBackground.setSize(sf::Vector2f(400, 60));
    pointsBackground.setFillColor(sf::Color::White);
    pointsBackground.setOutlineColor(sf::Color::Black);
    pointsBackground.setOutlineThickness(2.f);
    pointsBackground.setPosition(1500, 50);

    sf::Font font;
    if (!font.loadFromFile("../fonts/arial.ttf")) {
        std::cerr << "Cannot find font!" << std::endl;
        return -1;
    }
    sf::Text guessRegion;
    guessRegion.setFont(font);
    guessRegion.setCharacterSize(30);
    guessRegion.setFillColor(sf::Color::Black);

    sf::FloatRect textRect = guessRegion.getLocalBounds();
    guessRegion.setOrigin(
        textRect.left + textRect.width / 2.f,
        textRect.top + textRect.height / 2.f
    );
    guessRegion.setPosition(
    rectangleBackground.getPosition().x + rectangleBackground.getSize().x / 2.f - 190.f,
    rectangleBackground.getPosition().y + rectangleBackground.getSize().y / 2.f - 20.f // przesunięcie w górę
    );

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(30);
    pointsText.setFillColor(sf::Color::Black);

    sf::FloatRect pointTextRect = pointsText.getLocalBounds();
    pointsText.setOrigin(
        pointTextRect.left + pointTextRect.width / 2.f,
        pointTextRect.top + pointTextRect.height / 2.f
    );
    pointsText.setPosition(
    pointsBackground.getPosition().x + pointsBackground.getSize().x / 2.f - 100.f,
    pointsBackground.getPosition().y + pointsBackground.getSize().y / 2.f - 20.f // przesunięcie w górę
    );


    std::vector<Region> regions;
    std::ifstream regionsTXT("../textFiles/Wojewodztwa.txt");
    std::string reading;
    if (!regionsTXT.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }
    while(std::getline(regionsTXT,reading)) {
        std::istringstream iss(reading);
        std::string regionName;
        int r;
        int g;
        int b;
        if (!(iss >> regionName >> r >> g >> b)) {
            std::cerr << "Niepoprawny format linii: " << reading << std::endl;
            continue;
        }
        regions.push_back({regionName,sf::Color(r,g,b)});
    }

    window.setFramerateLimit(60);
    if(!mapImage.loadFromFile("../assets/poland_map.png")) {
        std::cerr << "Failed to load map image" << std::endl;
        return -1;
    }
    if(!mapMaskImage.loadFromFile("../assets/poland_mask.png")) {
        std::cerr << "Failed to load map mask image" << std::endl;
        return -1;
    }

    mapTexture.loadFromImage(mapImage);
    mapSprite.setTexture(mapTexture);
    mapMaskTexture.loadFromImage(mapMaskImage);
    mapMaskSprite.setTexture(mapMaskTexture);


    mapMaskSprite.setOrigin(static_cast<float>(mapMaskImage.getSize().x) / 2.f, static_cast<float>(mapMaskImage.getSize().y) / 2.f);
    mapMaskSprite.setPosition(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f);

    maxPoints = regions.size();
    name = regions[rand() % regions.size()].name;

    while (window.isOpen()) {


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f localPos = mapMaskSprite.getInverseTransform().transformPoint(static_cast<sf::Vector2f>(mousePos));
                    if(0 <= localPos.x && localPos.x < mapImage.getSize().x) {
                        if(0 <= localPos.y  && localPos.y < mapImage.getSize().y) {
                            sf::Color clickColor = mapImage.getPixel(localPos.x,localPos.y);
                            for(int i=0;i<regions.size();i++) {
                                if(regions[i].name == name) {
                                    if(regions[i].color == clickColor) {
                                        std::cout << "Poprawna odpowiedz!" << std::endl;

                                        sf::Color regionColor = regions[i].color;
                                        regions.erase(regions.begin() + i);
                                        points++;
                                        for (unsigned int x = 0; x < mapImage.getSize().x; x++) {
                                            for (unsigned int y = 0; y < mapImage.getSize().y; y++) {

                                                if (mapImage.getPixel(x, y) == regionColor) {
                                                    mapMaskImage.setPixel(x, y, sf::Color::Green);
                                                }

                                            }
                                        }

                                        mapMaskTexture.loadFromImage(mapMaskImage);

                                        if (!regions.empty()) {
                                            name = regions[rand() % regions.size()].name;
                                        }
                                        else {
                                            std::cout << "Wszystkie regiony zostaly odgadniete!" << std::endl;
                                            window.close();
                                            break;
                                        }
                                    }
                                    else {
                                        std::cout << "Niepoprawna odpowiedz!" << std::endl;
                                        sf::Color regionColor = regions[i].color;
                                        regions.erase(regions.begin() + i);

                                        for (unsigned int x = 0; x < mapImage.getSize().x; x++) {
                                            for (unsigned int y = 0; y < mapImage.getSize().y; y++) {

                                                if (mapImage.getPixel(x, y) == regionColor) {
                                                    mapMaskImage.setPixel(x, y, sf::Color::Red);
                                                }

                                            }
                                        }
                                        mapMaskTexture.loadFromImage(mapMaskImage);
                                        if (!regions.empty()) {
                                            name = regions[rand() % regions.size()].name;
                                        }
                                        else {
                                            std::cout << "Wszystkie regiony zostaly odgadniete!" << std::endl;
                                            window.close();
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        result = "Wynik: " + std::to_string(points) + "/" + std::to_string(maxPoints);
        guessRegion.setString(sf::String::fromUtf8(name.begin(), name.end()));
        pointsText.setString(sf::String::fromUtf8(result.begin(), result.end()));
        window.clear(backgroundColor);
        //window.draw(mapSprite);
        window.draw(mapMaskSprite);
        window.draw(rectangleBackground);
        window.draw(pointsBackground);
        window.draw(guessRegion);
        window.draw(pointsText);
        window.display();
    }
    return 0;
}
