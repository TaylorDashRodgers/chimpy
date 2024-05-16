//
// Created by Taylor Rodgers on 11/5/22.
//

#ifndef CHIMPYGAME_BULLET_H
#define CHIMPYGAME_BULLET_H

#endif //CHIMPYGAME_BULLET_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet {
public:
    CircleShape bullet;
    Vector2f currVelocity;
    float maxSpeed;

    Bullet (float radius = 5.f)
        : currVelocity(0.f, 0.f), maxSpeed(15.f)

    {
        this->bullet.setRadius(radius);
        this->bullet.setFillColor(Color::Green);
    }

    void draw(RenderWindow &window){
        window.draw(bullet);
    }

    void setPos(Vector2f newPos) {
        bullet.setPosition(newPos);
    }
private:
};
