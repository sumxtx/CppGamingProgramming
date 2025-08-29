#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Bat
{

  private:
    
    Vector2f m_Position;
    RectangleShape m_Shape;
    
    bool m_MovingRight = false;
    bool m_MovingLeft = false;
    float m_Speed = 1000.0f;

  public:

    Bat(float startX, float startY);
    FloatRect getPosition();
    RectangleShape getShape();

    void moveLeft();
    void moveRight();
    void stopLeft();
    void stopRight();
    void update(Time dt);
};
