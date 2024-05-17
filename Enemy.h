//
// Created by Taylor Rodgers on 11/5/22.
//

#ifndef CHIMPYGAME_ENEMY_H
#define CHIMPYGAME_ENEMY_H

#endif //CHIMPYGAME_ENEMY_H

#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace sf;

class Enemy {
public:

    Enemy(float radius = 75.f) {
        enemy.setRadius(radius);
        enemy.setFillColor(Color::Blue);
    }

    int getX() {
        return enemy.getPosition().x;
    }

    int getY() {
        return enemy.getPosition().y;
    }

    int getRadius() {
        return enemy.getRadius();
    }

    void setPos(Vector2f newPos) {
        enemy.setPosition(newPos);
    }

    void setColRed() {
        enemy.setFillColor(Color::Red);
    }

    void setColBlue() {
        enemy.setFillColor(Color::Blue);
    }

    FloatRect getBounds() {
        return enemy.getGlobalBounds();
    }

private:
    //RectangleShape enemy;
    CircleShape enemy;
};