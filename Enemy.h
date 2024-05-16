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
//    Enemy(sf::Vector2f size) {
//        enemy.setSize(size);
//        enemy.setFillColor(Color::Red);
//    }

    Enemy(float radius = 75.f) {
        enemy.setRadius(radius);
        enemy.setFillColor(Color::Blue);
//        enemy.setOutlineThickness(10.f);
//        enemy.setOutlineColor(Color::Red);
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

//    bool checkColl(Bullet bullet) {
//        if(bullet.getRight() > enemy.getPosition().x && bullet.getTop() < enemy.getPosition().y + enemy.getSize().y && bullet.getBottom() > enemy.getPosition().y && bullet.getLeft() < enemy.getPosition().x + enemy.getSize().x) {
////            enemy.setPosition(Vector2f(800,800));
//            enemy.setFillColor(Color::Transparent);
//            std::cout << "HIT" << std::endl;
//            //bullet.setPos(Vector2f(10000,10000));
//            return true;
//        } else {
//            return false;
//        }
//    }

    void shoot(Vector2f targetPosition, std::vector<Bullet> &bulletVec, float elapsedTime) {
        timeSinceLastShoot += elapsedTime;

        if (timeSinceLastShoot >= shootInterval) {
            timeSinceLastShoot = 0.0f;

            Vector2f shootDirection = targetPosition - enemy.getPosition();
            float shootDirectionLength = sqrt(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
            shootDirection /= shootDirectionLength;

            Bullet newBullet;
            newBullet.setPos(enemy.getPosition());
            newBullet.currVelocity = shootDirection * newBullet.maxSpeed;

            bulletVec.push_back(newBullet);
        }
    }

    void draw(RenderWindow &window) {
        window.draw(enemy);
    }
private:
    //RectangleShape enemy;
    CircleShape enemy;
    float shootInterval = 1.0f;  // Adjust this value for enemy shooting rate
    float timeSinceLastShoot = 0.0f;
};