#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Platform.h"
#include "MovingXPlatform.h"
#include "MovingYPlatform.h"
#include "BreakablePlatform.h"
#include "Constant.h"

using namespace std;
using namespace sf;

enum class GameState {
    Menu,
    Playing,
    GameOver
};

GameState currentState = GameState::Menu;

int main()
{
    int lives = 3;
    int score = 0;
    int highScore = 0;

    srand(static_cast<unsigned>(std::time(nullptr)));

    RenderWindow window(VideoMode(windowWidth, windowHeight), "Doodle Jump");
    window.setFramerateLimit(60);

    // Background
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Images\\background.png"))
    {
        cout << "Error loading background texture!" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Buttons and Text
    Font font;
    if (!font.loadFromFile("Font\\BrownieStencil.ttf"))
    {
        cout << "Error loading font!" << endl;
        return -1;
    }

    Text title("Doodle Jump", font, 40);
    title.setPosition(85, 100);
    title.setFillColor(Color::Yellow);

    Text startBtn("Start", font, 30);
    startBtn.setPosition(170, 300);
    startBtn.setFillColor(Color::Green);

    Text exitBtn("Exit", font, 30);
    exitBtn.setPosition(185, 400);
    exitBtn.setFillColor(Color::Red);

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::Red);
    livesText.setPosition(10, 10);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(200, 10);

    // Doodle sprite
    Texture doodleTexture;
    if (!doodleTexture.loadFromFile("Images\\doodle.png"))
    {
        cout << "Error loading doodle texture!" << endl;
        return -1;
    }
    Sprite doodle(doodleTexture);
    doodle.setPosition(200, 300);

    // Platforms
    vector<Platform*> platforms;

    for (int i = 0; i < platformCount; i++)
    {
        float platX = rand() % windowWidth;
        float platY = rand() % windowHeight;

        int typeChance = std::rand() % 100;
        if (typeChance < 60) {
            platforms.push_back(new Platform(platX, platY, "Images\\platform.png"));
        }
        else if (typeChance < 75) {
            platforms.push_back(new MovingXPlatform(platX, platY, "Images\\platform_moving_x.png"));
        }
        else if (typeChance < 90) {
            platforms.push_back(new MovingYPlatform(platX, platY, "Images\\platform_moving_y.png"));
        }
        else {
            platforms.push_back(new BreakablePlatform(platX, platY, "Images\\platform_breakable.png"));
        }
    }

    float x = 100, y = 100;
    float dx = 0, dy = 0;
    float gravity = 0.2f;
    float jumpStrength = -10.0f;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (currentState == GameState::Menu)
            {
                if (e.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (startBtn.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        currentState = GameState::Playing;
                        // Reset game state for new game
                        lives = 3;
                        score = 0;
                        x = 100; y = 100; dy = 0;
                        for (Platform* plat : platforms)
                        {
                            if (plat != nullptr)
                            {
                                plat->reset(rand() % windowHeight);
                            }
                        }
                    }
                    if (exitBtn.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        window.close();
                    }
                }
            }

            if (currentState == GameState::GameOver)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::R)
                    {
                        currentState = GameState::Playing;
                        lives = 3;
                        score = 0;
                        x = 100; y = 100; dy = 0;
                        for (Platform* plat : platforms)
                        {
                            if (plat != nullptr)
                            {
                                plat->reset(std::rand() % windowHeight);
                            }
                        }
                    }
                    if (e.key.code == Keyboard::Escape)
                    {
                        window.close();
                    }
                }
            }
        }

        if (currentState == GameState::Playing)
        {
            // Input
            if (Keyboard::isKeyPressed(Keyboard::Right)) dx = 4;
            else if (Keyboard::isKeyPressed(Keyboard::Left)) dx = -4;
            else dx = 0;

            x += dx;

            // Screen wrap
            if (x > windowWidth) x = 0;
            if (x < 0) x = windowWidth;

            dy += gravity;
            y += dy;

            // Update platform positions (moving types) and check collisions
            for (Platform* plat : platforms)
            {
                if (plat == nullptr) continue;

                plat->update();

                if (!plat->isBroken() && plat->checkCollision(x, y, doodle.getGlobalBounds().width, doodle.getGlobalBounds().height, dy))
                {
                    dy = jumpStrength;
                }
            }

            // Live check
            if (y > windowHeight)
            {
                lives--;
                if (lives > 0)
                {
                    // Reset player position
                    x = 100;
                    y = 100;
                    dy = 0;

                    // Reset platforms
                    for (Platform* plat : platforms)
                    {
                        if (plat != nullptr)
                        {
                            plat->reset(rand() % windowHeight);
                        }
                    }
                }
                else
                {
                    currentState = GameState::GameOver;
                    if (score > highScore) highScore = score;
                    score = 0; // Reset score on game over
                }
            }

            // Scroll screen up when player goes up
            if (y < 200)
            {
                for (size_t i = 0; i < platforms.size(); ++i)
                {
                    if (platforms[i] == nullptr)
                    {
                        continue;
                    }

                    platforms[i]->move(dy);
                    if (platforms[i]->y > windowHeight)
                    {
                        delete platforms[i];
                        platforms[i] = nullptr; 

                        float newPlatX;
                        int typeChance = rand() % 100;
                        if (typeChance < 60)
                        {
                            newPlatX = rand() % (windowWidth - 68); 
                            platforms[i] = new Platform(newPlatX, 0, "Images\\platform.png");
                        }
                        else if (typeChance < 75)
                        {
                            newPlatX = rand() % (windowWidth - 68);
                            platforms[i] = new MovingXPlatform(newPlatX, 0, "Images\\platform_moving_x.png");
                        }
                        else if (typeChance < 90)
                        {
                            newPlatX = rand() % (windowWidth - 68);
                            platforms[i] = new MovingYPlatform(newPlatX, 0, "Images\\platform_moving_y.png");
                        }
                        else
                        {
                            newPlatX = std::rand() % (windowWidth - 68);
                            platforms[i] = new BreakablePlatform(newPlatX, 0, "Images\\platform_breakable.png");
                        }
                    }
                }
                y = 200;
                score += static_cast<int>(-dy);
                if (score > highScore)
                {
                    highScore = score;
                }
            }

            doodle.setPosition(x, y);
        }

        // Drawing
        window.clear(Color::White);

        if (currentState == GameState::Menu)
        {
            window.draw(title);
            window.draw(startBtn);
            window.draw(exitBtn);
        }
        else if (currentState == GameState::Playing)
        {
            livesText.setString("Lives: " + to_string(lives));
            scoreText.setString("Score: " + to_string(score));

            window.draw(backgroundSprite);
            window.draw(livesText);
            window.draw(scoreText);
            window.draw(doodle);

            for (Platform* plat : platforms)
            {
                if (plat == nullptr)
                {
                    continue;
                }
                if (plat->isBroken())
                {
                    continue;
                }
                window.draw(plat->sprite);
            }
        }
        else if (currentState == GameState::GameOver)
        {
            Text gameOverText("Game Over", font, 40);
            gameOverText.setPosition(100, 200);
            gameOverText.setFillColor(Color::Red);

            Text finalScoreText("Final Score: " + std::to_string(score), font, 24);
            finalScoreText.setPosition(100, 250);
            finalScoreText.setFillColor(Color::Black);

            Text highScoreDisplay("High Score: " + std::to_string(highScore), font, 24);
            highScoreDisplay.setPosition(100, 280);
            highScoreDisplay.setFillColor(Color::Blue);


            Text retryText("Press R to Retry\n  or Esc to Exit", font, 24);
            retryText.setPosition(100, 350);
            retryText.setFillColor(Color::Black);


            window.draw(gameOverText);
            window.draw(finalScoreText);
            window.draw(highScoreDisplay);
            window.draw(retryText);

        }

        window.display();
    }

    // Clearing the memory allocated to platforms
    for (Platform* plat : platforms)
    {
        delete plat;
    }
    platforms.clear();

    return 0;
}