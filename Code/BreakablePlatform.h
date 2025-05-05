#pragma once
#include "Platform.h"

class BreakablePlatform : public Platform {
private:
    bool broken = false;

public:
    BreakablePlatform(float startX, float startY, const std::string& texturePath);

    bool checkCollision(float doodleX, float doodleY, float doodleWidth, float doodleHeight, float& doodleDy) override;

    bool isBroken() const override;

    void reset(float newY) override;
};

