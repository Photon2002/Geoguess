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

void resetGame(
    std::vector<Region>& regions,
    const std::vector<Region>& allRegions,
    sf::Image& mapMaskImage,
    const sf::Image& originalMask,
    sf::Texture& mapMaskTexture,
    std::string& name,
    int& points,
    bool& isGameFinished
){
    regions = allRegions;              // przywróć wszystkie regiony
    mapMaskImage = originalMask;       // przywróć czystą maskę
    mapMaskTexture.loadFromImage(mapMaskImage);

    points = 0;
    isGameFinished = false;

    if (!regions.empty())
        name = regions[rand() % regions.size()].name;
}

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
    bool isGameFinished = false;
    int points = 0;
    int maxPoints = 0;

    sf::RectangleShape rectangleBackground;
    rectangleBackground.setSize(sf::Vector2f(400, 60));
    rectangleBackground.setFillColor(sf::Color::White);
    rectangleBackground.setOutlineColor(sf::Color::Black);
    rectangleBackground.setOutlineThickness(2.f);
    rectangleBackground.setPosition(50, 50);


    sf::RectangleShape resultBox;
    resultBox.setSize(sf::Vector2f(400, 300));
    resultBox.setFillColor(sf::Color(255,255,255,230));
    resultBox.setOutlineColor(sf::Color::Black);
    resultBox.setOutlineThickness(2.f);
    resultBox.setPosition(800, 400);


    sf::RectangleShape retryBox;
    retryBox.setSize(sf::Vector2f(120, 50));
    retryBox.setFillColor(sf::Color(255,255,255,230));
    retryBox.setOutlineColor(sf::Color::Black);
    retryBox.setOutlineThickness(2.f);
    retryBox.setPosition(800, 600);


    sf::RectangleShape quitBox;
    quitBox.setSize(sf::Vector2f(120, 50));
    quitBox.setFillColor(sf::Color(255,255,255,230));
    quitBox.setOutlineColor(sf::Color::Black);
    quitBox.setOutlineThickness(2.f);
    quitBox.setPosition(1100, 600);

    float centerX = resultBox.getPosition().x + resultBox.getSize().x / 2.f;
    float bottomY = resultBox.getPosition().y + resultBox.getSize().y;

    // odstęp od dołu okna końcowego
    float buttonsY = bottomY - 70.f;

    // odstęp przycisków od środka
    float spacing = 80.f;

    retryBox.setPosition(
        centerX - retryBox.getSize().x - spacing/2.f,
        buttonsY
    );

    quitBox.setPosition(
        centerX + spacing/2.f,
        buttonsY
    );


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

    sf::Text endTitle, endScore;
    endTitle.setFont(font);
    endTitle.setString("KONIEC GRY");
    endTitle.setCharacterSize(48);
    endTitle.setFillColor(sf::Color::Black);
    endTitle.setStyle(sf::Text::Bold);

    endScore.setFont(font);
    endScore.setCharacterSize(32);
    endScore.setFillColor(sf::Color::Black);


    float cx = resultBox.getPosition().x + resultBox.getSize().x / 2.f;
    float topY = resultBox.getPosition().y;

    sf::FloatRect tRect = endTitle.getLocalBounds();
    endTitle.setOrigin(tRect.left + tRect.width/2.f, tRect.top + tRect.height/2.f);
    endTitle.setPosition(cx, topY + 70.f);

    sf::FloatRect sRect = endScore.getLocalBounds();
    endScore.setOrigin(sRect.left + sRect.width/2.f, sRect.top + sRect.height/2.f);
    endScore.setPosition(cx, topY + 150.f);

    sf::Text retryText, quitText;

    retryText.setFont(font);
    quitText.setFont(font);

    retryText.setCharacterSize(22);
    quitText.setCharacterSize(22);

    retryText.setFillColor(sf::Color::Black);
    quitText.setFillColor(sf::Color::Black);

    retryText.setString("Jeszcze raz");
    quitText.setString("Menu");

    auto centerText = [](sf::Text& text, const sf::RectangleShape& box)
    {
        sf::FloatRect r = text.getLocalBounds();
        text.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
        text.setPosition(
            box.getPosition().x + box.getSize().x/2.f,
            box.getPosition().y + box.getSize().y/2.f
        );
    };

    centerText(retryText, retryBox);
    centerText(quitText, quitBox);

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

    std::vector<Region> allRegions = regions;   // kopia pełnej listy
    sf::Image originalMask = mapMaskImage;     // czysta maska

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && isGameFinished)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mouse(sf::Mouse::getPosition(window));

                    if (retryBox.getGlobalBounds().contains(mouse))
                    {
                        resetGame(
                            regions,
                            allRegions,
                            mapMaskImage,
                            originalMask,
                            mapMaskTexture,
                            name,
                            points,
                            isGameFinished
                        );

                        continue;
                    }

                    if (quitBox.getGlobalBounds().contains(mouse))
                    {
                        window.close(); // na razie zamyka grę
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && !isGameFinished) {
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
                                            isGameFinished = true;
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
                                            isGameFinished = true;
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
        std::string endText =
        "Koniec Gry\n"
        "Twoj wynik: " + std::to_string(points) + "/" + std::to_string(maxPoints);

        guessRegion.setString(sf::String::fromUtf8(name.begin(), name.end()));
        pointsText.setString(sf::String::fromUtf8(result.begin(), result.end()));

        window.clear(backgroundColor);
        //window.draw(mapSprite);
        window.draw(mapMaskSprite);
        window.draw(rectangleBackground);
        window.draw(pointsBackground);
        window.draw(guessRegion);
        window.draw(pointsText);

        if(isGameFinished == true) {

            std::string scoreStr =
                "Twoj wynik: " + std::to_string(points) + " / " + std::to_string(maxPoints);

            endScore.setString(sf::String::fromUtf8(scoreStr.begin(), scoreStr.end()));

            sf::FloatRect sRect = endScore.getLocalBounds();
            endScore.setOrigin(sRect.left + sRect.width/2.f, sRect.top + sRect.height/2.f);
            endScore.setPosition(cx, topY + 150.f);

            window.draw(resultBox);
            window.draw(endTitle);
            window.draw(endScore);
            window.draw(retryBox);
            window.draw(quitBox);
            window.draw(retryText);
            window.draw(quitText);
        }
        window.display();
    }
    return 0;
}
