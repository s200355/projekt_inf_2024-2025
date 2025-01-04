#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::RectangleShape startButton;
    sf::RectangleShape descriptionButton;
    sf::RectangleShape historyButton;
    sf::RectangleShape singlep;
    sf::Font font;
    sf::Text startText;
    sf::Text descriptionText;
    sf::Text historyText;
    sf::Text singleptext;
    bool startSelected = false;
    bool descriptionSelected = false;
    bool singlepSelected = false;
public:
    Menu(sf::RenderWindow& win) : window(win) {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("blad");
        }
        singlep.setSize({ 220.0f, 80.0f });
        singlep.setFillColor(sf::Color::Cyan);
        singlep.setPosition(300.0f, 100.0f);
        singlep.setOutlineThickness(5.0f);
        singlep.setOutlineColor(sf::Color::Black);

        startButton.setSize({ 220.0f, 80.0f });
        startButton.setFillColor(sf::Color::Cyan);
        startButton.setPosition(300.0f, 200.0f);
        startButton.setOutlineThickness(5.0f);
        startButton.setOutlineColor(sf::Color::Black);


        descriptionButton.setSize({ 220.0f, 80.0f });
        descriptionButton.setFillColor(sf::Color::Cyan);
        descriptionButton.setPosition(300.0f, 300.0f);
        descriptionButton.setOutlineThickness(5.0f);
        descriptionButton.setOutlineColor(sf::Color::Black);

        historyButton.setSize({ 220.0f, 80.0f });
        historyButton.setFillColor(sf::Color::Cyan);
        historyButton.setPosition(300.0f, 400.0f);
        historyButton.setOutlineThickness(5.0f);
        historyButton.setOutlineColor(sf::Color::Black);

        singleptext.setFont(font);
        singleptext.setString("1 osoba");
        singleptext.setCharacterSize(24);
        singleptext.setFillColor(sf::Color::Black);
        singleptext.setPosition(360.0f, 125.0f);

        startText.setFont(font);
        startText.setString("2 osoby");
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::Black);
        startText.setPosition(360.0f, 225.0f);

        descriptionText.setFont(font);
        descriptionText.setString("Opis");
        descriptionText.setCharacterSize(24);
        descriptionText.setFillColor(sf::Color::Black);
        descriptionText.setPosition(380.0f, 325.0f);

        historyText.setFont(font);
        historyText.setString("Historia Gier");
        historyText.setCharacterSize(24);
        historyText.setFillColor(sf::Color::Black);
        historyText.setPosition(340.0f, 425.0f);
    }
    void handleEvents() {
        sf::Event event;
        bool selectingMap = false;
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
                    else if (singlep.getGlobalBounds().contains(mousePos)) {
                        singlepSelected = true;
                    }
                }
            }
        }
    }
    void draw() {
        window.clear(sf::Color::Green);
        window.draw(startButton);
        window.draw(descriptionButton);
        window.draw(historyButton);
        window.draw(startText);
        window.draw(descriptionText);
        window.draw(historyText);
        window.draw(singlep);
        window.draw(singleptext);
        window.display();
    }
    bool isStartSelected() const {
        return startSelected;
    }
    bool isDescriptionSelected() const {
        return descriptionSelected;
    }
    bool issinglepSelected() const {
        return singlepSelected;
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
            throw std::runtime_error("blad");
        }

        descriptionText.setFont(font);
        descriptionText.setString("Gra dla 2 osob lub 1 osoby \n, polegająca na odbiciu pilki tak by przeciwnik nie zdazyl jej odbic.\n Kazde trafienie w sciane przeciwnika daje nam punkt.\n Tryb dla jednej osoby polega na zbiciu bloczkow w celu uzyskania punktow");
        descriptionText.setCharacterSize(16);
        descriptionText.setFillColor(sf::Color::White);
        descriptionText.setPosition(
            (window.getSize().x - descriptionText.getLocalBounds().width) / 4,
            (window.getSize().y - descriptionText.getLocalBounds().height) / 4);
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
class Paddle {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Paddle(float x, float y, float width, float height, float speed)
        : speed(speed) {
        shape.setSize({ width, height });
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void moveUp() {
        if (shape.getPosition().y > 0) {
            shape.move(0, -speed);
        }
    }

    void moveDown(float windowHeight) {
        if (shape.getPosition().y + shape.getSize().y < windowHeight) {
            shape.move(0, speed);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }
};

class Ball {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    Paddle bottomPaddle;

public:
    Ball(float radius, sf::Vector2f position, sf::Vector2f initialVelocity)
        : shape(radius), velocity(initialVelocity), bottomPaddle(350.0f, 550.0f, 100.0f, 20.0f, 7.0f) {
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);
    }

    void update(const sf::RenderWindow& window) {
        // Aktualizacja pozycji
        sf::Vector2f position = shape.getPosition();
        position += velocity;

        // Odbicie od ścian
        if (position.x <= 0 || position.x + shape.getRadius() * 2 >= window.getSize().x) {
            velocity.x = -velocity.x;
        }
        if (position.y <= 0 || position.y + shape.getRadius() * 2 >= window.getSize().y) {
            velocity.y = -velocity.y;
        }
        sf::FloatRect ballBounds = shape.getGlobalBounds();
        sf::FloatRect paddleBounds = bottomPaddle.getGlobalBounds();

        if (ballBounds.intersects(paddleBounds)) {
            // Odbicie piłki w górę
            velocity.y = -std::abs(velocity.y);
            shape.setPosition(position);
        }
        shape.setPosition(position);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        
    }
};
class Paddle1 {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Paddle1(float x, float y, float width, float height, float speed)
        : speed(speed) {
        shape.setSize({ width, height });
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void moveLeft() {
        if (shape.getPosition().x > 0) {
            shape.move(-speed, 0);
        }
    }

    void moveRight(float windowWidth) {
        if (shape.getPosition().x + shape.getSize().x < windowWidth) {
            shape.move(speed, 0);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }
};
class BouncingCircle {
private:
    sf::CircleShape circle;
    sf::Vector2f velocity;
    sf::RenderWindow& window;
    sf::Font font;
    int& leftScore;
    int& rightScore;
    sf::Text scoreText;
    sf::Clock delayClock;
    bool waitingForRestart = false;
    float delayTime = 2.0f;
    void updateScoreText() {
        std::ostringstream ss;
        ss << leftScore << " : " << rightScore;
        scoreText.setString(ss.str());
        scoreText.setPosition(
            (window.getSize().x - scoreText.getLocalBounds().width) / 2,
            20);
    }


public:
    BouncingCircle(sf::RenderWindow& win, float radius, const sf::Vector2f& initialPosition, const sf::Vector2f& initialVelocity, int& left, int& right)
        : window(win), velocity(initialVelocity), leftScore(left), rightScore(right) {
        circle.setRadius(radius);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(initialPosition);
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("blad");
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(35);
        scoreText.setFillColor(sf::Color::White);

        updateScoreText();
    }

    bool update(const Paddle& leftPaddle, const Paddle& rightPaddle) {
        if (waitingForRestart) {
            if (delayClock.getElapsedTime().asSeconds() >= delayTime) {
                waitingForRestart = false;
            }
            else {
                return true;
            }
        }
        sf::Vector2f position = circle.getPosition();
        if (position.x <= 0) {
            rightScore++;
            circle.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            waitingForRestart = true;
            delayClock.restart();
            updateScoreText();
            return true;
        }
        if (position.x + circle.getRadius() * 2 >= window.getSize().x) {
            leftScore++;
            circle.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            waitingForRestart = true;
            delayClock.restart();
            updateScoreText();
            return true;
        }
        if (position.y <= 0 || position.y + circle.getRadius() * 2 >= window.getSize().y) {
            velocity.y = -velocity.y;
        }
        if (circle.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) ||
            circle.getGlobalBounds().intersects(rightPaddle.getGlobalBounds())) {
            velocity.x = -velocity.x;
        }
        circle.move(velocity);
        return true;
    }
    void draw() {
        window.draw(circle);
        window.draw(scoreText);
    }
    sf::FloatRect getGlobalBounds() const {
        return circle.getGlobalBounds();
    }
};

class PauseMenu {
private:
    sf::RectangleShape background;
    sf::RectangleShape closeButton;
    sf::Text pauseText;
    sf::Text closeButtonText;
    sf::Font font;
    sf::RenderWindow& window;

public:
    PauseMenu(sf::RenderWindow& win) : window(win) {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Unable to load font");
        }


        background.setSize(sf::Vector2f(400, 300));
        background.setFillColor(sf::Color(50, 50, 50, 200));
        background.setPosition(200.0f, 150.0f);

        pauseText.setFont(font);
        pauseText.setString("Pauza. Jest to gra typu PONG, \n Sterowanie: \n W oraz S - lewy gracz, \n strzalki(gora-dol) - prawy gracz");
        pauseText.setCharacterSize(18);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(
            background.getPosition().x + 50,
            background.getPosition().y + 50);

        closeButton.setSize(sf::Vector2f(100, 50));
        closeButton.setFillColor(sf::Color::Red);
        closeButton.setPosition(
            background.getPosition().x + background.getSize().x - 110,
            background.getPosition().y + background.getSize().y - 60);

        closeButtonText.setFont(font);
        closeButtonText.setString("X");
        closeButtonText.setCharacterSize(24);
        closeButtonText.setFillColor(sf::Color::White);
        closeButtonText.setPosition(
            closeButton.getPosition().x + (closeButton.getSize().x - closeButtonText.getLocalBounds().width) / 2,
            closeButton.getPosition().y + (closeButton.getSize().y - closeButtonText.getLocalBounds().height) / 2 - 5);
    }

    bool handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (closeButton.getGlobalBounds().contains(mousePos)) {
                    return false;
                }
            }
        }
        return true;
    }

    void draw() {
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(pauseText);
        window.draw(closeButton);
        window.draw(closeButtonText);
        window.display();
    }
};
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    window.setFramerateLimit(60);
    Menu menu(window);
    int leftScore = 0;
    int rightScore = 0;
    BouncingCircle bouncingCircle(window, 20.0f, { 100.0f, 100.0f }, { 8.0f, 7.0f }, leftScore, rightScore);
    Paddle leftPaddle(50.0f, 250.0f, 20.0f, 100.0f, 7.0f);
    Paddle rightPaddle(730.0f, 250.0f, 20.0f, 100.0f, 7.0f);
    Paddle1 bottomPaddle(350.0f, 550.0f, 100.0f, 20.0f, 7.0f);
    Ball singlePlayerBall(10.f, { 400.f, 300.f }, { 5.f, 4.f });
    bool insinglePlayer = false;
    DescriptionWindow description(window);
    bool inAnimation = false;
    bool inDescription = false;
    sf::Color mapColor;
    PauseMenu pauseMenu(window);
    bool isPaused = false;
    

    while (window.isOpen()) {
        if (isPaused) {
            if (!pauseMenu.handleEvents()) {
                isPaused = false;
            }
            else {
                pauseMenu.draw();
                continue;
            }
        }
        if (insinglePlayer) {
            singlePlayerBall.update(window);
            window.clear(sf::Color::Black);  
            singlePlayerBall.draw(window);  
            bottomPaddle.draw(window);
            window.display();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                bottomPaddle.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                bottomPaddle.moveRight(window.getSize().x);
            }
        }
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
            else if (menu.issinglepSelected()) {
                insinglePlayer = true;
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                isPaused = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                leftPaddle.moveUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                leftPaddle.moveDown(window.getSize().y);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                rightPaddle.moveUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                rightPaddle.moveDown(window.getSize().y);
            }

            bouncingCircle.update(leftPaddle, rightPaddle);
            window.clear();
            bouncingCircle.draw();
            leftPaddle.draw(window);
            rightPaddle.draw(window);
            window.display();
        }
    }

    return 0;
}
