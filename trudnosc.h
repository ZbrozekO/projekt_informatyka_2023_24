#pragma once
#include "SFML/Graphics.hpp"

#define maksymalna_liczba 3 

class trudnosc
{
public:
	trudnosc(float szerokosc, float wysokosc);
	~trudnosc();

	void draw(sf::RenderWindow& window);
	void wybur_trudnosci();

private:
	sf::Font font;
	sf::Text trud[maksymalna_liczba];
};

