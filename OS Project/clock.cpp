#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <sstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "Clock");

    // Create a clock shape (circle)
    sf::CircleShape clockCircle(150);
    clockCircle.setFillColor(sf::Color::White);
    clockCircle.setPosition(50, 50); // Positioning the circle in the center of the window

    // Create a text object to display the time
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(48);
    timeText.setFillColor(sf::Color::Blue);

    // Center the text
    timeText.setPosition(90, 150); // This can be adjusted based on the font size

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Get the current time
        std::time_t currentTime = std::time(nullptr);
        std::tm* timeInfo = std::localtime(&currentTime);

        // Format the time (HH:MM:SS)
        std::stringstream timeStream;
        timeStream << (timeInfo->tm_hour < 10 ? "0" : "") << timeInfo->tm_hour << ":"
                   << (timeInfo->tm_min < 10 ? "0" : "") << timeInfo->tm_min << ":"
                   << (timeInfo->tm_sec < 10 ? "0" : "") << timeInfo->tm_sec;

        timeText.setString(timeStream.str());

        window.clear();

        // Draw the clock background (circle) and time text
        window.draw(clockCircle);
        window.draw(timeText);

        window.display();
    }

    return 0;
}


