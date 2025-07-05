#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

void saveProgress(int score, int clickReward, int upgradeCost, int autoClickerCount, int autoClickerCost) {
    std::ofstream out("progress.txt");
    if (out.is_open()) {
        out << score << '\n'
            << clickReward << '\n'
            << upgradeCost << '\n'
            << autoClickerCount << '\n'
            << autoClickerCost << '\n';
    }
}

bool loadProgress(int& score, int& clickReward, int& upgradeCost, int& autoClickerCount, int& autoClickerCost) {
    std::ifstream in("progress.txt");
    if (in.is_open()) {
        in >> score >> clickReward >> upgradeCost >> autoClickerCount >> autoClickerCost;
        return true;
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 500), "Clicker Game");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Font load error\n";
        return 1;
    }

    // Game variables
    int score = 0, clickReward = 1, upgradeCost = 10, autoClickerCount = 0, autoClickerCost = 50;
    loadProgress(score, clickReward, upgradeCost, autoClickerCount, autoClickerCost);

    // UI Elements
    sf::Text title("Clicker Game", font, 24);
    title.setPosition(270, 20);
    title.setFillColor(sf::Color::Black);

    sf::Text scoreLabel("Score:", font, 18);
    scoreLabel.setPosition(100, 100);
    scoreLabel.setFillColor(sf::Color::Black);

    sf::Text scoreText(std::to_string(score), font, 18);
    scoreText.setPosition(200, 100);
    scoreText.setFillColor(sf::Color::Black);

    sf::Text clickLabel("Click Reward:", font, 18);
    clickLabel.setPosition(100, 150);
    clickLabel.setFillColor(sf::Color::Black);

    sf::Text clickText(std::to_string(clickReward), font, 18);
    clickText.setPosition(240, 150); // aligned with label
    clickText.setFillColor(sf::Color::Black);

    sf::RectangleShape clickButton(sf::Vector2f(140, 40));
    clickButton.setPosition(280, 200);
    clickButton.setFillColor(sf::Color(100, 200, 100));

    sf::Text clickButtonText("Click!", font, 18);
    clickButtonText.setPosition(305, 208);
    clickButtonText.setFillColor(sf::Color::Black);

    sf::RectangleShape resetButton(sf::Vector2f(140, 40));
    resetButton.setPosition(280, 260);
    resetButton.setFillColor(sf::Color(200, 100, 100));

    sf::Text resetButtonText("Reset", font, 18);
    resetButtonText.setPosition(330, 268);
    resetButtonText.setFillColor(sf::Color::Black);

    // Upgrade button
    sf::RectangleShape upgradeButton(sf::Vector2f(140, 40));
    upgradeButton.setPosition(280, 320);
    upgradeButton.setFillColor(sf::Color(100, 100, 250));

    sf::Text upgradeText("Upgrade", font, 18);
    upgradeText.setPosition(310, 328);
    upgradeText.setFillColor(sf::Color::White);

    sf::Text upgradeCostText("Cost: " + std::to_string(upgradeCost), font, 16);
    upgradeCostText.setPosition(430, 330);
    upgradeCostText.setFillColor(sf::Color::Black);

    // Auto clicker
    sf::RectangleShape autoClickerButton(sf::Vector2f(140, 40));
    autoClickerButton.setPosition(280, 380);
    autoClickerButton.setFillColor(sf::Color(255, 165, 0));

    sf::Text autoClickerText("Auto-Clicker", font, 18);
    autoClickerText.setPosition(290, 388);
    autoClickerText.setFillColor(sf::Color::Black);

    sf::Text autoClickerCostText("Cost: " + std::to_string(autoClickerCost), font, 16);
    autoClickerCostText.setPosition(430, 390);
    autoClickerCostText.setFillColor(sf::Color::Black);

    sf::Text autoClickerCountText("Auto-Clickers: " + std::to_string(autoClickerCount), font, 16);
    autoClickerCountText.setPosition(100, 200);
    autoClickerCountText.setFillColor(sf::Color::Black);

    sf::Clock autoClickClock;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                saveProgress(score, clickReward, upgradeCost, autoClickerCount, autoClickerCost);
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);

                // Click button
                if (clickButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    score += clickReward;
                    scoreText.setString(std::to_string(score));
                }

                // Reset
                if (resetButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    score = 0;
                    clickReward = 1;
                    upgradeCost = 10;
                    autoClickerCount = 0;
                    autoClickerCost = 50;
                    scoreText.setString("0");
                    clickText.setString("1");
                    upgradeCostText.setString("Cost: 10");
                    autoClickerCostText.setString("Cost: 50");
                    autoClickerCountText.setString("Auto-Clickers: 0");
                }

                // Upgrade
                if (upgradeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    if (score >= upgradeCost) {
                        score -= upgradeCost;
                        clickReward++;
                        upgradeCost += 10;
                        scoreText.setString(std::to_string(score));
                        clickText.setString(std::to_string(clickReward));
                        upgradeCostText.setString("Cost: " + std::to_string(upgradeCost));
                    }
                }

                // Auto-clicker
                if (autoClickerButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    if (score >= autoClickerCost) {
                        score -= autoClickerCost;
                        autoClickerCount++;
                        autoClickerCost += 20;
                        scoreText.setString(std::to_string(score));
                        autoClickerCountText.setString("Auto-Clickers: " + std::to_string(autoClickerCount));
                        autoClickerCostText.setString("Cost: " + std::to_string(autoClickerCost));
                    }
                }
            }
        }

        // Auto-clicking every second
        if (autoClickClock.getElapsedTime().asSeconds() >= 1.0f) {
            score += autoClickerCount;
            scoreText.setString(std::to_string(score));
            autoClickClock.restart();
        }

        window.clear(sf::Color(240, 250, 255));  // Background

        // Draw UI
        window.draw(title);
        window.draw(scoreLabel);
        window.draw(scoreText);
        window.draw(clickLabel);
        window.draw(clickText);
        window.draw(clickButton);
        window.draw(clickButtonText);
        window.draw(resetButton);
        window.draw(resetButtonText);
        window.draw(upgradeButton);
        window.draw(upgradeText);
        window.draw(upgradeCostText);
        window.draw(autoClickerButton);
        window.draw(autoClickerText);
        window.draw(autoClickerCostText);
        window.draw(autoClickerCountText);

        window.display();
    }

    return 0;
}

