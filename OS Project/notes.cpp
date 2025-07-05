#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 400), "Notepad");
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    sf::Text text("", font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, 10);

    std::string content;
    bool saveRequested = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered) {
                char c = static_cast<char>(event.text.unicode);
                if (c == 8) { // Backspace
                    if (!content.empty()) content.pop_back();
                } else if (c == 13) { // Enter
                    content += '\n';
                } else if (c >= 32 && c <= 126) {
                    content += c;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::ofstream file("notepad.txt");
                file << content;
                file.close();
                std::cout << "File saved.\n";
            }
        }

        text.setString(content);
        window.clear(sf::Color::White);
        window.draw(text);
        window.display();
    }
    return 0;
}

