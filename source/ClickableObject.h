#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class ClickableObject
{
	public:
		ClickableObject();
		~ClickableObject();
		void setData(const std::string& fileName, int line); // add name and set texture file
		void drawObject(sf::RenderWindow& window) const;
		bool isClicked(sf::Vector2f mousePos) const;
	private:
		std::string objectName;
		sf::Texture objectTexture;
		sf::Sprite objectSprite;
		bool isChecked; // check if object was clicked

};