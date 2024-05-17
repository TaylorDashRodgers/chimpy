//
// Created by Taylor Rodgers on 11/5/22.
//

#ifndef CHIMPYGAME_PLAYER_H
#define CHIMPYGAME_PLAYER_H

#endif //CHIMPYGAME_PLAYER_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
public:
    Player(Vector2f size) {
        player.setSize(size);
    }

    void setPos(Vector2f newPos) {
        player.setPosition(newPos);
    }

    int getX() {
        return player.getPosition().x;
    }

    int getY() {
        return player.getPosition().y;
    }

    int getRadiusX() {
        return player.getSize().x/2;
    }

    int getRadiusY() {
        return player.getSize().y/2;
    }

    FloatRect getBounds() {
        return player.getGlobalBounds();
    }

    void draw(RenderWindow &window) {
        window.draw(player);
    }
private:
    RectangleShape player;
};
