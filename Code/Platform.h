#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    float x, y;
    sf::Sprite sprite;
    sf::Texture texture;

    Platform(float startX, float startY, const std::string& texturePath);

    virtual ~Platform();

    virtual void update();

    virtual bool checkCollision(float doodleX, float doodleY, float doodleWidth, float doodleHeight, float& doodleDy);

    virtual bool isBroken() const;

    virtual void move(float dy);

    virtual void reset(float newY);
};