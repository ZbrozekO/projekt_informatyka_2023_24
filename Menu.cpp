#include "Menu.h"

Menu::Menu(float szerokosc, float wysokosc) {
	if (!font.loadFromFile("arial_narrow_7.ttf")) {
		//error
	}

	menu[0].setFont(font);
	menu[0].setString("Graj");
	menu[0].setPosition(sf::Vector2f(szerokosc - 120, wysokosc / (MAX_NUMBERS_OF_ITEMS + 1)));


	menu[1].setFont(font);
	menu[1].setString("Dificulty");
	menu[1].setPosition(sf::Vector2f(szerokosc - 150, wysokosc / (MAX_NUMBERS_OF_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setString("Wyjscie");
	menu[2].setPosition(sf::Vector2f(szerokosc - 145, wysokosc / (MAX_NUMBERS_OF_ITEMS + 1)*3));

}

Menu::~Menu() {

}

void Menu::draw(sf::RenderWindow& window) {
	for (int i = 0; i < MAX_NUMBERS_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}
}

void Menu::MoveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setCharacterSize(30);
		selectedItemIndex--;
		menu[selectedItemIndex].setCharacterSize(36);
	}
}

void Menu::MoveDown() {
	if (selectedItemIndex + 1 < MAX_NUMBERS_OF_ITEMS) {
		menu[selectedItemIndex].setCharacterSize(30);
		selectedItemIndex++;
		menu[selectedItemIndex].setCharacterSize(36);
	}
}