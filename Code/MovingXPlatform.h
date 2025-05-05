#pragma once
#include "Platform.h"

class MovingXPlatform : public Platform
{
public:
    float speedX = 1.0f;

    MovingXPlatform(float startX, float startY, const std::string& texturePath);

    void update() override;
};