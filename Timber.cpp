#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
  /* Set the application configurations, Name, Window Size, Window Mode */
  std::string app_name = "Timber";
  int win_size_x = 1920;
  int win_size_y = 1080;
  VideoMode vm(win_size_x, win_size_y);

  /* Setting the Font configuration */
  std::string fontPath = "fonts/KOMIKAP_.ttf";
  Font font;
  font.loadFromFile(fontPath);
  
  /* Start Game Message */
  std::string textStartText = "Press Enter To Start";
  Text startText;
  startText.setFont(font);
  startText.setString(textStartText);
  startText.setCharacterSize(75);
  startText.setFillColor(Color::White);

  FloatRect startTextRect = startText.getLocalBounds();
  startText.setOrigin(
      startTextRect.left + 
      startTextRect.width /2.0f,
      startTextRect.top + 
      startTextRect.height / 2.0f);

  startText.setPosition(1920 / 2.0f, 1080 / 2.0f);


  /* Score Message */
  int score = 0;
  Text scoreText;
  scoreText.setFont(font);
  scoreText.setString("Score = 0");
  scoreText.setCharacterSize(100);
  scoreText.setFillColor(Color::White);
  scoreText.setPosition(20,20);

  RenderWindow window(vm, app_name, Style::Fullscreen);

  /* Setting the player Data */
  std::string playerName = "Player1";
  std::string message = "Welcome to the game" + playerName + "!";
  std::cout << message << std::endl;

  /* Create the Texture background set Texture to an Sprite */
  Texture textureBG;
  std::string backgroundTexturePath = "graphics/background.png";
  textureBG.loadFromFile(backgroundTexturePath);

  Sprite spriteBG;
  spriteBG.setTexture(textureBG);

  /* Create the Texture tree set the Texture to an Sprite and set its position on the window*/

  int tree_position_x = 810;
  int tree_position_y = 0;
  std::string textureTreePath = "graphics/tree.png";
  Texture textureTree;
  textureTree.loadFromFile(textureTreePath);

  Sprite spriteTree;
  spriteTree.setTexture(textureTree);
  spriteTree.setPosition(tree_position_x, tree_position_y);

  /* Create the Texture bee and set the Texture to an Sprite and set its position on the window */

  int bee_position_x = 0;
  int bee_position_y = 800;
  std::string textureBeePath = "graphics/bee.png";
  Texture textureBee;
  textureBee.loadFromFile(textureBeePath);

  Sprite spriteBee;
  spriteBee.setTexture(textureBee);
  spriteBee.setPosition(bee_position_x, bee_position_y);

  bool beeActive = false;
  float beeSpeed = 0.0f;

  /* Create the Texture cloud and cloud Sprites */
  std::string textureCloudPath = "graphics/cloud.png";

  Texture textureCloud;
  textureCloud.loadFromFile(textureCloudPath);

  Sprite spriteCloud1;
  Sprite spriteCloud2;
  Sprite spriteCloud3;

  spriteCloud1.setTexture(textureCloud);
  spriteCloud2.setTexture(textureCloud);
  spriteCloud3.setTexture(textureCloud);

  spriteCloud1.setPosition(0,0);
  spriteCloud2.setPosition(0,250);
  spriteCloud3.setPosition(0,500);

  bool cloud1Active = false;
  bool cloud2Active = false;
  bool cloud3Active = false;

  float cloud1Speed = 0.0f;
  float cloud2Speed = 0.0f;
  float cloud3Speed = 0.0f;


  /* Creates the clock object */
  Clock clock;

  /* Create the time remaining Bar */
  RectangleShape timeBar;
  float timeBarStartWidth = 400;
  float timeBarHeight = 80;
  timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
  timeBar.setFillColor(Color::Red);
  timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

  Time gameTimeTotal;
  float timeRemaining = 6.0f;
  float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

  bool paused = true;

  while(window.isOpen())
  {
    if(Keyboard::isKeyPressed(Keyboard::Escape))
    {
      window.close();
    }

    if(Keyboard::isKeyPressed(Keyboard::Return))
    {
      paused = false;
      score = 0;
      timeRemaining = 6;
    }

    if(!paused)
    {

      Time dt = clock.restart();

      timeRemaining -= dt.asSeconds();
      timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

      if(timeRemaining <= 0.0f)
      {
        paused = true;

        startText.setString("Out of time!");

        FloatRect textRect = startText.getLocalBounds();
        startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        startText.setPosition(1920 / 2.0f, 1080 /2.0f);
      }

      if(!beeActive)
      {

        srand((int)time(0) * 10);
        beeSpeed = (rand() % 200) + 200;

        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;
        spriteBee.setPosition(2000, height);
        beeActive = true;
      }
      else
      {
        spriteBee.setPosition(
            spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
            spriteBee.getPosition().y );
        if (spriteBee.getPosition().x < -100)
        {
          beeActive = false;
        }
      }

      if(!cloud1Active)
      {
        srand((int)time(0) * 10);
        cloud1Speed = (rand() % 200);
        srand((int)time(0) * 10);
        float height = (rand() % 150);
        spriteCloud1.setPosition(-200, height);
        cloud1Active = true;
      }
      else
      {
        spriteCloud1.setPosition(
            spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
            spriteCloud1.getPosition().y);
        if (spriteCloud1.getPosition().x > 1920)
        {
          cloud1Active = false;
        }
      }

      if(!cloud2Active)
      {
        srand((int)time(0) * 20);
        cloud2Speed = (rand() % 200);
        srand((int)time(0) * 20);
        float height = (rand() % 150);
        spriteCloud2.setPosition(-200, height);
        cloud2Active = true;
      }
      else
      {
        spriteCloud2.setPosition(
            spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
            spriteCloud2.getPosition().y);
        if (spriteCloud2.getPosition().x > 1920)
        {
          cloud2Active = false;
        }
      }

      if(!cloud3Active)
      {
        srand((int)time(0) * 30);
        cloud3Speed = (rand() % 200);
        srand((int)time(0) * 30);
        float height = (rand() % 150);
        spriteCloud3.setPosition(-200, height);
        cloud3Active = true;
      }
      else
      {
        spriteCloud3.setPosition(
            spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
            spriteCloud3.getPosition().y);
        if (spriteCloud3.getPosition().x > 1920)
        {
          cloud3Active = false;
        }
      }

      std::stringstream ss_score;
      ss_score << "Score = " << score;
      scoreText.setString(ss_score.str());

    } // End if(!paused)
    window.clear();

    window.draw(spriteBG);
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);
    window.draw(spriteTree);
    window.draw(spriteBee);

    window.draw(scoreText);
    window.draw(timeBar);
    if(paused)
    {
      window.draw(startText);
    }

    window.display();
  }
}
