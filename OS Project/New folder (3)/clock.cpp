#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <sstream>
#include <unistd.h>

const int RAM_USAGE_MB = 128;

int main() {
    std::cout << "[Clock] Starting. RAM usage: " << RAM_USAGE_MB << " MB\n";

    sf::RenderWindow window(sf::VideoMode(400, 400), "Clock");
    sf::CircleShape clockCircle(150);
    clockCircle.setFillColor(sf::Color::White);
    clockCircle.setPosition(50, 50);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(48);
    timeText.setFillColor(sf::Color::Blue);
    timeText.setPosition(90, 150);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::time_t currentTime = std::time(nullptr);
        std::tm* timeInfo = std::localtime(&currentTime);

        std::stringstream timeStream;
        timeStream << (timeInfo->tm_hour < 10 ? "0" : "") << timeInfo->tm_hour << ":"
                   << (timeInfo->tm_min < 10 ? "0" : "") << timeInfo->tm_min << ":"
                   << (timeInfo->tm_sec < 10 ? "0" : "") << timeInfo->tm_sec;

        timeText.setString(timeStream.str());

        window.clear();
        window.draw(clockCircle);
        window.draw(timeText);
        window.display();
    }

    std::cout << "[Clock] Exiting. RAM released: " << RAM_USAGE_MB << " MB\n";
    return 0;
}
