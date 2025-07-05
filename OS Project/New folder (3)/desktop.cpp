#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <csignal>

int totalRAM = 2048;
int usedRAM = 0;
int totalHDD = 256;
int numCores = 4;

struct Task {
    std::string name;
    std::string path;
    int ramUsage;
    pid_t pid;
};

std::map<std::string, std::string> appPaths = {
    {"calculator", "./calculator"},
    {"clock", "./clock"},
    {"notepad", "./notes"},
    {"monitor", "./monitor"},
    {"clicker", "./clicker"},
    {"primegen", "./primegen"}
};

std::map<std::string, int> appRAM = {
    {"calculator", 256},
    {"clock", 128},
    {"notepad", 512},
    {"monitor", 256},
    {"clicker", 128},
    {"primegen", 384}
};

std::map<std::string, pid_t> runningPIDs;
std::queue<Task> readyQueue;
std::vector<Task> activeTasks;
std::set<std::string> queuedTaskNames;
std::map<std::string, int> retryCount;

void updateSystemFile(const std::vector<Task>& tasks, int usedRAM, int totalRAM) {
    std::ofstream sysFile("system.txt");
    if (sysFile.is_open()) {
        sysFile << usedRAM << " " << totalRAM << " " << tasks.size() << "\n";
        for (const auto& task : tasks)
            sysFile << task.name << ":" << task.ramUsage << " ";
        sysFile << "\n";
        sysFile.close();
    }
}

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

bool scheduleTask(Task task) {
    if (usedRAM + task.ramUsage > totalRAM || (int)activeTasks.size() >= numCores) {
        if (queuedTaskNames.find(task.name) == queuedTaskNames.end()) {
            std::cout << "Queuing " << task.name << "\n";
            readyQueue.push(task);
            queuedTaskNames.insert(task.name);
        } else {
            std::cout << task.name << " is already queued.\n";
        }
        return false;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed for " << task.name << "\n";
        return false;
    } else if (pid == 0) {
        if (access(task.path.c_str(), X_OK) == 0) {
            execl(task.path.c_str(), task.name.c_str(), nullptr);
        }
        std::cerr << "Failed to launch " << task.name << ": executable not found or not executable\n";
        exit(1);
    } else {
        task.pid = pid;
        usedRAM += task.ramUsage;
        activeTasks.push_back(task);
        runningPIDs[task.name] = pid;
        std::cout << "Scheduled " << task.name << " with PID " << pid << "\n";
        updateSystemFile(activeTasks, usedRAM, totalRAM);
        return true;
    }
}

void closeTask(const std::string& name) {
    auto it = std::find_if(activeTasks.begin(), activeTasks.end(), [&](Task& t) { return t.name == name; });
    if (it != activeTasks.end()) {
        kill(it->pid, SIGTERM);
        waitpid(it->pid, nullptr, 0);
        usedRAM -= it->ramUsage;
        std::cout << "Closed " << name << ", RAM freed: " << it->ramUsage << "MB\n";
        runningPIDs.erase(name);
        queuedTaskNames.erase(name);
        activeTasks.erase(it);
        updateSystemFile(activeTasks, usedRAM, totalRAM);
    }
}

void trySchedulingFromQueue() {
    int attempts = readyQueue.size();
    while (attempts-- > 0) {
        Task task = readyQueue.front();
        readyQueue.pop();
        queuedTaskNames.erase(task.name);

        if (retryCount[task.name] >= 3) {
            std::cout << "[!] Max retries reached for " << task.name << ". Skipping.\n";
            continue;
        }

        if (scheduleTask(task)) {
            retryCount[task.name] = 0;
        } else {
            retryCount[task.name]++;
            readyQueue.push(task);
            queuedTaskNames.insert(task.name);
        }
    }
}

int main() {
    loadSystemConfig();

    sf::RenderWindow window(sf::VideoMode(800, 600), "MyOS Desktop");
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    // Load textures
    sf::Texture calcTexture, notesTexture, clockTexture, monitorTexture, clickerTexture, primegenTexture;
    calcTexture.loadFromFile("icons/calculator.png");
    notesTexture.loadFromFile("icons/notepad.png");
    clockTexture.loadFromFile("icons/clock.png");
    monitorTexture.loadFromFile("icons/monitor.png");
    clickerTexture.loadFromFile("icons/clicker.png");
    primegenTexture.loadFromFile("icons/primegen.png");

    // Sprites
    sf::Sprite calcIcon(calcTexture), notesIcon(notesTexture), clockIcon(clockTexture);
    sf::Sprite monitorIcon(monitorTexture), clickerIcon(clickerTexture), primegenIcon(primegenTexture);

    // Scaling
    calcIcon.setScale(0.15f, 0.15f);
    notesIcon.setScale(0.15f, 0.15f);
    clockIcon.setScale(0.15f, 0.15f);
    monitorIcon.setScale(0.15f, 0.15f);
    clickerIcon.setScale(0.15f, 0.15f);
    primegenIcon.setScale(0.15f, 0.15f);

    // Positioning
    calcIcon.setPosition(50, 100);
    notesIcon.setPosition(200, 100);
    clockIcon.setPosition(350, 100);
    monitorIcon.setPosition(500, 100);
    clickerIcon.setPosition(50, 250);
    primegenIcon.setPosition(200, 250);

    // Labels
    sf::Text calcLabel("Calculator", font, 14);
    sf::Text notesLabel("Notepad", font, 14);
    sf::Text clockLabel("Clock", font, 14);
    sf::Text monitorLabel("Monitor", font, 14);
    sf::Text clickerLabel("Clicker", font, 14);
    sf::Text primegenLabel("Prime Generator", font, 14);

    calcLabel.setPosition(50, 190);
    notesLabel.setPosition(200, 190);
    clockLabel.setPosition(350, 190);
    monitorLabel.setPosition(500, 190);
    clickerLabel.setPosition(50, 340);
    primegenLabel.setPosition(200, 340);

    sf::RectangleShape taskbar(sf::Vector2f(800, 40));
    taskbar.setPosition(0, 560);
    taskbar.setFillColor(sf::Color(50, 50, 50));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));

                if (calcIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"calculator", appPaths["calculator"], appRAM["calculator"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                } else if (notesIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"notepad", appPaths["notepad"], appRAM["notepad"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                } else if (clockIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"clock", appPaths["clock"], appRAM["clock"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                } else if (monitorIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"monitor", appPaths["monitor"], appRAM["monitor"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                } else if (clickerIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"clicker", appPaths["clicker"], appRAM["clicker"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                } else if (primegenIcon.getGlobalBounds().contains(mouse)) {
                    Task task = {"primegen", appPaths["primegen"], appRAM["primegen"]};
                    if (scheduleTask(task)) updateSystemFile(activeTasks, usedRAM, totalRAM);
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (size_t i = 0; i < activeTasks.size(); ++i) {
                        if (i * 60 < mouse.x && mouse.x < (i + 1) * 60 &&
                            560 < mouse.y && mouse.y < 600) {
                            closeTask(activeTasks[i].name);
                            trySchedulingFromQueue();
                            break;
                        }
                    }
                }
            }
        }

        for (auto it = activeTasks.begin(); it != activeTasks.end(); ) {
            if (waitpid(it->pid, nullptr, WNOHANG) == it->pid) {
                std::cout << "[" << it->name << "] Detected exit. RAM freed: " << it->ramUsage << " MB\n";
                usedRAM -= it->ramUsage;
                runningPIDs.erase(it->name);
                queuedTaskNames.erase(it->name);
                it = activeTasks.erase(it);
                trySchedulingFromQueue();
                updateSystemFile(activeTasks, usedRAM, totalRAM);
            } else {
                ++it;
            }
        }

        window.clear();
        window.draw(calcIcon); window.draw(notesIcon); window.draw(clockIcon);
        window.draw(monitorIcon); window.draw(clickerIcon); window.draw(primegenIcon);
        window.draw(calcLabel); window.draw(notesLabel); window.draw(clockLabel);
        window.draw(monitorLabel); window.draw(clickerLabel); window.draw(primegenLabel);
        window.draw(taskbar);

        for (size_t i = 0; i < activeTasks.size(); ++i) {
            sf::Text label(activeTasks[i].name, font, 14);
            label.setPosition(i * 60 + 10, 565);
            label.setFillColor(sf::Color::White);
            window.draw(label);
        }

        window.display();
    }

    for (const auto& task : activeTasks) closeTask(task.name);
    return 0;
}

