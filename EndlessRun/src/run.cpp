#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include "GameObject.hpp"
#include "Factory.hpp"
#include "InputDispatcher.hpp"

using namespace std;
using namespace sf;

int main()
{
  /* Fullscreen Window */
  RenderWindow window(VideoMode::getDesktopMode(),"Booster", Style::Fullscreen);
  
  /* Holds all graphics */
  VertexArray canvas(Quads,0);
  
  /* Dispatch events to any object */
  InputDispatcher inputDispatcher(&window);
  
  /* Everything will be a game object */
  vector<GameObject> gameObjects;
  
  /* Construct game objects */
  Factory factory(&window);
  
  /* Setup the game at the factory with the objects, the canvas to drawn on and the events */
  factory.loadLevel(gameObjects, canvas, inputDispatcher);
  
  /* timing */
  Clock clock;
  
  /* the default background */
  const Color BACKGROUND_COLOR(100, 100, 100, 255);
  

  /** Main Game Loop **/
  while(window.isOpen())
  {
    /* Time Frames Take */
    float timeTakenInSeconds = clock.restart().asSeconds();
    
    /* Handle input events */
    inputDispatcher.dispatchInputEvents();
    
    /* Clear previous frames*/
    window.clear(BACKGROUND_COLOR);

    /* Update game objects */
    for(auto &gameObject : gameObjects)
    {
      gameObject.update(timeTakenInSeconds);
    }

    /* Draw all the game objects to the canvas */
    for(auto &gameObject : gameObjects)
    {
      gameObject.draw(canvas);
    }

    /*for test only */
    window.draw(canvas, factory.m_Texture);


    /* Show the new frame */
    window.display();
  }
  return 0;
}
