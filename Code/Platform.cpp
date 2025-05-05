#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Constant.h"
#include <cstdlib>
#include <string>

Platform::Platform(float startX, float startY, const std::string& texturePath) : x(startX), y(startY)
{
    if (!texture.loadFromFile(texturePath))
    {
   
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

Platform::~Platform()
{

}

void Platform::update()
{
    sprite.setPosition(x, y);
}

/*bool Platform::checkCollision(float playerX, float playerY, float playerWidth, float playerHeight, float playerDy)
{
    if ((playerX + playerWidth > x) && (playerX < x + sprite.getGlobalBounds().width) &&
        (playerY + playerHeight > y) && (playerY < y + sprite.getGlobalBounds().height) &&
        playerDy > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}*/

bool Platform::checkCollision(float doodleX, float doodleY, float doodleWidth, float doodleHeight, float& doodleDy)
{
    // Отримуємо поточні межі Doodle та платформи
    sf::FloatRect doodleBounds(doodleX, doodleY, doodleWidth, doodleHeight);
    sf::FloatRect platformBounds = sprite.getGlobalBounds();

    // 1. Перевіряємо, чи Doodle рухається вниз
    if (doodleDy > 0)
    {
        // 2. Перевіряємо, чи нижній край Doodle був вище верхнього краю платформи на попередньому кроці
        // (doodleY - doodleDy) - це приблизна Y-координата Doodle на попередньому кроці
        float prevDoodleBottom = (doodleY - doodleDy) + doodleHeight;
        float currentDoodleBottom = doodleY + doodleHeight;
        float platformTop = platformBounds.top;

        // Перевіряємо, чи нижній край Doodle перетнув верхній край платформи
        if (prevDoodleBottom <= platformTop && currentDoodleBottom >= platformTop)
        {
            // 3. Перевіряємо, чи горизонтальні проекції перетинаються
            if (doodleBounds.left < platformBounds.left + platformBounds.width &&
                doodleBounds.left + doodleBounds.width > platformBounds.left)
            {
                // Якщо всі умови виконуються, це зіткнення з верхньою частиною платформи
                return true;
            }
        }
    }

    // Якщо умови не виконані, зіткнення не відбулося (або відбулося з іншою частиною, яку ми ігноруємо)
    return false;
}

bool Platform::isBroken() const
{
    return false;
}

void Platform::move(float dy)
{
    y -= dy;
    sprite.setPosition(x, y);
}

void Platform::reset(float newY)
{
    y = newY;
    x = std::rand() % (windowWidth - static_cast<int>(sprite.getGlobalBounds().width));
    sprite.setPosition(x, y);
}