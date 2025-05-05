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
    // �������� ������ ��� Doodle �� ���������
    sf::FloatRect doodleBounds(doodleX, doodleY, doodleWidth, doodleHeight);
    sf::FloatRect platformBounds = sprite.getGlobalBounds();

    // 1. ����������, �� Doodle �������� ����
    if (doodleDy > 0)
    {
        // 2. ����������, �� ����� ���� Doodle ��� ���� ��������� ���� ��������� �� ������������ �����
        // (doodleY - doodleDy) - �� ��������� Y-���������� Doodle �� ������������ �����
        float prevDoodleBottom = (doodleY - doodleDy) + doodleHeight;
        float currentDoodleBottom = doodleY + doodleHeight;
        float platformTop = platformBounds.top;

        // ����������, �� ����� ���� Doodle �������� ������ ���� ���������
        if (prevDoodleBottom <= platformTop && currentDoodleBottom >= platformTop)
        {
            // 3. ����������, �� ������������ �������� �������������
            if (doodleBounds.left < platformBounds.left + platformBounds.width &&
                doodleBounds.left + doodleBounds.width > platformBounds.left)
            {
                // ���� �� ����� �����������, �� �������� � �������� �������� ���������
                return true;
            }
        }
    }

    // ���� ����� �� �������, �������� �� �������� (��� �������� � ����� ��������, ��� �� ��������)
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