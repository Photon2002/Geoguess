#include <SFML/Graphics.hpp>
#include <iostream>
#include "ClickableObject.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 1200 }), "SFML works!");
    sf::Color backgroundColour(135, 206, 235);
    
    //conturs
    sf::Texture conturTexture;
    if (!conturTexture.loadFromFile("assets/map/conturs2.png")) {
        std::cerr << "Error during loading map" << std::endl;
        return 1;
    }
    sf::Sprite contureSprite;
    contureSprite.setTexture(conturTexture);
    contureSprite.setScale(0.35f, 0.29f);
    contureSprite.setPosition(0,10);

    //countries data
    const int objectQuantity = 2;
    std::vector<ClickableObject> countries(objectQuantity);
    for (int i = 0; i < objectQuantity; i++) {
        countries[i].setData("textFiles/europe.txt", i);
    }




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(backgroundColour);
        window.draw(contureSprite);
        for (int i = 0; i < objectQuantity; i++) {
            countries[i].drawObject(window);
        }
        window.display();
    }
}