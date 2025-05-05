#pragma once
#include "Platform.h"

class MovingYPlatform : public Platform {
public:
    float speedY = 1.0f;

    MovingYPlatform(float startX, float startY, const std::string& texturePath);

    void update() override;
};