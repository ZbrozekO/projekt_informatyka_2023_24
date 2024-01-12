#pragma once
#include "SFML/Graphics.hpp"

#define MAX_NUMBERS_OF_ITEMS 3

class Menu
{
public:
	Menu(float szerokosc, float wysokosc);
	~Menu();

	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBERS_OF_ITEMS];
};

