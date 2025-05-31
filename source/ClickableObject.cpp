#include "ClickableObject.h"
#include <fstream>
#include <sstream>

ClickableObject::ClickableObject() {
	objectName = "";
}

ClickableObject::~ClickableObject() {

}

void ClickableObject::setData(const std::string& fileName, int line) {
    
    std::ifstream file(fileName);
    std::string currentLine;
    int currentLineNumber = 0;

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return;
    }
    while (std::getline(file, currentLine)) {
        if (currentLineNumber == line) {
            std::istringstream iss(currentLine);
            std::string name, texturePath;
            iss >> name >> texturePath;
            objectName = name;
            if (!objectTexture.loadFromFile(texturePath)) {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
            else {
                objectSprite.setTexture(objectTexture);
            }
            break;
        }
        currentLineNumber++;
    }
    objectSprite.setTexture(objectTexture);
    objectSprite.setScale(0.35f, 0.29f);
    objectSprite.setPosition(0, 10);
}

void ClickableObject::drawObject(sf::RenderWindow& window) const {
    window.draw(objectSprite);
}

/*
bool ClickableObject::isClicked(sf::Vector2f mousePos) const {

}
*/
