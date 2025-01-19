#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::RectangleShape startButton;
    sf::RectangleShape descriptionButton;
    sf::RectangleShape singlep;
    sf::Font font;
    sf::Text startText;
    sf::Text descriptionText;
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
        window.draw(startText);
        window.draw(descriptionText);
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
    void resetsinglepSelected() {
        singlepSelected = false;
    }
    void resetmultiSelected() {
        startSelected = false;
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

class BallAnimation {
public:
    BallAnimation(sf::RenderWindow& window, sf::Font& loadedFont)
        : window(window), ballRadius(10.f), ballVelocity(4.f, -4.f), ballPosition(400.f, 300.f),
        paddleSize(100.f, 20.f), paddleVelocity(6.f), font(loadedFont) {
        ball.setRadius(ballRadius);
        ball.setFillColor(sf::Color::Cyan);
        ball.setPosition(ballPosition);
        initializeBricks();
        loadGameState();
        initializeLives();
        loadLevel(currentLevel);
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("blad");
        }
        napis.setFont(font);
        napis.setString("Zmiana tla: J");
        napis.setCharacterSize(20);
        napis.setFillColor(sf::Color::Green);
        napis.setPosition(30.f, 550.f);
        convexShape.setPointCount(5);
        convexShape.setPoint(0, sf::Vector2f(100.f, 100.f));
        convexShape.setPoint(1, sf::Vector2f(200.f, 50.f));
        convexShape.setPoint(2, sf::Vector2f(300.f, 150.f));
        convexShape.setPoint(3, sf::Vector2f(250.f, 250.f));
        convexShape.setPoint(4, sf::Vector2f(150.f, 200.f));

        convexShape.setFillColor(sf::Color::Green);
        convexShape.setOutlineThickness(3.f);
        convexShape.setOutlineColor(sf::Color::White);

        bottomPaddle.setSize(paddleSize);
        bottomPaddle.setFillColor(sf::Color::Red);
        bottomPaddle.setPosition(
            (window.getSize().x - paddleSize.x) / 2.f, 
            window.getSize().y - paddleSize.y - 10.f   
        );
    }
    void initializeBricks() {
        const int rows = 3;   
        const int cols = 10;  
        const float brickWidth = 60.0f;
        const float brickHeight = 20.0f;
        const float spacing = 10.0f;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                sf::RectangleShape brick;
                brick.setSize({ brickWidth, brickHeight });
                brick.setFillColor(sf::Color::Yellow);
                brick.setPosition(
                    j * (brickWidth + spacing) + 50.0f,
                    i * (brickHeight + spacing) + 50.0f
                );
                bricks.push_back(brick);
            }
        }
        brickCount = bricks.size();
    }
    void initializeLives() {
        lifeIndicators.clear();
        for (int i = 0; i < lives; ++i) {
            sf::RectangleShape life(sf::Vector2f(20, 20)); 
            life.setFillColor(sf::Color::Red);           
            life.setPosition(10 + i * 30, 10);            
            lifeIndicators.push_back(life);
        }
    }
    void saveGameState() {
        std::ofstream saveFile("game_save.txt");
        if (saveFile.is_open()) {
            saveFile << ball.getPosition().x << " " << ball.getPosition().y << "\n";
            saveFile << (brickCount - bricks.size()) << "\n";
            saveFile.close();
            std::cout << "Game state saved!\n";
        }
        else {
            std::cerr << "Failed to save the game state.\n";
        }
    }
    void loadGameState() {
        std::ifstream loadFile("game_save.txt");
        if (loadFile.is_open()) {
            float ballX, ballY;
            int destroyedBricks;

            loadFile >> ballX >> ballY;
            ball.setPosition(ballX, ballY);

            loadFile >> destroyedBricks;
            bricks.clear();
            initializeBricks();
            loadLevel(currentLevel);
            for (int i = 0; i < destroyedBricks; i++) {
                if (!bricks.empty()) {
                    bricks.pop_back();
                }
            }

            loadFile.close();
            std::cout << "Game state loaded!\n";
        }
        else {
            std::cerr << "No save file found. Starting new game.\n";
            resetGameState(); 
        }
    }
    void resetGameState() {
        ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        bricks.clear(); 
        initializeBricks();
        loadLevel(currentLevel);
        std::cout << "Game state reset!\n";
    }
    void startGame() {
        loadGameState(); 
        isRunning = true;
    }
    void drawLives(sf::RenderWindow& window) {
        for (const auto& life : lifeIndicators) {
            window.draw(life);
        }
    }
    void update() {
        ballPosition += ballVelocity;
        for (auto it = bricks.begin(); it != bricks.end(); ++it) {
            if (it->getGlobalBounds().intersects(ball.getGlobalBounds())) {
                bricks.erase(it);  
                ballVelocity.y = -ballVelocity.y;
                break; 
            }
        }
        if (ballPosition.x <= 0 || ballPosition.x + ballRadius * 2 >= window.getSize().x) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ballPosition.y <= 0) {
            ballVelocity.y = -ballVelocity.y;
        }
        sf::FloatRect ballBounds = ball.getGlobalBounds();
        sf::FloatRect paddleBounds = bottomPaddle.getGlobalBounds();
        if (bricks.empty()) {
            currentLevel++;
            if (currentLevel > 3) {
                std::cout << "Gratulacje! Ukończyłeś wszystkie poziomy!\n";
                window.close();
            }
            else {
                std::cout << "Przechodzisz do poziomu: " << currentLevel << "\n";
                loadLevel(currentLevel);
                ball.setPosition(400, 300);
                ballVelocity = sf::Vector2f(-4.f, -4.f);
            }
        }
        if (ballBounds.intersects(paddleBounds)) {
            ballVelocity.y = -std::abs(ballVelocity.y); 
        }
        if (ballPosition.y + ballRadius * 2 >= window.getSize().y) {
            lives--;
            ballPosition = { 400.f, 300.f }; 
            ballVelocity = { 4.f, -4.f }; 
            if (!lifeIndicators.empty()) {
                lifeIndicators.pop_back();
            }
            if (lives <= 0) {
                exit(0); 
            }
        }
        ball.setPosition(ballPosition);
    }
    void loadLevel(int level) {
        bricks.clear();

        if (level == 1) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j) {
                    bricks.push_back(sf::RectangleShape(sf::Vector2f(50, 20)));
                    bricks.back().setPosition(j * 80 + 10, i * 30 + 50);
                    bricks.back().setFillColor(sf::Color::Yellow);
                }
            }
        }
        else if (level == 2) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 12; ++j) {
                    bricks.push_back(sf::RectangleShape(sf::Vector2f(40, 20)));
                    bricks.back().setPosition(j * 60 + 20, i * 40 + 80);
                    bricks.back().setFillColor(sf::Color::Green);
                }
            }
        }
        else if (level == 3) {
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 8; ++j) {
                    bricks.push_back(sf::RectangleShape(sf::Vector2f(60, 25)));
                    bricks.back().setPosition(j * 70 + 15, i * 35 + 100);
                    bricks.back().setFillColor(sf::Color::Blue);
                }
            }
        }
    }
    void handlePaddleMovement() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            bottomPaddle.getPosition().x > 0) {
            bottomPaddle.move(-paddleVelocity, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
            bottomPaddle.getPosition().x + paddleSize.x < window.getSize().x) {
            bottomPaddle.move(paddleVelocity, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            saveGameState();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            resetGameState();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            if (backgroundColor == sf::Color::Black) {
                backgroundColor = sf::Color::White;
            }
            else {
                backgroundColor = sf::Color::Black;
            }
        }
    }
    void draw() {
        window.clear(backgroundColor);
        window.draw(ball);
        window.draw(bottomPaddle);
        window.draw(napis);
        window.draw(convexShape);
        drawLives(window);
        for (const auto& brick : bricks) {
            window.draw(brick);
        }

    }
private:
    sf::Color backgroundColor = sf::Color::Black;
    sf::RenderWindow& window;
    sf::CircleShape ball;
    sf::RectangleShape bottomPaddle;
    std::vector<sf::RectangleShape> bricks;
    sf::Vector2f ballVelocity;
    sf::Vector2f ballPosition;
    sf::ConvexShape convexShape;
    int lives = 3;
    int currentLevel = 1;
    sf::RectangleShape lifeIndicator; 
    std::vector<sf::RectangleShape> lifeIndicators;
    sf::Text napis;
    sf::Font& font;
    float ballRadius;
    bool isRunning = false;
    int brickCount;

    sf::Vector2f paddleSize;
    float paddleVelocity;
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
        if (rightScore || leftScore > 3) {
            sf::Text endGameText;
            endGameText.setFont(font);
            endGameText.setCharacterSize(50);
            endGameText.setFillColor(sf::Color::Red);
            endGameText.setString(leftScore >= 3 ? "Lewy gracz wygral!" : "Prawy gracz wygral!");
            endGameText.setPosition(
                (window.getSize().x - endGameText.getLocalBounds().width) / 2,
                (window.getSize().y - endGameText.getLocalBounds().height) / 2
            );
            window.clear();
            window.draw(endGameText);
            window.display();
            sf::sleep(sf::seconds(1));
            window.close(); 
            return false;
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
    sf::Font font;
    window.setFramerateLimit(60);

    Menu menu(window);
    int leftScore = 0;
    int rightScore = 0;
    BouncingCircle bouncingCircle(window, 20.0f, { 100.0f, 100.0f }, { 8.0f, 7.0f }, leftScore, rightScore);
    Paddle leftPaddle(50.0f, 250.0f, 20.0f, 100.0f, 7.0f);
    Paddle rightPaddle(730.0f, 250.0f, 20.0f, 100.0f, 7.0f);
    BallAnimation ballAnimation(window, font);
    bool insinglePlayer = false;
    DescriptionWindow description(window);
    bool inAnimation = false;
    bool inDescription = false;
    sf::Color mapColor;
    PauseMenu pauseMenu(window);
    bool isPaused = false;
    bool inMenu = true;
    
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
            ballAnimation.update();
            window.clear(sf::Color::Black);  
            ballAnimation.draw();  
            ballAnimation.handlePaddleMovement();
            window.display();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                insinglePlayer = false;
                menu.resetsinglepSelected();
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                inAnimation = false;
                menu.resetmultiSelected();
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
