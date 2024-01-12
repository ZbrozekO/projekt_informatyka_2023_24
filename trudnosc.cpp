#include "trudnosc.h"


trudnosc::trudnosc(float szerokosc,float wysokosc) {
	if (!font.loadFromFile("arial_narrow_7.ttf")) {
		//error
	}
	trud[0].setFont(font);
	trud[0].setString("Latwy\nnacisnij klawisz 1");
	trud[0].setPosition(sf::Vector2f(szerokosc / 2  -50,70 + wysokosc/(maksymalna_liczba + 1)*1));

	trud[1].setFont(font);
	trud[1].setString("Sredni\nnacisnij klawisz 2");
	trud[1].setPosition(sf::Vector2f(szerokosc / 2 - 50, wysokosc/(maksymalna_liczba + 1) * 2));

	trud[2].setFont(font);
	trud[2].setString("Trudny\nnacisnij klawisz 3");
	trud[2].setPosition(sf::Vector2f(szerokosc/2 - 50, wysokosc/(maksymalna_liczba + 1) * 3 -60));
}

trudnosc::~trudnosc() {

}

void trudnosc::draw(sf::RenderWindow& window) {

	for (int i = 0; i < maksymalna_liczba; i++) {
		window.draw(trud[i]);
	}

}
