#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <unistd.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Music Player");
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    sf::Text title("Playing Beep Music...", font, 20);
    title.setPosition(50, 60);

    int beepCount = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= 2 && beepCount < 5) {
            std::cout << "\a";  // Beep (audible in terminal)
            beepCount++;
            clock.restart();
        }

        window.clear(sf::Color(230, 230, 250));
        window.draw(title);
        window.display();
    }

    return 0;
}

