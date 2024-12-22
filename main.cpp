#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>

class Menu {
private:
    sf::RenderWindow& window;
    sf::RectangleShape startButton;
    sf::RectangleShape descriptionButton;
    sf::RectangleShape historyButton;
    sf::Font font;
    sf::Text startText;
    sf::Text descriptionText;
    sf::Text historyText;
    bool startSelected = false;
    bool descriptionSelected = false;
public:
    Menu(sf::RenderWindow& win) : window(win) {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Unable to load font");
        }
        startButton.setSize({ 220.0f, 80.0f });
        startButton.setFillColor(sf::Color::Cyan);
        startButton.setPosition(300.0f, 200.0f);
        startButton.setOutlineThickness(5.0f);
        startButton.setOutlineColor(sf::Color::Black);

        descriptionButton.setSize({ 220.0f, 80.0f });
        descriptionButton.setFillColor(sf::Color::Magenta);
        descriptionButton.setPosition(300.0f, 300.0f);
        descriptionButton.setOutlineThickness(5.0f);
        descriptionButton.setOutlineColor(sf::Color::Black);
        historyButton.setSize({ 220.0f, 80.0f });
        historyButton.setFillColor(sf::Color::Blue);
        historyButton.setPosition(300.0f, 400.0f);
        historyButton.setOutlineThickness(5.0f);
        historyButton.setOutlineColor(sf::Color::Black);

        startText.setFont(font);
        startText.setString("Start");
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::Black);
        startText.setPosition(
            startButton.getPosition().x + (startButton.getSize().x - startText.getLocalBounds().width) / 2,
            startButton.getPosition().y + (startButton.getSize().y - startText.getLocalBounds().height) / 2 - 5);

        descriptionText.setFont(font);
        descriptionText.setString("Opis");
        descriptionText.setCharacterSize(24);
        descriptionText.setFillColor(sf::Color::Black);
        descriptionText.setPosition(
            descriptionButton.getPosition().x + (descriptionButton.getSize().x - descriptionText.getLocalBounds().width) / 2,
            descriptionButton.getPosition().y + (descriptionButton.getSize().y - descriptionText.getLocalBounds().height) / 2 - 5);

        historyText.setFont(font);
        historyText.setString("Historia Gier");
        historyText.setCharacterSize(24);
        historyText.setFillColor(sf::Color::Black);
        historyText.setPosition(
            historyButton.getPosition().x + (historyButton.getSize().x - historyText.getLocalBounds().width) / 2,
            historyButton.getPosition().y + (historyButton.getSize().y - historyText.getLocalBounds().height) / 2 - 5);
    }
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        startSelected = true;
                    }
                    else if (descriptionButton.getGlobalBounds().contains(mousePos)) {
                        descriptionSelected = true;
                    }
                }
            }
        }
    }
    void draw() {
        window.clear(sf::Color::White);
        window.draw(startButton);
        window.draw(descriptionButton);
        window.draw(historyButton);
        window.draw(startText);
        window.draw(descriptionText);
        window.draw(historyText);
        window.display();
    }
    bool isStartSelected() const {
        return startSelected;
    }
    bool isDescriptionSelected() const {
        return descriptionSelected;
    }
    void resetDescriptionSelected() {
        descriptionSelected = false;
    }
};
class DescriptionWindow {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text descriptionText;
public:
    DescriptionWindow(sf::RenderWindow& win) : window(win) {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Unable to load font");
        }

        descriptionText.setFont(font);
        descriptionText.setString("Gra dla 2 osob, polegająca na odbiciu pilki tak by przeciwnik nie zdazyl jej odbic.\n Kazde trafienie w sciane przeciwnika daje nam punkt. Gra konczy się wraz z osiagnieciem progu 3 punktow.");
        descriptionText.setCharacterSize(16);
        descriptionText.setFillColor(sf::Color::White);
        descriptionText.setPosition(
            (window.getSize().x - descriptionText.getLocalBounds().width) / 3,
            (window.getSize().y - descriptionText.getLocalBounds().height) / 3);
    }
    void draw() {
        window.clear(sf::Color::Black);
        window.draw(descriptionText);
        window.display();
    }
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return;
                }
            }
        }
    }
};

class BouncingCircle {
private:
    sf::CircleShape circle;
    sf::Vector2f velocity;
    sf::RenderWindow& window;

public:
    BouncingCircle(sf::RenderWindow& win, float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity)
        : window(win), velocity(initialVelocity) {
        circle.setRadius(radius);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(initialPosition);
    }
    void update() {
        sf::Vector2f position = circle.getPosition();
        if (position.x <= 0 || position.x + circle.getRadius() * 2 >= window.getSize().x) {
            velocity.x = -velocity.x;
        }
        if (position.y <= 0 || position.y + circle.getRadius() * 2 >= window.getSize().y) {
            velocity.y = -velocity.y;
        }
        circle.move(velocity);
    }
    void draw() {
        window.draw(circle);
    }
};
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu i Animacja");
    window.setFramerateLimit(60);
    Menu menu(window);
    BouncingCircle bouncingCircle(window, 30.0f, { 100.0f, 100.0f }, { 5.0f, 4.0f });
    DescriptionWindow description(window);
    bool inAnimation = false;
    bool inDescription = false;

    while (window.isOpen()) {
        if (inDescription) {
            description.handleEvents();
            description.draw();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                inDescription = false;
                menu.resetDescriptionSelected();
            }
        }
        else if (!inAnimation) {
            menu.handleEvents();

            if (menu.isStartSelected()) {
                inAnimation = true;
            }
            else if (menu.isDescriptionSelected()) {
                inDescription = true;
            }
            else {
                menu.draw();
            }
        }
        else {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            bouncingCircle.update();
            window.clear(sf::Color::Green);
            bouncingCircle.draw();
            window.display();
        }
    }

    return 0;
}
