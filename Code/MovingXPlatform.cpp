#include "MovingXPlatform.h"
#include <SFML/Graphics.hpp>
#include "Constant.h"

MovingXPlatform::MovingXPlatform(float startX, float startY, const std::string& texturePath) : Platform(startX, startY, texturePath)
{

}

void MovingXPlatform::update()
{
    x += speedX;
    if (x < 0 || x > windowWidth - sprite.getGlobalBounds().width)
    {
        speedX *= -1;
    }
    Platform::update();
}