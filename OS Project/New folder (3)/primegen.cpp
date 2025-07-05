#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 500), "Prime Number Generator");

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Font load error\n";
        return 1;
    }

    sf::Text title("Generate Prime Numbers", font, 24);
    title.setPosition(200, 20);
    title.setFillColor(sf::Color::Black);

    sf::Text fromLabel("Start Number:", font, 18);
    fromLabel.setPosition(100, 100);
    fromLabel.setFillColor(sf::Color::Black);  // Black text

    sf::Text toLabel("End Number:", font, 18);
    toLabel.setPosition(100, 150);
    toLabel.setFillColor(sf::Color::Black);  // Black text

    sf::Text countLabel("How Many Primes:", font, 18);
    countLabel.setPosition(100, 200);
    countLabel.setFillColor(sf::Color::Black);  // Black text

    // Creating the input boxes with updated background colors
    sf::RectangleShape fromBox(sf::Vector2f(150, 30));
    fromBox.setPosition(280, 100);
    fromBox.setFillColor(sf::Color(255, 230, 230));  // Light red background
    fromBox.setOutlineThickness(2);
    fromBox.setOutlineColor(sf::Color(255, 100, 100));  // Dark red outline

    sf::RectangleShape toBox(sf::Vector2f(150, 30));
    toBox.setPosition(280, 150);
    toBox.setFillColor(sf::Color(230, 255, 230));  // Light green background
    toBox.setOutlineThickness(2);
    toBox.setOutlineColor(sf::Color(100, 255, 100));  // Dark green outline

    sf::RectangleShape countBox(sf::Vector2f(150, 30));
    countBox.setPosition(280, 200);
    countBox.setFillColor(sf::Color(230, 230, 255));  // Light blue background
    countBox.setOutlineThickness(2);
    countBox.setOutlineColor(sf::Color(100, 100, 255));  // Dark blue outline

    // Text inside input boxes (all text color set to black)
    sf::Text fromText("", font, 18);
    fromText.setPosition(285, 103);
    fromText.setFillColor(sf::Color::Black);  // Black text

    sf::Text toText("", font, 18);
    toText.setPosition(285, 153);
    toText.setFillColor(sf::Color::Black);  // Black text

    sf::Text countText("", font, 18);
    countText.setPosition(285, 203);
    countText.setFillColor(sf::Color::Black);  // Black text

    // Adding the "Generate" button
    sf::RectangleShape button(sf::Vector2f(140, 40));
    button.setPosition(280, 260);
    button.setFillColor(sf::Color(100, 200, 100));

    sf::Text buttonText("Generate", font, 18);
    buttonText.setPosition(305, 268);
    buttonText.setFillColor(sf::Color::Black);

    bool selectingFrom = false, selectingTo = false, selectingCount = false;
    std::string fromStr = "", toStr = "", countStr = "";
    bool showResults = false;

    std::vector<int> primes;
    sf::Text resultText("", font, 16);
    resultText.setPosition(100, 320);
    resultText.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);
                selectingFrom = fromBox.getGlobalBounds().contains(mouse.x, mouse.y);
                selectingTo = toBox.getGlobalBounds().contains(mouse.x, mouse.y);
                selectingCount = countBox.getGlobalBounds().contains(mouse.x, mouse.y);

                if (button.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    selectingFrom = selectingTo = selectingCount = false;
                    showResults = false;
                    primes.clear();

                    if (fromStr.empty() || toStr.empty() || countStr.empty()) continue;

                    int start = std::stoi(fromStr);
                    int end = std::stoi(toStr);
                    int count = std::stoi(countStr);

                    for (int i = start; i <= end; ++i) {
                        if (isPrime(i)) {
                            primes.push_back(i);
                            if ((int)primes.size() >= count) break;
                        }
                    }

                    std::ostringstream out;
                    if (primes.empty()) {
                        out << "No primes found in range.";
                    } else {
                        out << "Generated " << primes.size() << " primes:\n";
                        for (int p : primes) out << p << " ";
                    }

                    // Save to file
                    std::ofstream file("primes.txt");
                    if (file.is_open()) {
                        file << "Primes between " << start << " and " << end << " (up to " << count << "):\n";
                        for (int p : primes) file << p << " ";
                        file.close();
                    }

                    resultText.setString(out.str());
                    showResults = true;
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(event.text.unicode)) {
                    char ch = static_cast<char>(event.text.unicode);
                    if (selectingFrom && fromStr.size() < 6) fromStr += ch;
                    else if (selectingTo && toStr.size() < 6) toStr += ch;
                    else if (selectingCount && countStr.size() < 6) countStr += ch;
                } else if (event.text.unicode == 8) {
                    if (selectingFrom && !fromStr.empty()) fromStr.pop_back();
                    else if (selectingTo && !toStr.empty()) toStr.pop_back();
                    else if (selectingCount && !countStr.empty()) countStr.pop_back();
                }

                fromText.setString(fromStr);
                toText.setString(toStr);
                countText.setString(countStr);
            }
        }

        window.clear(sf::Color(240, 250, 255));  // Light blue background with soft gradient
        window.draw(title);
        window.draw(fromLabel); window.draw(toLabel); window.draw(countLabel);
        window.draw(fromBox); window.draw(toBox); window.draw(countBox);
        window.draw(fromText); window.draw(toText); window.draw(countText);
        window.draw(button); window.draw(buttonText);

        if (showResults) window.draw(resultText);

        window.display();
    }

    return 0;
}

