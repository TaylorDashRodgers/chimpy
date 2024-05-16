#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "Enemy.h"
#include "Player.h"
#include <vector>
#include <math.h>

using namespace std;
using namespace sf;

int main()
{
    // Declares start positions and sizes for use later.
    float wilsonStartX = 510.f;
    float wilsonStartY = 800.f;
    float chimpyStartX = 480.f;
    float chimpyStartY = 380.f;
    float wilsonSize = 30.f;
    float chimpySize = 60.f;
    float nickSizeR = 90.f;
    float titleSize = 150;
    float xLineSize= 10.f;
    float yLineSize = 1080.f;
    float xVelocity = 7.5;
    float yVelocity = 7.5;
    float xVelocityChimp = 1.5;
    float yVelocityChimp = 1.5;
    float collisionDif = wilsonSize + nickSizeR;
    float length;
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
    //float reappear;
    float currentTime;
    Clock clock;
    float timeSinceLastShot = 0.0f;
    const float minTimeBetweenShots = 0.2f;
    float enemyReappearTime = 0.0f;
    const float enemyReappearDelay = 2.0f;

    // Define Objects
    Player player(Vector2f(60,60));
//    Enemy enemy(Vector2f(160,160));
    vector<Enemy> enemyVec;
    enemyVec.emplace_back();
    Enemy enemy;
    Bullet b1;
    vector<Bullet> bulletVec;

    enemyCenter = Vector2f(enemy.getX() + enemy.getRadius(), enemy.getY() + enemy.getRadius());

    enemy.setPos(enemyCenter);
    player.setPos(Vector2f(wilsonStartX, wilsonStartY));

    // Creates a vector to handle movement for wilson.
    Vector2f wilsonPosition(wilsonStartX, wilsonStartY);

    // Creates a vector to handle movement for chimpy.
    Vector2f chimpyPosition(chimpyStartX, chimpyStartY);

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
        //startWindow.draw(sideLine3);
        startWindow.display();
    }

    // Creates and renders the window.
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "CHIMPY");
    window.setFramerateLimit(60);

    // Creates the wilson object.
//    sf::CircleShape wilson;
//    wilson.setRadius(wilsonSize);
//    wilson.setPosition(wilsonPosition);
//    wilson.setFillColor(sf::Color::Cyan);

    // Creates the Chimpy object.
    sf::CircleShape chimpy;
    chimpy.setRadius(chimpySize);
    chimpy.setPosition(chimpyStartX, chimpyStartY);
    chimpy.setFillColor(sf::Color::Red);

    // Creates Nicks face.
    Texture imageSource;
    if(!imageSource.loadFromFile("Sprites/test180.jpg")){
        throw("Could Not Load Sprite!");
    }
    Sprite nick;
    nick.setTexture(imageSource);
    nick.setPosition(chimpyStartX, chimpyStartY);

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

    // Testing center.
    RectangleShape line;
    line.setSize(Vector2f(1,1080));
    line.setFillColor(Color::Green);
    line.setPosition(window.getSize().x/2,0);
    RectangleShape line1;
    line1.setSize(Vector2f(1080,1));
    line1.setFillColor(Color::Green);
    line1.setPosition(0,window.getSize().y/2);

    // Creates our level text.
    Text level;
    level.setFont(font1);
    level.setCharacterSize(35);
    level.setColor(Color::Green);
    level.setString("Level");
    level.setPosition(820, 30);

//    Text levelNum;
//    levelNum.setFont(font1);
//    levelNum.setCharacterSize(35);
//    levelNum.setColor(Color::Green);
//    levelNum.setString(to_string(levelCounter));
//    levelNum.setPosition(1000,30);

    // Creates center circle for wilson.
    CircleShape center;
    center.setRadius(1);
    center.setPosition(wilsonPosition.x + wilsonSize,wilsonPosition.y + wilsonSize);
    center.setFillColor(Color::Red);

    // Creates center circle for chimpy.
    CircleShape center2;
    center2.setRadius(1);
    center2.setPosition(chimpyPosition.x + nickSizeR,chimpyPosition.y + nickSizeR);
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

//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
//                isFiring = true;
//                //bullDirection = 0.3;
//                //isRight = true;
//                direction = "Right";
//            }

//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
//                isFiring = true;
//                //bullDirection = -0.3;
//                //isRight = false;
//                direction = "Left";
//            }

//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) {
//                isFiring = true;
//                //bullDirection = 0.3;
//                //isRight = true;
//                direction = "Up";
//            }

//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down) {
//                isFiring = true;
//                //bullDirection = 0.3;
//                //isRight = true;
//                direction = "Down";
//            }

            if (levelUp){
                levelCounter += 1;

                //usleep(75000);
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

        // Creates our Chimpy movement.
        if (chimpyPosition.x < window.getSize().x/2 - 200 || chimpyPosition.x > window.getSize().x/2 + 80) xVelocityChimp *= -1;
        if (chimpyPosition.y < window.getSize().y/2 - 200 || chimpyPosition.y > window.getSize().y/2 + 80) yVelocityChimp *= -1;

        chimpyPosition.x += xVelocityChimp;
        chimpyPosition.y += yVelocityChimp;
        chimpy.setPosition(chimpyPosition);
        nick.setPosition(chimpyPosition);
        enemy.setPos(Vector2f(chimpyPosition.x + 15.f, chimpyPosition.y + 15.f));
        center2.setPosition(chimpyPosition.x + nickSizeR,chimpyPosition.y + nickSizeR);

        Text levelNum;
        levelNum.setFont(font1);
        levelNum.setCharacterSize(35);
        levelNum.setColor(Color::Green);
        levelNum.setString(to_string(levelCounter));
        levelNum.setPosition(1000,30);

//        if (Keyboard::isKeyPressed(Keyboard::L)){
//            levelCounter += 1;
//            usleep(75000);
//        }

//        if (levelUp){
//            levelCounter += 1;
//            usleep(75000);
//            levelUp = false;
//        }

        // Draws and displays the wilson object.
        window.clear();
        player.draw(window);
        //window.draw(wilson);
        window.draw(wilson);
        window.draw(nick);
        //enemy.draw(window);
        //window.draw(rectangle);
        //window.draw(line);
        //window.draw(line1);
        //window.draw(chimpy);
        window.draw(level);
        window.draw(levelNum);
        window.draw(sideLine1);
        window.draw(sideLine2);
        window.draw(center);
        window.draw(center2);
//        window.display();

        // Reads keyboard input and moves the wilson object.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            //wilson.move(0, -0.5);
            wilsonPosition.y -= yVelocity;
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            //wilson.move(0, 0.5);
            wilsonPosition.y += yVelocity;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            //wilson.move(-0.5, 0);
            wilsonPosition.x -= xVelocity;
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            //wilson.move(0.5, 0);
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
        for(auto &enemy : enemyVec) {
            enemy.shoot(playerCenter, bulletVec, deltaTime);
            enemy.draw(window);
        }

        for(int i = 0; i < bulletVec.size(); i++){
            bulletVec[i].bullet.move(bulletVec[i].currVelocity);

            // Out of bounds.
            if(bulletVec[i].bullet.getPosition().x < 0 || bulletVec[i].bullet.getPosition().x > window.getSize().x || bulletVec[i].bullet.getPosition().y < 0 || bulletVec[i].bullet.getPosition().y > window.getSize().y){
                bulletVec.erase(bulletVec.begin() + i);
                i--;
            }

            if(nick.getColor() == Color::Transparent){
                cout << "Can't hit me." << endl;
                continue;
            }else{
                if(bulletVec[i].bullet.getGlobalBounds().intersects(enemy.getBounds())){
                    bulletVec.erase(bulletVec.begin() + i);
                    health -= 1;
                    cout << health << endl;
                    if(health <= 0){
                        cout << "DONE" << endl;
                        //enemy.setColRed();
//                    sleep(2);
//                    enemy.setColBlue();
                        nick.setColor(Color::Transparent);
                        //reappear = currentTime + 2;
                        enemyReappearTime = 0.0f;
                        health = startHealth * levelMultiplier;
                        startHealth = startHealth * levelMultiplier;
                        levelCounter += 1;
                    }
                }
            }
        }
        if(enemyReappearTime > enemyReappearDelay){
            //enemy.setColBlue();
            nick.setColor(Color::White);
        }

        // Update wilson's position.
        //wilson.setPosition(wilsonPosition);
        wilson.setPosition(wilsonPosition);
        player.setPos(Vector2f(wilsonPosition.x, wilsonPosition.y));
        center.setPosition(wilsonPosition.x + wilsonSize, wilsonPosition.y + wilsonSize);

        // Creates left window border.
        if(wilson.getPosition().x<10){
            //wilson.setPosition(10,wilson.getPosition().y);
            wilson.setPosition(10, wilson.getPosition().y);
            player.setPos(Vector2f(10, wilson.getPosition().y));
            center.setPosition(10 + wilsonSize, wilson.getPosition().y + wilsonSize);
            wilsonPosition.x = 10;
            wilsonPosition.y = wilson.getPosition().y;
        }

        // Creates top window border.
        if(wilson.getPosition().y<0){
            //wilson.setPosition(wilson.getPosition().x, 0);
            wilson.setPosition(wilson.getPosition().x, 0);
            player.setPos(Vector2f(wilson.getPosition().x, 0));
            center.setPosition(wilson.getPosition().x + wilsonSize, 0 + wilsonSize);
            wilsonPosition.x = wilson.getPosition().x;
            wilsonPosition.y = 0;
        }

        // Creates right window border.
        if(wilson.getPosition().x+wilson.getGlobalBounds().width>WINDOW_W - 10){
            //wilson.setPosition(WINDOW_W-wilson.getGlobalBounds().width - 10, wilson.getPosition().y);
            wilson.setPosition(WINDOW_W-wilson.getGlobalBounds().width - 10, wilson.getPosition().y);
            player.setPos(Vector2f(WINDOW_W-wilson.getGlobalBounds().width - 10, wilson.getPosition().y));
            center.setPosition(WINDOW_W-wilson.getGlobalBounds().width - 10 + wilsonSize, wilson.getPosition().y + wilsonSize);
            wilsonPosition.x = WINDOW_W-wilson.getGlobalBounds().width - 10;
            wilsonPosition.y = wilson.getPosition().y;
        }

        // Creates bottom window border.
        if(wilson.getPosition().y+wilson.getGlobalBounds().height>WINDOW_H){
            //wilson.setPosition(wilson.getPosition().x, WINDOW_H-wilson.getGlobalBounds().height);
            wilson.setPosition(wilson.getPosition().x, WINDOW_H-wilson.getGlobalBounds().height);
            player.setPos(Vector2f(wilson.getPosition().x, WINDOW_H-wilson.getGlobalBounds().height));
            center.setPosition(wilson.getPosition().x + wilsonSize, WINDOW_H-wilson.getGlobalBounds().height + wilsonSize);
            wilsonPosition.x = wilson.getPosition().x;
            wilsonPosition.y = WINDOW_H-wilson.getGlobalBounds().height;
        }

        if(abs(center.getPosition().y - center2.getPosition().y) < collisionDif && abs(center.getPosition().x - center2.getPosition().x) < collisionDif){
            window.close();
        }

//        if(isFiring) {
//            Bullet newBullet(Vector2f(10,10), direction);
//            newBullet.setPos(Vector2f(player.getX() + 30, player.getY() + 30));
//            bulletVec.push_back(newBullet);
//            isFiring = false;
//        }

        for(auto & i : bulletVec) {
            i.draw(window);
        }

//        for(auto & i : bulletVec) {
//            levelUp = enemy.checkColl(i);
//        }

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
