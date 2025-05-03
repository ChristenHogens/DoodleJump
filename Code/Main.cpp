#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int windowWidth = 400;
const int windowHeight = 600;
const int platformCount = 8;

enum class PlatformType { 
    Normal, 
    MovingX, 
    MovingY, 
    Breakable 
};

struct Platform {
    float x, y;
    PlatformType type;
    bool broken = false; // для крихких платформ
    float speedX = 1.0f; // для рухомих по осі X
    float speedY = 1.0f; // для рухомих по осі Y
};

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

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Doodle Jump");
    window.setFramerateLimit(60);

    //Background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\background.png"))
    {
        return -1; // Вихід, якщо фон не завантажився
    }

    sf::Sprite backgroundSprite(backgroundTexture);

    //Buttons
    sf::Font font;
    if (!font.loadFromFile("C:\\Course\\1\\DoodleJump\\Font\\BrownieStencil.ttf"))
    {
        return -1; // Вихід, якщо шрифт не завантажився
    }

    sf::Text title("Doodle Jump", font, 40);
    title.setPosition(85, 100);
    title.setFillColor(sf::Color::Yellow);

    sf::Text startBtn("Start", font, 30);
    startBtn.setPosition(170, 300);
    startBtn.setFillColor(sf::Color::Green);

    sf::Text exitBtn("Exit", font, 30);
    exitBtn.setPosition(185, 400);
    exitBtn.setFillColor(sf::Color::Red);

    //Live text
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(10, 10);

    //Score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(200, 10);

    // Textures
    sf::Texture doodleTexture, platformTexture, platformMovingXTexture, platformMovingYTexture, platformBreakableTexture;
    doodleTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\doodle.png");
    platformTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\platform.png");
    platformMovingXTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\platform_moving_x.png");
    platformMovingYTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\platform_moving_y.png");
    platformBreakableTexture.loadFromFile("C:\\Course\\1\\DoodleJump\\Images\\platform_breakable.png");

    // Doodle sprite
    sf::Sprite doodle(doodleTexture);
    doodle.setPosition(200, 300);

    // Platforms
    Platform platforms[platformCount];
  
    for (int i = 0; i < platformCount; i++)
    {
        platforms[i].x = std::rand() % windowWidth;
        platforms[i].y = std::rand() % windowHeight;

        int typeChance = std::rand() % 100;
        if (typeChance < 60) platforms[i].type = PlatformType::Normal;
        else if (typeChance < 75) platforms[i].type = PlatformType::MovingX;
        else if (typeChance < 90) platforms[i].type = PlatformType::MovingY;
        else platforms[i].type = PlatformType::Breakable;
    }

    float x = 100, y = 100;
    float dx = 0, dy = 0;
    float gravity = 0.2f;
    float jumpStrength = -10.0f;

    while (window.isOpen())
    {

        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            //Check "play" and "Exit" buttons
            if (currentState == GameState::Menu)
            {
                if (e.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (startBtn.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        currentState = GameState::Playing;
                    }

                    if (exitBtn.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        window.close();
                    }
                }
            }

            //Check "R" and "Esc" buttons
            if (currentState == GameState::GameOver)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    currentState = GameState::Playing;
                    lives = 3;
                    score = 0;
                    x = 100; y = 100; dy = 0;
                    for (int i = 0; i < platformCount; i++)
                    {
                        platforms[i].x = std::rand() % windowWidth;
                        platforms[i].y = std::rand() % windowHeight;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
            }
        }

        // Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 4;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -4;

        x += dx;

        // Screen wrap
        if (x > windowWidth) x = 0;
        if (x < 0) x = windowWidth;

        dy += gravity;
        y += dy;

        // Update platform positions (moving types)
        for (int i = 0; i < platformCount; i++)
        {
            if (platforms[i].type == PlatformType::MovingX)
            {
                platforms[i].x += platforms[i].speedX;
                if (platforms[i].x < 0 || platforms[i].x > windowWidth - 68)
                {
                    platforms[i].speedX *= -1;
                }
            }
            else if (platforms[i].type == PlatformType::MovingY)
            {
                platforms[i].y += platforms[i].speedY;
                if (platforms[i].y < 0 || platforms[i].y > windowHeight - 14)
                {
                    platforms[i].speedY *= -1;
                }
            }
        }

        //live check
        if (y > windowHeight)
        {
            lives--;
            if (lives > 0)
            {
                // Відновлюємо позицію гравця
                x = 100;
                y = 100;
                dy = 0;

                // Перерозташовуємо платформи
                for (int i = 0; i < platformCount; i++)
                {
                    platforms[i].x = std::rand() % windowWidth;
                    platforms[i].y = std::rand() % windowHeight;
                }
            }
            else
            {
                currentState = GameState::GameOver;
                score = 0;
            }
        }

        // Platform collision
        for (int i = 0; i < platformCount; i++)
        {
            if (platforms[i].type == PlatformType::Breakable && platforms[i].broken)
            {
                continue;
            }

            if ((x + 50 > platforms[i].x) && (x + 20 < platforms[i].x + 68) &&
                (y + 70 > platforms[i].y) && (y + 70 < platforms[i].y + 14) &&
                dy > 0)
            {
                dy = jumpStrength;

                if (platforms[i].type == PlatformType::Breakable)
                {
                    platforms[i].broken = true;
                }
            }
        }

        // Scroll screen up when player goes up
        if (y < 200)
        {
            for (int i = 0; i < platformCount; i++)
            {
                platforms[i].y -= dy;
                if (platforms[i].y > windowHeight)
                {
                    platforms[i].y = 0;
                    platforms[i].x = std::rand() % windowWidth;

                    int typeChance = std::rand() % 100;
                    if (typeChance < 60)
                    {
                        platforms[i].type = PlatformType::Normal;
                    }
                    else if (typeChance < 75)
                    {
                        platforms[i].type = PlatformType::MovingX;
                    }
                    else if (typeChance < 90)
                    {
                        platforms[i].type = PlatformType::MovingY;
                    }
                    else
                    {
                        platforms[i].type = PlatformType::Breakable;
                    }

                    platforms[i].broken = false;
                }
            }
            y = 200;
            score += static_cast<int>(-dy); // чим сильніше підстрибок — тим більше очок
            if (score > highScore)
            {
                highScore = score;
            }
        }

        doodle.setPosition(x, y);

        // Drawing
        window.clear(sf::Color::White);

        if (currentState == GameState::Menu)
        {
            window.draw(title);
            window.draw(startBtn);
            window.draw(exitBtn);
        }
        else if (currentState == GameState::Playing)
        {
            
            livesText.setString("Lives: " + std::to_string(lives));
            scoreText.setString("Score: " + std::to_string(score));

            window.draw(backgroundSprite);

            window.draw(livesText);

            window.draw(scoreText);

            window.draw(doodle);

            for (int i = 0; i < platformCount; i++)
            {
                if (platforms[i].type == PlatformType::Breakable && platforms[i].broken)
                {
                    continue; // не малюємо зламану крихку платформу
                }

                sf::Sprite platform;

                if (platforms[i].type == PlatformType::Breakable)
                {
                    if (platforms[i].broken) continue;
                    platform.setTexture(platformBreakableTexture);
                }
                else if (platforms[i].type == PlatformType::MovingX)
                {
                    platform.setTexture(platformMovingXTexture);
                }
                else if (platforms[i].type == PlatformType::MovingY)
                {
                    platform.setTexture(platformMovingYTexture);
                }
                else
                {
                    platform.setTexture(platformTexture);
                }

                platform.setPosition(platforms[i].x, platforms[i].y);
                window.draw(platform);
            }
        }
        else if (currentState == GameState::GameOver)
        {
            sf::Text gameOverText("Game Over", font, 40);
            gameOverText.setPosition(100, 200);
            gameOverText.setFillColor(sf::Color::Red);

            sf::Text retryText("Press R to Retry\n  or Esc to Exit", font, 24);
            retryText.setPosition(100, 300);
            retryText.setFillColor(sf::Color::Black);

            window.draw(gameOverText);
            window.draw(retryText);
            score = 0;
        }

        window.display();

        dx = 0; // reset horizontal movement
    }

    return 0;
}