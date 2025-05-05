#include "MovingYPlatform.h"
#include <SFML/Graphics.hpp>
#include "Constant.h"

MovingYPlatform::MovingYPlatform(float startX, float startY, const std::string& texturePath) : Platform(startX, startY, texturePath)
{

}

void MovingYPlatform::update()
{
    y += speedY;
    if (y < 0 || y > windowHeight - sprite.getGlobalBounds().height) {
        speedY *= -1;
    }
    Platform::update();
}