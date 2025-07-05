#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>  // For execl() system call

bool isNumber(const std::string& str) {
    for (char c : str)
        if (!isdigit(c)) return false;
    return !str.empty();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 400), "Boot MyOS");

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    // Input boxes
    sf::RectangleShape ramBox(sf::Vector2f(200, 30));
    sf::RectangleShape hddBox(sf::Vector2f(200, 30));
    sf::RectangleShape coreBox(sf::Vector2f(200, 30));
    ramBox.setPosition(200, 80); hddBox.setPosition(200, 140); coreBox.setPosition(200, 200);
    ramBox.setFillColor(sf::Color::White);
    hddBox.setFillColor(sf::Color::White);
    coreBox.setFillColor(sf::Color::White);

    sf::Text ramLabel("RAM (MB):", font, 20);
    sf::Text hddLabel("Hard Drive (GB):", font, 20);
    sf::Text coreLabel("CPU Cores:", font, 20);
    ramLabel.setPosition(60, 80);
    hddLabel.setPosition(20, 140);
    coreLabel.setPosition(60, 200);

    sf::Text ramText("", font, 20), hddText("", font, 20), coreText("", font, 20);
    ramText.setPosition(205, 80); hddText.setPosition(205, 140); coreText.setPosition(205, 200);
    ramText.setFillColor(sf::Color::Black);
    hddText.setFillColor(sf::Color::Black);
    coreText.setFillColor(sf::Color::Black);

    std::string ramInput = "", hddInput = "", coreInput = "";
    enum InputField { NONE, RAM, HDD, CORE };
    InputField active = NONE;

    // Start button
    sf::RectangleShape startBtn(sf::Vector2f(150, 40));
    startBtn.setFillColor(sf::Color(100, 200, 100));
    startBtn.setPosition(225, 280);
    sf::Text startText("Start OS", font, 22);
    startText.setPosition(250, 285);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                if (ramBox.getGlobalBounds().contains(mouse)) active = RAM;
                else if (hddBox.getGlobalBounds().contains(mouse)) active = HDD;
                else if (coreBox.getGlobalBounds().contains(mouse)) active = CORE;
                else if (startBtn.getGlobalBounds().contains(mouse)) {
                    if (isNumber(ramInput) && isNumber(hddInput) && isNumber(coreInput)) {
                        int ram = std::stoi(ramInput);
                        int hdd = std::stoi(hddInput);
                        int cores = std::stoi(coreInput);

                        std::ofstream config("config.txt");
                        config << ram << "\n" << hdd << "\n" << cores << "\n";
                        config.close();

                        window.close();
                        // Instead of system, use execl to launch desktop
                        execl("./myos", "myos", nullptr);
                    } else {
                        std::cout << "Invalid input.\n";
                    }
                }
            }

            if (event.type == sf::Event::TextEntered) {
                char c = static_cast<char>(event.text.unicode);
                if (isdigit(c)) {
                    switch (active) {
                        case RAM: ramInput += c; break;
                        case HDD: hddInput += c; break;
                        case CORE: coreInput += c; break;
                        default: break;
                    }
                }
                if (c == 8) { // Backspace
                    switch (active) {
                        case RAM: if (!ramInput.empty()) ramInput.pop_back(); break;
                        case HDD: if (!hddInput.empty()) hddInput.pop_back(); break;
                        case CORE: if (!coreInput.empty()) coreInput.pop_back(); break;
                        default: break;
                    }
                }
            }
        }

        ramText.setString(ramInput);
        hddText.setString(hddInput);
        coreText.setString(coreInput);

        window.clear(sf::Color(220, 240, 220));
        window.draw(ramBox); window.draw(hddBox); window.draw(coreBox);
        window.draw(ramLabel); window.draw(hddLabel); window.draw(coreLabel);
        window.draw(ramText); window.draw(hddText); window.draw(coreText);
        window.draw(startBtn); window.draw(startText);
        window.display();
    }

    return 0;
}



