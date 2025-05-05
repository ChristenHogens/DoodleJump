#include "BreakablePlatform.h"
#include "Constant.h"

BreakablePlatform::BreakablePlatform(float startX, float startY, const std::string& texturePath) : Platform(startX, startY, texturePath)
{
  
}

bool BreakablePlatform::checkCollision(float doodleX, float doodleY, float doodleWidth, float doodleHeight, float& doodleDy)
{
    if (!broken && Platform::checkCollision(doodleX, doodleY, doodleWidth, doodleHeight, doodleDy)) {
        broken = true;
        return true;
    }
    return false;
}

bool BreakablePlatform::isBroken() const
{
    return broken;
}

void BreakablePlatform::reset(float newY)
{
    Platform::reset(newY);
    broken = false;
}