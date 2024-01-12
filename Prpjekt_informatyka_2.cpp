#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "trudnosc.h"
#include <fstream>

#define szerokosc_okna 712
#define wysokosc_okna 512
int score = 0;



class platforma {
public:
    //inicializowanie platformy i jej pozycja
    platforma() {
        platform.setSize(sf::Vector2f(szerokosc_platformy, wysokosc_platformy));
        platform.setPosition(szerokosc_okna / 2 - szerokosc_platformy / 2, 420);
        platform.setFillColor(sf::Color::Blue);
    }
    //poruszanie sie platforma
    void ruch_platformy() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && platform.getPosition().x - predkosc_platformy >= 0) {
            platform.move(-predkosc_platformy, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && platform.getPosition().x + szerokosc_platformy + predkosc_platformy <= szerokosc_okna - 200) {
            platform.move(predkosc_platformy, 0);
        }
    }
    //funkcja rysujaca platforme 
    void render_platform(sf::RenderWindow& window) const {
        window.draw(platform);
    }
    //inicjalizowanie granic platformy
    sf::FloatRect getBounds() const {
        return platform.getGlobalBounds();
    }
private:
    sf::RectangleShape platform;
    //wielkosc platformy
    int szerokosc_platformy = 120;
    int wysokosc_platformy = 15;
    //predkosc poruszania sie platformy 
    float predkosc_platformy = 5.0f;
};

class bloki {
public:
    int liczba_wierszy = 3;    // ilosc wierszy
    int liczba_cegielek_w_rzedzie = 10;  // liczba cegielek w rzedzie
    bloki() {
        cegielki.setSize(sf::Vector2f(szerokosc_cegielki, wysokosc_cegielki));
    }

    void pozycje_cegielek() {
        for (int i = 0; i < liczba_wierszy; i++) {
            for (int j = 0; j < liczba_cegielek_w_rzedzie; j++) {
                cegielki.setPosition(j * (szerokosc_cegielki + odstep_cegielki) + 10,
                    i * (wysokosc_cegielki + odstep_cegielki) + 20);
                cegielki_lista.push_back(cegielki);
            }
        }
    }

    void render_cegielki(sf::RenderWindow& window) const {
        for (const auto& cegielka : cegielki_lista) {
            window.draw(cegielka);
        }
    }

    sf::FloatRect getBounds(int index) const {
        return cegielki_lista[index].getGlobalBounds();
    }

    int getLiczbaCegielek() const {
        return cegielki_lista.size();
    }

    void removeCegielka(int index) {
        if (index >= 0 && index < cegielki_lista.size()) {
            cegielki_lista.erase(cegielki_lista.begin() + index);
        }
    }

    // Funkcja zapisująca stan cegiełek do pliku
    void zapiszStanCegielek() const {
        std::ofstream plik("stan_cegielek.txt");
        if (plik.is_open()) {
            for (const auto& cegielka : cegielki_lista) {
                plik << cegielka.getPosition().x << " " << cegielka.getPosition().y << "\n";
            }
            plik.close();
        }
    }

    // Funkcja wczytująca stan cegiełek z pliku
    void wczytajStanCegielek() {
        // Usuń wcześniej zapisane informacje o cegiełkach
        cegielki_lista.clear();

        std::ifstream plik("stan_cegielek.txt");
        if (plik.is_open()) {
            // Wczytaj pozycje cegiełek ze stanu z pliku
            float x, y;
            while (plik >> x >> y) {
                sf::RectangleShape cegielka;
                cegielka.setSize(sf::Vector2f(szerokosc_cegielki, wysokosc_cegielki));
                cegielka.setPosition(x, y);
                cegielki_lista.push_back(cegielka);
            }
            plik.close();
        }
    }


private:
    std::vector<sf::RectangleShape> cegielki_lista;
    sf::RectangleShape cegielki;
    int wysokosc_cegielki = 20;
    int odstep_cegielki = 10;
    float szerokosc_cegielki = (502 / liczba_cegielek_w_rzedzie) - odstep_cegielki;
};

class kula {
public:
    
    float predkosc_pilki_x = 0.0f;
    float predkosc_pilki_y = 0.0f;
    //inicjalizacja pilki i jej pozycja
    kula() {
        pilka.setRadius(promien);
        pilka.setPosition(490 / 2 - promien / 2, 350);
        pilka.setFillColor(sf::Color::Green);
    }

    // Ruch pilki
    void ruch_pilki() {
        pilka.move(predkosc_pilki_x, predkosc_pilki_y);
    }
    //renderowanie pilki na ekranie
    void render_kula(sf::RenderWindow& window) const {
        window.draw(pilka);
    }

    sf::FloatRect getBounds() const {
        return pilka.getGlobalBounds();
    }

    void kolizja_pilki_sciana() {
        //kolizja ze sciana wertykalna
        if (pilka.getPosition().x <= 0 || pilka.getPosition().x + 2 * promien >= 512) {
            predkosc_pilki_x = -predkosc_pilki_x;
        }
        //kolizja z gorna sciana
        if (pilka.getPosition().y - promien <= 0) {
            predkosc_pilki_y = -predkosc_pilki_y;
        }
    }
    //kolizja pilki z platforma
    void kolizja_pilka_platforma(const platforma& platform) {
        if (getBounds().intersects(platform.getBounds())) {
            // odwroc predkosc pionowa pilki
            predkosc_pilki_y = -predkosc_pilki_y;
        }
    }

    void kolizja_pilka_cegielka(bloki& cegielki, int& score) {
        for (int i = 0; i < cegielki.getLiczbaCegielek(); i++) {
            if (getBounds().intersects(cegielki.getBounds(i))) {
                predkosc_pilki_y = -predkosc_pilki_y;

                // Usuwanie sie klockow
                if (cegielki.getLiczbaCegielek() > 0) {
                    cegielki.removeCegielka(i);
                    score++;
                    break;
                }
            }
        }
    }

    void przegrana(sf::RenderWindow& window) {
        if (pilka.getPosition().y > 420) {
            window.close();
        }
    }

    enum KolorPiłki {
        ZIELONY,
        POMARAŃCZOWY,
        CZERWONY
    };

    void ustawKolorPiłki(sf::Color kolor) {
        pilka.setFillColor(kolor);
    }

    // Metoda do aktualizacji koloru piłki w zależności od poziomu trudności
    void aktualizujKolorPiłki(int dificulty_level) {
        switch (dificulty_level) {
        case 0:
            ustawKolorPiłki(sf::Color::Green);
            break;
        case 1:
            ustawKolorPiłki(sf::Color(255, 165, 0));  
            break;
        case 2:
            ustawKolorPiłki(sf::Color::Red);
            break;
        }
    }
    void zapiszStanGry() const {
        std::ofstream plik("stan_gry.txt");
        if (plik.is_open()) {
            plik << pilka.getPosition().x << " " << pilka.getPosition().y << " " << score;
            plik.close();
        }
    }

    // Funkcja wczytująca stan gry z pliku
    void wczytajStanGry() {
        std::ifstream plik("stan_gry.txt");
        if (plik.is_open()) {
            float x, y;
            int punkty;
            plik >> x >> y >> punkty;
            pilka.setPosition(x, y);
            score = punkty;
            plik.close();
        }
    }



private:
    sf::CircleShape pilka;
    //promin pilki
    int promien = 10;
    
};

void podzialka(sf::RenderWindow& window) {
    sf::RectangleShape linia_odzielajaca;
    linia_odzielajaca.setSize(sf::Vector2f(3, wysokosc_okna));
    linia_odzielajaca.setPosition(sf::Vector2f(512, 0));
    window.draw(linia_odzielajaca);
}

void drawScore(sf::RenderWindow& window, int score) {
    sf::Font font;
    if (!font.loadFromFile("arial_narrow_7.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(szerokosc_okna - 140, 30);

    window.draw(scoreText);
}

enum GameState {
    ROZGRYWKA,
    POMOC,
    POZIOM_TRUDNOSCI
};

bool confirmExit(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial_narrow_7.ttf")) {
        std::cerr << "Nie udalo sie wczytac fontu!" << std::endl;
        return false;
    }

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Jestes pewny ze chcesz wyjsc\nNacisnij Enter by zatwierdzic, F2 by anulowac");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(szerokosc_okna / 4, wysokosc_okna / 4);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    return true;  
                }
                else if (event.key.code == sf::Keyboard::F2) {
                    return false;  
                }
            }
        }

        window.clear();
        window.draw(exitText);
        window.display();
    }

    return false;  
}

class NieregularnyElement {
public:
    NieregularnyElement() {
        trapezoid.setPointCount(10);
        trapezoid.setPoint(0, sf::Vector2f(0,0));
        trapezoid.setPoint(1, sf::Vector2f(50, 0));
        trapezoid.setPoint(2, sf::Vector2f(25, 15));
        trapezoid.setPoint(3, sf::Vector2f(25, 30));
        trapezoid.setPoint(4, sf::Vector2f(13, 40));
        trapezoid.setPoint(5, sf::Vector2f(14, 40));
        trapezoid.setPoint(6, sf::Vector2f(15, 40));
        trapezoid.setPoint(7, sf::Vector2f(16, 40));
        trapezoid.setPoint(8, sf::Vector2f(17, 40));
        trapezoid.setPoint(9, sf::Vector2f(18, 40));
        trapezoid.setPoint(10, sf::Vector2f(19, 40));
        trapezoid.setPosition(szerokosc_okna-100, wysokosc_okna-60);  
        trapezoid.setFillColor(sf::Color::Green);
    }

    void render(sf::RenderWindow& window) const {
        window.draw(trapezoid);
    }

private:
    sf::ConvexShape trapezoid;  
};




int main() {
    sf::RenderWindow window(sf::VideoMode(szerokosc_okna, wysokosc_okna), "Arkanoid", sf::Style::Close | sf::Style::Resize);

    sf::Texture backgrounde;
    backgrounde.loadFromFile("tloo.jpg");
    sf::Sprite s(backgrounde);
    NieregularnyElement nieregularny_element;

    Menu menu(szerokosc_okna, wysokosc_okna);
    
    

    platforma postac;
    kula pilka;
    bloki cegielka;
    

    cegielka.pozycje_cegielek();

    int dificulty_level = 1;

    GameState stanGry = ROZGRYWKA;  

    

    while (window.isOpen()) {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            switch (evnt.type) {
            case sf::Event::KeyReleased:
                switch (evnt.key.code) {
                case sf::Keyboard::Escape:
                    if (confirmExit(window)) {
                        window.close();
                    }
                    break;
                case sf::Keyboard::S:
                    // Zapisz stan gry na żądanie użytkownika (np. po naciśnięciu klawisza 'S')
                    pilka.zapiszStanGry();
                    cegielka.zapiszStanCegielek();
                    break;
                case sf::Keyboard::L:
                    // Wczytaj stan gry na żądanie użytkownika (np. po naciśnięciu klawisza 'L')
                    pilka.wczytajStanGry();
                    cegielka.wczytajStanCegielek();
                    break;
                case sf::Keyboard::Up:
                    menu.MoveUp();
                    break;
                case sf::Keyboard::Down:
                    menu.MoveDown();
                    break;
                case sf::Keyboard::Return:
                    switch (menu.GetPressedItem()) {
                    case 0:
                        pilka.predkosc_pilki_x = 2.0f;
                        pilka.predkosc_pilki_y = 2.0f;
                        stanGry = ROZGRYWKA;  
                        break;
                    case 1:
                        stanGry = POZIOM_TRUDNOSCI;
                        break;
                    case 2:
                        window.close();
                        break;
                    }
                    break;  
                }
                break;  
            }

            if (evnt.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                stanGry = POMOC;  // zmien stan gry na pomoc
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
                stanGry = ROZGRYWKA;  // Zmien stan gry na rozgrywka
            }
        }

        window.clear();

        switch (stanGry) {
        case ROZGRYWKA:
            window.draw(s);
            menu.draw(window);
            postac.ruch_platformy();
            pilka.ruch_pilki();
            pilka.kolizja_pilka_platforma(postac);
            pilka.kolizja_pilki_sciana();
            pilka.kolizja_pilka_cegielka(cegielka, score);
            podzialka(window);
            cegielka.render_cegielki(window);
            pilka.render_kula(window);
            postac.render_platform(window);
            pilka.przegrana(window);
            drawScore(window, score);
            nieregularny_element.render(window);
            break;

        case POMOC:
            
        {
            sf::Font font;
            if (font.loadFromFile("arial_narrow_7.ttf")) {
                sf::Text Pomoc;
                Pomoc.setFont(font);
                Pomoc.setString("Sterowanie\n1.Sterujesz platforma uzywajac klawiszy A i D \n2.By wybrac pozycje w menu uzyj strzalek\n3.Po nacisnieciu klawisza S zapisujesz stan gry,\n by odczytac stan gry musisz nacisnac L \n4. Pod klawiszem ESC\n mozesz szybkow wyjsc z gry.\n5.By uruchomic gre wybierz\n w menu Graj i nacisnij Enter\nNacisnij F2 by wrocic do rozgrywki");
                Pomoc.setCharacterSize(24);
                Pomoc.setPosition(szerokosc_okna / 4, wysokosc_okna / 4);
                window.draw(Pomoc);
            }
        }
        break; 

        case POZIOM_TRUDNOSCI:
        {
            sf::Font font;
            if (font.loadFromFile("arial_narrow_7.ttf")) {
                sf::Text poziom_trudnosci;
                poziom_trudnosci.setFont(font);
                poziom_trudnosci.setString("Poziom Trudnosci\nNacisnij F2 by wrocic do rozgrywki\n1.Latwy\n2.Sredni\n3.Trudny\nNacisnij 1-2-3 by wybrac poziom trudnosci");
                poziom_trudnosci.setCharacterSize(24);
                poziom_trudnosci.setPosition(szerokosc_okna / 4, wysokosc_okna / 4);
                window.draw(poziom_trudnosci);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
                    pilka.predkosc_pilki_x = 2;
                    pilka.predkosc_pilki_y = 2;
                    pilka.aktualizujKolorPiłki(kula::ZIELONY);
                    stanGry = ROZGRYWKA;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
                    pilka.predkosc_pilki_x = 4;
                    pilka.predkosc_pilki_y = 4;
                    pilka.aktualizujKolorPiłki(kula::POMARAŃCZOWY);
                    stanGry = ROZGRYWKA;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) {
                    pilka.predkosc_pilki_x = 6;
                    pilka.predkosc_pilki_y = 6;
                    pilka.aktualizujKolorPiłki(kula::CZERWONY);
                    stanGry = ROZGRYWKA;
                }
                
            }
        }
        break;  
        }

        window.display();
    }

    return 0;
}