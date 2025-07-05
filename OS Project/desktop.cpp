// Modified desktop.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <map>

int totalRAM = 2048;
int totalHDD = 256;
int numCores = 4;

void loadSystemConfig() {
    std::ifstream config("config.txt");
    if (config.is_open()) {
        config >> totalRAM >> totalHDD >> numCores;
        config.close();
        std::cout << "Loaded System Config: RAM=" << totalRAM
                  << "MB, HDD=" << totalHDD << "GB, Cores=" << numCores << "\n";
    } else {
        std::cerr << "Warning: Could not open config.txt. Using default values.\n";
    }
}

std::map<std::string, std::string> appExecutables = {
    {"calculator", "./calculator"},
    {"clock", "./clock"},
    {"notepad", "./notes"}
};

std::map<std::string, pid_t> appPIDs;

void launchApp(const std::string &app, std::vector<std::string>& runningApps) {
    if (appPIDs.find(app) != appPIDs.end()) {
        // Check if process has already exited
        pid_t pid = appPIDs[app];
        if (waitpid(pid, nullptr, WNOHANG) == pid) {
            // Process has terminated, clean up tracking
            appPIDs.erase(app);
            runningApps.erase(std::remove(runningApps.begin(), runningApps.end(), app), runningApps.end());
        } else {
            std::cout << app << " is already running.\n";
            return;
        }
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed\n";
    } else if (pid == 0) {
        execl(appExecutables[app].c_str(), app.c_str(), nullptr);
        std::cerr << "Failed to launch " << app << "\n";
        exit(1);
    } else {
        runningApps.push_back(app);
        appPIDs[app] = pid;
        std::cout << "Launched " << app << " with PID: " << pid << "\n";
    }
}


void closeApp(const std::string& app, std::vector<std::string>& runningApps) {
    if (appPIDs.find(app) != appPIDs.end()) {
        kill(appPIDs[app], SIGTERM);
        waitpid(appPIDs[app], nullptr, 0);
        appPIDs.erase(app);
        runningApps.erase(std::remove(runningApps.begin(), runningApps.end(), app), runningApps.end());
        std::cout << "Closed " << app << "\n";
    }
}

int main() {
    loadSystemConfig();

    sf::RenderWindow window(sf::VideoMode(800, 600), "MyOS Desktop");

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    // Load Icons
    sf::Texture calcTexture, notesTexture, clockTexture;
    calcTexture.loadFromFile("icons/calculator.png");
    notesTexture.loadFromFile("icons/notepad.png");
    clockTexture.loadFromFile("icons/clock.png");

    sf::Sprite calcIcon(calcTexture), notesIcon(notesTexture), clockIcon(clockTexture);
    calcIcon.setScale(0.15f, 0.15f);
    notesIcon.setScale(0.15f, 0.15f);
    clockIcon.setScale(0.15f, 0.15f);

    calcIcon.setPosition(50, 100);
    notesIcon.setPosition(200, 100);
    clockIcon.setPosition(350, 100);

    sf::Text calcLabel("Calculator", font, 14);
    sf::Text notesLabel("Notepad", font, 14);
    sf::Text clockLabel("Clock", font, 14);

    calcLabel.setPosition(50, 190);
    notesLabel.setPosition(200, 190);
    clockLabel.setPosition(350, 190);

    sf::RectangleShape taskbar(sf::Vector2f(800, 40));
    taskbar.setPosition(0, 560);
    taskbar.setFillColor(sf::Color(50, 50, 50));

    std::vector<std::string> runningApps;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                if (calcIcon.getGlobalBounds().contains(mouse)) {
                    std::cout << "Launching Calculator\n";
                    launchApp("calculator", runningApps);
                } else if (notesIcon.getGlobalBounds().contains(mouse)) {
                    std::cout << "Launching Notepad\n";
                    launchApp("notepad", runningApps);
                } else if (clockIcon.getGlobalBounds().contains(mouse)) {
                    std::cout << "Launching Clock\n";
                    launchApp("clock", runningApps);
                }

                // Close app from taskbar with right click
                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (size_t i = 0; i < runningApps.size(); ++i) {
                        if (i * 60 < mouse.x && mouse.x < (i + 1) * 60 && 560 < mouse.y && mouse.y < 600) {
                            closeApp(runningApps[i], runningApps);
                            break;
                        }
                    }
                } else {
                    // Switch logic can be extended
                    for (size_t i = 0; i < runningApps.size(); ++i) {
                        if (i * 60 < mouse.x && mouse.x < (i + 1) * 60 && 560 < mouse.y && mouse.y < 600) {
                            std::cout << "Switching to " << runningApps[i] << "\n";
                        }
                    }
                }
            }
        }

        window.clear();
        for (int i = 0; i < 600; i += 2) {
            sf::RectangleShape line(sf::Vector2f(800, 2));
            line.setPosition(0, i);
            line.setFillColor(sf::Color(20 + i / 5, 20 + i / 5, 40 + i / 6));
            window.draw(line);
        }

        window.draw(calcIcon);
        window.draw(notesIcon);
        window.draw(clockIcon);
        window.draw(calcLabel);
        window.draw(notesLabel);
        window.draw(clockLabel);

        window.draw(taskbar);
        for (size_t i = 0; i < runningApps.size(); ++i) {
            sf::Text appLabel(runningApps[i], font, 14);
            appLabel.setPosition(i * 60 + 10, 565);
            appLabel.setFillColor(sf::Color::White);
            window.draw(appLabel);
        }

        window.display();
    }

    // Cleanup on close
    for (const auto& app : runningApps) {
        closeApp(app, runningApps);
    }

    return 0;
}

