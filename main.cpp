#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "Enemy.h"
#include "Player.h"
#include <vector>
#include <math.h>
#include <random>

using namespace std;
using namespace sf;

int main()
{
    // Declares start positions and sizes for use later.
    float wilsonStartX = 510.f;
    float wilsonStartY = 800.f;
    float nickStartX = 480.f;
    float nickStartY = 380.f;
    float wilsonSize = 30.f;
    float nickSizeR = 90.f;
    float titleSize = 150;
    float xLineSize= 10.f;
    float yLineSize = 1080.f;
    float xVelocity = 7.5;
    float yVelocity = 7.5;
    float xVelocityNick = 1.5;
    float yVelocityNick = 1.5;
    float collisionDif = wilsonSize + nickSizeR;
    float length;
    float shootLength;
    int levelCounter = 1;
    bool isFiring = false;
    bool levelUp = false;
    String direction;
    Vector2f playerCenter;
    Vector2f enemyCenter;
    Vector2f mosPosWindow;
    Vector2f aimDir;
    Vector2f aimDirNorm;
    float startHealth = 25;
    float health = 25;
    float levelMultiplier = 1.1;
    float currentTime;
    Clock clock;
    float timeSinceLastShot = 0.0f;
    const float minTimeBetweenShots = 0.2f;
    float enemyReappearTime = 0.0f;
    const float enemyReappearDelay = 2.0f;
    float shootInterval = 0.5f;
    float levelShootMultiplier = 0.9;
    float timeSinceLastShoot = 0.0f;
    float enemyBulletSpeed = 4.0f;

    // Define Objects
    Player player(Vector2f(60,60));
    vector<Enemy> enemyVec;
    enemyVec.emplace_back();
    Enemy enemy;
    Bullet b1;
    vector<Bullet> bulletVec;
    vector<Bullet> enemyBulletVec;

    enemy.setPos(Vector2f(nickStartX, nickStartY));
    player.setPos(Vector2f(wilsonStartX, wilsonStartY));

    // Creates a vector to handle movement for wilson.
    Vector2f wilsonPosition(wilsonStartX, wilsonStartY);

    // Creates a vector to handle movement for chimpy.
    Vector2f nickPosition(nickStartX, nickStartY);

    // Creates and renders the start window.
    sf::RenderWindow startWindow(sf::VideoMode(1080, 1080), "Welcome To CHIMPY");

    // Loads our fonts in for use.
    sf::Font font1;
    if(!font1.loadFromFile("Fonts/BungeeShade-Regular.ttf")){
        throw("Could Not Load Font!");
    }

    sf::Font font2;
    if(!font2.loadFromFile("Fonts/PressStart2P-Regular.ttf")){
        throw("Could Not Load Font!");
    }

    // Creates our title
    sf::Text title;
    title.setFont(font1);
    title.setCharacterSize(titleSize);
    title.setColor(sf::Color::Green);
    title.setString("CHIMPY");
    title.setPosition(startWindow.getSize().x/2 - 350, 0.f);

    // Creates our play text.
    Text play;
    play.setFont(font1);
    play.setCharacterSize(35);
    play.setColor(Color::Green);
    play.setString("-PLAY-");
    play.setPosition(startWindow.getSize().x/2 - 75, startWindow.getSize().y/2 - 100);

    // Creates our side bars for title screen.
    RectangleShape sideLine1;
    sideLine1.setSize(Vector2f(xLineSize,yLineSize));
    sideLine1.setFillColor(Color::Green);
    sideLine1.setPosition(0,0);

    RectangleShape sideLine2;
    sideLine2.setSize(sf::Vector2f(xLineSize,yLineSize));
    sideLine2.setFillColor(sf::Color::Green);
    sideLine2.setPosition(1070,0);

    RectangleShape sideLine3;
    sideLine3.setSize(sf::Vector2f(1.f,yLineSize));
    sideLine3.setFillColor(sf::Color::Green);
    sideLine3.setPosition(startWindow.getSize().x/2,0);

    while (startWindow.isOpen())
    {
        // Creates an event to close the window when it is exited out.
        sf::Event event;

        while (startWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                startWindow.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                startWindow.close();
        }

        // Draws and displays the title screen.
        startWindow.clear();
        startWindow.draw(title);
        startWindow.draw(play);
        startWindow.draw(sideLine1);
        startWindow.draw(sideLine2);
        startWindow.display();
    }

    // Creates and renders the window.
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "CHIMPY");
    window.setFramerateLimit(60);

    // Creates Nicks face.
    Texture imageSource;
    if(!imageSource.loadFromFile("Sprites/test180.jpg")){
        throw("Could Not Load Sprite!");
    }
    Sprite nick;
    nick.setTexture(imageSource);
    nick.setPosition(nickStartX, nickStartY);

    // Creates Wilsons face.
    Texture imageSource1;
    if(!imageSource1.loadFromFile("Sprites/60.jpg")){
        throw("Could Not Load Sprite!");
    }
    Sprite wilson;
    wilson.setTexture(imageSource1);
    wilson.setPosition(wilsonStartX, wilsonStartY);

    // Rectangle for boss movement.
    RectangleShape rectangle;
    rectangle.setSize(Vector2f(400,400));
    rectangle.setOutlineColor(Color::White);
    rectangle.setPosition(window.getSize().x/2 - 200,window.getSize().y/2 - 200);

    // Creates our level text.
    Text level;
    level.setFont(font1);
    level.setCharacterSize(35);
    level.setColor(Color::Green);
    level.setString("Level");
    level.setPosition(820, 30);

    // Creates center circle for wilson.
    CircleShape center;
    center.setRadius(1);
    center.setPosition(wilsonPosition.x + wilsonSize,wilsonPosition.y + wilsonSize);
    center.setFillColor(Color::Red);

    // Creates center circle for nick.
    CircleShape center2;
    center2.setRadius(1);
    center2.setPosition(nickPosition.x + nickSizeR, nickPosition.y + nickSizeR);
    center2.setFillColor(Color::Cyan);

    // Screen resolutions for border.
    int WINDOW_W = window.getSize().x;
    int WINDOW_H = window.getSize().y;

    //Starts the clock
    clock.restart();

    while (window.isOpen())
    {
        // Creates an event to close the window when it is exited out.
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();

            if (levelUp){
                levelCounter += 1;
                levelUp = false;
            }
        }

        float deltaTime = clock.restart().asSeconds();
        timeSinceLastShot += deltaTime;
        enemyReappearTime += deltaTime;

        // Updates mouse position
        mosPosWindow = Vector2f(Mouse::getPosition(window));
        playerCenter = Vector2f(player.getX() + player.getRadiusX(), player.getY() + player.getRadiusY());
        enemyCenter = Vector2f(enemy.getX() + enemy.getRadius(), enemy.getY() + enemy.getRadius());
        aimDir = mosPosWindow - playerCenter;
        length = sqrt(pow(aimDir.x,2) + pow(aimDir.y,2));
        aimDirNorm = aimDir / length;
        currentTime = clock.getElapsedTime().asSeconds();

        // Creates our Nick movement.
        if (nickPosition.x < window.getSize().x / 2 - 200 || nickPosition.x > window.getSize().x / 2 + 80) xVelocityNick *= -1;
        if (nickPosition.y < window.getSize().y / 2 - 200 || nickPosition.y > window.getSize().y / 2 + 80) yVelocityNick *= -1;

        nickPosition.x += xVelocityNick;
        nickPosition.y += yVelocityNick;
        nick.setPosition(nickPosition);
        enemy.setPos(Vector2f(nickPosition.x + 15.f, nickPosition.y + 15.f));
        center2.setPosition(nickPosition.x + nickSizeR, nickPosition.y + nickSizeR);

        Text levelNum;
        levelNum.setFont(font1);
        levelNum.setCharacterSize(35);
        levelNum.setColor(Color::Green);
        levelNum.setString(to_string(levelCounter));
        levelNum.setPosition(1000,30);

        // Draws and displays the wilson object.
        window.clear();
        window.draw(wilson);
        window.draw(nick);
        window.draw(level);
        window.draw(levelNum);
        window.draw(sideLine1);
        window.draw(sideLine2);

        // Reads keyboard input and moves the wilson object.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            wilsonPosition.y -= yVelocity;
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            wilsonPosition.y += yVelocity;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            wilsonPosition.x -= xVelocity;
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            wilsonPosition.x += xVelocity;
        }

        // Shooting
        if(Mouse::isButtonPressed(Mouse::Left) && timeSinceLastShot >= minTimeBetweenShots){
            b1.setPos(playerCenter);
            b1.currVelocity = aimDirNorm * b1.maxSpeed;

            bulletVec.push_back(Bullet(b1));
            timeSinceLastShot = 0.0f;
        }

        // Enemy Bullets
        timeSinceLastShoot += deltaTime;

        if (timeSinceLastShoot >= shootInterval) {
            timeSinceLastShoot = 0.0f;

            // Generate a random direction
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 360); // Random angle between 0 and 360 degrees

            float randomAngle = dis(gen);
            float radianAngle = randomAngle * (3.14159265 / 180.0f);

            Vector2f shootDirection(cos(radianAngle), sin(radianAngle));

            Bullet newBullet;
            newBullet.setPos(enemyCenter);
            newBullet.currVelocity = shootDirection * enemyBulletSpeed;

            enemyBulletVec.push_back(newBullet);
        }

        for(int i = 0; i < enemyBulletVec.size(); i++){
            enemyBulletVec[i].bullet.move(enemyBulletVec[i].currVelocity);
            // Out of bounds.
            if(enemyBulletVec[i].bullet.getPosition().x < 0 || enemyBulletVec[i].bullet.getPosition().x > window.getSize().x || enemyBulletVec[i].bullet.getPosition().y < 0 || enemyBulletVec[i].bullet.getPosition().y > window.getSize().y){
                enemyBulletVec.erase(enemyBulletVec.begin() + i);
                i--;
            }

            if(nick.getColor() == Color::Transparent) {
                enemyBulletVec.erase(enemyBulletVec.begin() + i);
                i--;
            }else{
                if(enemyBulletVec[i].bullet.getGlobalBounds().intersects(player.getBounds())){
                    window.close();
                }
            }
        }

        for(int i = 0; i < bulletVec.size(); i++){
            bulletVec[i].bullet.move(bulletVec[i].currVelocity);

            // Out of bounds.
            if(bulletVec[i].bullet.getPosition().x < 0 || bulletVec[i].bullet.getPosition().x > window.getSize().x || bulletVec[i].bullet.getPosition().y < 0 || bulletVec[i].bullet.getPosition().y > window.getSize().y){
                bulletVec.erase(bulletVec.begin() + i);
                i--;
            }

            if(nick.getColor() == Color::Transparent){
                continue;
            }else{
                if(bulletVec[i].bullet.getGlobalBounds().intersects(enemy.getBounds())){
                    bulletVec.erase(bulletVec.begin() + i);
                    health -= 1;
                    if(health <= 0){
                        nick.setColor(Color::Transparent);
                        enemyReappearTime = 0.0f;
                        health = startHealth * levelMultiplier;
                        startHealth = startHealth * levelMultiplier;
                        shootInterval = shootInterval * levelShootMultiplier;
                        levelCounter += 1;
                    }
                }
            }
        }
        if(enemyReappearTime > enemyReappearDelay){
            nick.setColor(Color::White);
        }

        // Update wilson's position.
        wilson.setPosition(wilsonPosition);
        player.setPos(Vector2f(wilsonPosition.x, wilsonPosition.y));
        center.setPosition(wilsonPosition.x + wilsonSize, wilsonPosition.y + wilsonSize);

        // Creates left window border.
        if(wilson.getPosition().x<10){
            wilson.setPosition(10, wilson.getPosition().y);
            player.setPos(Vector2f(10, wilson.getPosition().y));
            center.setPosition(10 + wilsonSize, wilson.getPosition().y + wilsonSize);
            wilsonPosition.x = 10;
            wilsonPosition.y = wilson.getPosition().y;
        }

        // Creates top window border.
        if(wilson.getPosition().y<0){
            wilson.setPosition(wilson.getPosition().x, 0);
            player.setPos(Vector2f(wilson.getPosition().x, 0));
            center.setPosition(wilson.getPosition().x + wilsonSize, 0 + wilsonSize);
            wilsonPosition.x = wilson.getPosition().x;
            wilsonPosition.y = 0;
        }

        // Creates right window border.
        if(wilson.getPosition().x+wilson.getGlobalBounds().width>WINDOW_W - 10){
            wilson.setPosition(WINDOW_W-wilson.getGlobalBounds().width - 10, wilson.getPosition().y);
            player.setPos(Vector2f(WINDOW_W-wilson.getGlobalBounds().width - 10, wilson.getPosition().y));
            center.setPosition(WINDOW_W-wilson.getGlobalBounds().width - 10 + wilsonSize, wilson.getPosition().y + wilsonSize);
            wilsonPosition.x = WINDOW_W-wilson.getGlobalBounds().width - 10;
            wilsonPosition.y = wilson.getPosition().y;
        }

        // Creates bottom window border.
        if(wilson.getPosition().y+wilson.getGlobalBounds().height>WINDOW_H){
            wilson.setPosition(wilson.getPosition().x, WINDOW_H-wilson.getGlobalBounds().height);
            player.setPos(Vector2f(wilson.getPosition().x, WINDOW_H-wilson.getGlobalBounds().height));
            center.setPosition(wilson.getPosition().x + wilsonSize, WINDOW_H-wilson.getGlobalBounds().height + wilsonSize);
            wilsonPosition.x = wilson.getPosition().x;
            wilsonPosition.y = WINDOW_H-wilson.getGlobalBounds().height;
        }

        if(nick.getColor() != Color::Transparent){
            if(abs(center.getPosition().y - center2.getPosition().y) < collisionDif && abs(center.getPosition().x - center2.getPosition().x) < collisionDif){
                window.close();
            }
        }

        for(auto & i : bulletVec) {
            i.draw(window);
        }

        for(auto & i : enemyBulletVec) {
            i.draw(window);
        }

        window.display();
    }

    // Creates and renders the window.
    sf::RenderWindow gameOver(sf::VideoMode(1080, 1080), "GAME OVER");

    while (gameOver.isOpen())
    {
        // Creates an event to close the window when it is exited out.
        sf::Event event;

        while (gameOver.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameOver.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                gameOver.close();
        }

        // Creates Nicks face for game over.
        Texture imageSource1;
        if(!imageSource1.loadFromFile("Sprites/Original.png")){
            throw("Could Not Load Sprite!");
        }
        Sprite imageSprite1;
        imageSprite1.setTexture(imageSource1);

        // Draws and displays the wilson object.
        gameOver.clear();
        gameOver.draw(imageSprite1);
        gameOver.display();
    }
}
