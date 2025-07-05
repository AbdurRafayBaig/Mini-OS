#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

struct ProcessInfo {
    std::string name;
    int ramUsage;
};

void readSystemFile(int& usedRAM, int& totalRAM, int& processCount, std::vector<ProcessInfo>& processes) {
    std::ifstream file("system.txt");
    if (!file.is_open()) return;

    std::string line1, line2;
    if (std::getline(file, line1) && std::getline(file, line2)) {
        std::istringstream iss1(line1);
        iss1 >> usedRAM >> totalRAM >> processCount;

        processes.clear();
        std::istringstream iss2(line2);
        std::string entry;
        while (iss2 >> entry) {
            size_t colon = entry.find(':');
            if (colon != std::string::npos) {
                ProcessInfo p;
                p.name = entry.substr(0, colon);
                p.ramUsage = std::stoi(entry.substr(colon + 1));
                processes.push_back(p);
            }
        }
    }
    file.close();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 300), "System Monitor");

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Could not load font\n";
        return 1;
    }

    int usedRAM = 0, totalRAM = 0, processCount = 0;
    std::vector<ProcessInfo> processes;

    sf::Text ramText("", font, 16);
    ramText.setPosition(20, 20);
    ramText.setFillColor(sf::Color::White);

    sf::Text processText("", font, 14);
    processText.setPosition(20, 60);
    processText.setFillColor(sf::Color::White);

    sf::RectangleShape background(sf::Vector2f(400, 300));
    background.setFillColor(sf::Color(30, 30, 30));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            readSystemFile(usedRAM, totalRAM, processCount, processes);
            clock.restart();
        }

        std::ostringstream ramStream;
        ramStream << "RAM Usage: " << usedRAM << " / " << totalRAM << " MB";
        ramText.setString(ramStream.str());

        std::ostringstream procStream;
        procStream << "Running Processes (" << processCount << "):\n";
        for (const auto& p : processes) {
            procStream << "- " << p.name << " (" << p.ramUsage << "MB)\n";
        }
        processText.setString(procStream.str());

        window.clear();
        window.draw(background);
        window.draw(ramText);
        window.draw(processText);
        window.display();
    }

    return 0;
}

