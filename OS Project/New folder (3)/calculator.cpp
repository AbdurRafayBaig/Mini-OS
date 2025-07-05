#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(300, 400), "Calculator");
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    sf::Text display("", font, 24);
    display.setFillColor(sf::Color::Black);
    display.setPosition(10, 10);

    std::string input = "", result = "";
    char operation = '\0';
    float first = 0;

    sf::RectangleShape buttons[16];
    sf::Text labels[16];
    std::string texts[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "C", "0", "=", "+"
    };

    for (int i = 0; i < 16; ++i) {
        buttons[i].setSize(sf::Vector2f(60, 50));
        buttons[i].setPosition(10 + (i % 4) * 70, 60 + (i / 4) * 60);
        buttons[i].setFillColor(sf::Color(200, 200, 200));

        labels[i].setFont(font);
        labels[i].setString(texts[i]);
        labels[i].setCharacterSize(20);
        labels[i].setFillColor(sf::Color::Black);
        labels[i].setPosition(buttons[i].getPosition().x + 20, buttons[i].getPosition().y + 10);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window));
                for (int i = 0; i < 16; ++i) {
                    if (buttons[i].getGlobalBounds().contains(mouse)) {
                        std::string label = texts[i];
                        if (label == "C") {
                            input = "";
                            result = "";
                            operation = '\0';
                            first = 0;
                        } else if (label == "+" || label == "-" || label == "*" || label == "/") {
                            first = std::stof(input);
                            operation = label[0];
                            input = "";
                        } else if (label == "=") {
                            float second = std::stof(input);
                            std::ostringstream oss;
                            switch (operation) {
                                case '+': oss << (first + second); break;
                                case '-': oss << (first - second); break;
                                case '*': oss << (first * second); break;
                                case '/': 
                                    if (second != 0)
                                        oss << (first / second);
                                    else
                                        oss << "Error";
                                    break;
                            }
                            input = oss.str();
                        } else {
                            input += label;
                        }
                        display.setString(input);
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(display);
        for (int i = 0; i < 16; ++i) {
            window.draw(buttons[i]);
            window.draw(labels[i]);
        }
        window.display();
    }

    return 0;
}

