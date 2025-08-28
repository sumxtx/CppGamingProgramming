#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
  VideoMode vm(1920, 1080);

  RenderWindow window(vm, "Timber", Style::Fullscreen);
  
  Texture textureBackground;
  textureBackground.loadFromFile("graphics/background.png");
  
  Sprite spriteBackground;

  spriteBackground.setTexture(textureBackground);

  while(window.isOpen())
  {
    if(Keyboard::isKeyPressed(Keyboard::Escape))
    {
      window.close();
    }

    window.clear();

    window.draw(spriteBackground);

    window.display();
  }
}
