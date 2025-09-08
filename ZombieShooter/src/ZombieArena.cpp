#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ZombieArena.hpp"
#include "Player.hpp"
#include "TextureHolder.hpp"
#include "Bullet.hpp"
#include "Pickup.hpp"

using namespace sf;

int main()
{
  /* TextureHolder instance */
  TextureHolder holder;

  /* Different Game States */
  enum class State 
  {
    PAUSED,
    LEVELING_UP,
    GAME_OVER,
    PLAYING
  };

  /* Initial Game State */
  State state = State::GAME_OVER;

  /* Screen resolution and SFML Window */
  Vector2f resolution;
  resolution.x = VideoMode::getDesktopMode().width;
  resolution.y = VideoMode::getDesktopMode().height;
  RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

  /* SFML View for the main action*/
  View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

  /* Main Clock */
  Clock clock;

  /* PLAYING game state duration*/
  Time gameTimeTotal;

  /* Mouse in relation to world */
  Vector2f mouseWorldPosition;
  /* Mouse in relation to the screen */
  Vector2i mouseScreenPosition;

  /* Instance of the Player Class */
  Player player;

  /* Arena  Boundaries */
  IntRect arena;

  /* Create the background */
  VertexArray background;
  Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

  /* Create the Zombies*/
  int numZombies;
  int numZombiesAlive;
  Zombie* zombies = nullptr;

  /* Handle the bullets */
  Bullet bullets[100];
  int currentBullet = 0;
  int bulletsSpare = 24;
  int bulletsInClip = 6;
  int clipSize = 6;
  float fireRate = 1;
  Time lastPressed;

  /* Show the crosshair instead of the mouse pointer */
  window.setMouseCursorVisible(true);
  Sprite spriteCrosshair;
  Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
  spriteCrosshair.setTexture(textureCrosshair);
  spriteCrosshair.setOrigin(25,25);

  /* Health and Ammo pickups */ //TODO: Maybe add an armor pickup
  Pickup healthPickup(1);
  Pickup ammoPickup(2);

  /* Score */
  int score = 0;
  int hiScore = 0;

  /* Home and Game Over screen */
  Sprite spriteGameOver;
  Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
  spriteGameOver.setTexture(textureGameOver);
  spriteGameOver.setPosition(0,0);

  /* Hud superpositioned View */
  View hudView(sf::FloatRect(0,0,1920,1080));

  /* Ammo Textures */
  Sprite spriteAmmoIcon;
  Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
  spriteAmmoIcon.setTexture(textureAmmoIcon);
  spriteAmmoIcon.setPosition(20, 980);

  /* Font */
  Font font;
  font.loadFromFile("fonts/zombiecontrol.ttf");

  /* Game Paused Text */
  Text pausedText;
  pausedText.setFont(font);
  pausedText.setCharacterSize(155);
  pausedText.setFillColor(Color::White);
  pausedText.setPosition(400,400);
  pausedText.setString("Press Enter \nto Continue");

  /* Game Over Text */
  Text gameOverText;
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(135);
  gameOverText.setFillColor(Color::White);
  gameOverText.setPosition(250,850);
  gameOverText.setString("Press Enter to Play");

  /* Level Up Text */
  Text levelUpText;
  levelUpText.setFont(font);
  levelUpText.setCharacterSize(80);
  levelUpText.setFillColor(Color::White);
  levelUpText.setPosition(150,250);
  std::stringstream levelUpStream;
  levelUpStream << 
    "1 - Increase rate of fire\n" <<
    "2 - Increase clip size (next reload)\n" <<
    "3 - Increase max health\n" <<
    "4 - Increase run speed\n" <<
    "5 - More and better health pickups\n" <<
    "6 - More and better ammo pickups";
  levelUpText.setString(levelUpStream.str());

  /* Ammo Text */
  Text ammoText;
  ammoText.setFont(font);
  ammoText.setCharacterSize(55);
  ammoText.setFillColor(Color::White);
  ammoText.setPosition(200,980);

  /* Score Text */
  Text scoreText;
  scoreText.setFont(font);
  scoreText.setCharacterSize(55);
  scoreText.setFillColor(Color::White);
  scoreText.setPosition(20,0);

  std::ifstream inputFile("gamedata/scores.txt");
  if(inputFile.is_open())
  {
    inputFile >> hiScore;
    inputFile.close();
  }

  /* High Score Teext */
  Text hiScoreText;
  hiScoreText.setFont(font);
  hiScoreText.setCharacterSize(55);
  hiScoreText.setFillColor(Color::White);
  hiScoreText.setPosition(1400,0);
  std::stringstream s;
  s << "Hi Score: " << hiScore;
  hiScoreText.setString(s.str());

  /* Zombies Remaining Text */
  Text zombiesRemainingText;
  zombiesRemainingText.setFont(font);
  zombiesRemainingText.setCharacterSize(55);
  zombiesRemainingText.setFillColor(Color::White);
  zombiesRemainingText.setPosition(1500,98);
  zombiesRemainingText.setString("Zombies: 100");

  /* Waves Text */
  int wave = 0;
  Text waveNumberText;
  waveNumberText.setFont(font);
  waveNumberText.setCharacterSize(55);
  waveNumberText.setFillColor(Color::White);
  waveNumberText.setPosition(1250, 980);
  waveNumberText.setString("Wave: 0");

  /* Health Bar Display */
  RectangleShape healthBar;
  healthBar.setFillColor(Color::Red);
  healthBar.setPosition(450, 980);

  /* HUD Update timing */
  int framesSinceLastHUDUpdate = 0;
  int fpsMeasurementFrameInterval = 1000;

  /* Sounds */
  SoundBuffer hitBuffer;
  hitBuffer.loadFromFile("sound/hit.wav");
  Sound hit;
  hit.setBuffer(hitBuffer);

  SoundBuffer splatBuffer;
  splatBuffer.loadFromFile("sound/splat.wav");
  Sound splat;
  splat.setBuffer(splatBuffer);

  SoundBuffer shootBuffer;
  shootBuffer.loadFromFile("sound/shoot.wav");
  Sound shoot;
  shoot.setBuffer(shootBuffer);

  SoundBuffer reloadBuffer;
  reloadBuffer.loadFromFile("sound/reload.wav");
  Sound reload;
  reload.setBuffer(reloadBuffer);

  SoundBuffer reloadFailedBuffer;
  reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
  Sound reloadFailed;
  reloadFailed.setBuffer(reloadFailedBuffer);

  SoundBuffer powerupBuffer;
  powerupBuffer.loadFromFile("sound/powerup.wav");
  Sound powerup;
  powerup.setBuffer(powerupBuffer);
  SoundBuffer pickupBuffer;
  pickupBuffer.loadFromFile("sound/pickup.wav");
  Sound pickup;
  pickup.setBuffer(pickupBuffer);


  /** Main Game Loop **/
  int upAtBegin = 0;

  while(window.isOpen())
  {
    /* Handle Input */
    Event event;
    while(window.pollEvent(event))
    {
      /* Handle Keys being Pressed */
      if(event.type == Event::KeyPressed)
      {
        /* Game Flow (Pause, Continue, New Game) */
        /* Enter Key Pauses the game */
        if(event.key.code == Keyboard::Return && state == State::PLAYING)
        {
          state = State::PAUSED;
        }
        /* Enter Key when Paused continues the game */
        else if(event.key.code == Keyboard::Return && state == State::PAUSED)
        {
          state = State::PLAYING;
          clock.restart(); // resets the clock (avoid frame jump);
        }
        /* Starts a New Game when in Game Over State */
        else if(event.key.code == Keyboard::Return && state == State::GAME_OVER)
        {
          state = State::LEVELING_UP;

          wave = 0;
          score = 0;
          currentBullet = 0;
          bulletsSpare = 24;
          bulletsInClip = 6;
          clipSize = 6;
          fireRate = 1;
          player.resetPlayerStats();
        }

        /* Game is Running */
        if (state == State::PLAYING)
        {
          if(event.key.code == Keyboard::R)
          {
            if(bulletsSpare >= clipSize)
            {
              bulletsInClip = clipSize;
              bulletsSpare -= clipSize;
              reload.play();
            }
            else if(bulletsSpare > 0)
            {
              bulletsInClip = bulletsSpare;
              bulletsSpare = 0;
              reload.play();
            }
            else
            {
              // No Ammo
              reloadFailed.play();
            }
          }
        }
      }
    }

    /* Esc Quit the Game */
    if(Keyboard::isKeyPressed(Keyboard::Escape))
    {
      window.close();
    }

    /* Player Movement with WASD */
    if(state == State::PLAYING)
    {
      if(Keyboard::isKeyPressed(Keyboard::W))
      {
        player.moveUp();
      }
      else
      {
        player.stopUp();
      }
      if(Keyboard::isKeyPressed(Keyboard::S))
      {
        player.moveDown();
      }
      else
      {
        player.stopDown();
      }
      if(Keyboard::isKeyPressed(Keyboard::A))
      {
        player.moveLeft();
      }
      else
      {
        player.stopLeft();
      }
      if(Keyboard::isKeyPressed(Keyboard::D))
      {
        player.moveRight();
      }
      else
      {
        player.stopRight();
      }

      /* Shooting */
      if(Mouse::isButtonPressed(sf::Mouse::Left))
      {
        if(gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate)
        {
          if(bulletsInClip > 0)
          {
            bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
            currentBullet++;
            if(currentBullet > 99)
            {
              currentBullet = 0;
            }
            lastPressed = gameTimeTotal;
            shoot.play();
            bulletsInClip--;
          }
          else
          {
            reload.play();
          }
        }
      }
    }

    /* Handle the Level up (End of wach Wave state)*/
    if(state == State::LEVELING_UP)
    {
      if(event.key.code == Keyboard::Num1)
      {
        fireRate++;
        state = State::PLAYING;
      }
      if(event.key.code == Keyboard::Num2)
      {
        clipSize += clipSize;
        state = State::PLAYING;
      }
      if(event.key.code == Keyboard::Num3)
      {
        player.upgradeHealth();
        state = State::PLAYING;
      }
      if(event.key.code == Keyboard::Num4)
      {
        player.upgradeSpeed();
        state = State::PLAYING;
      }
      if(event.key.code == Keyboard::Num5)
      {
        healthPickup.upgrade();
        state = State::PLAYING;
      }
      if(event.key.code == Keyboard::Num6)
      {
        ammoPickup.upgrade();
        state = State::PLAYING;
      }

      if(state == State::PLAYING)
      {

        wave++;

        /* Create the Arena and place the player on it's center */
        /* Max arena size, to avoid expanding forever */
        if (wave <= 6) 
        {
          arena.width = 500 * wave;
          arena.height = 500 * wave;
        }
        else
        {
          arena.width = 500 * 6;
          arena.height = 500 * 6;
        }
        arena.left = 0;
        arena.top = 0;
        int tileSize = createBackground(background, arena);
        player.spawn(arena, resolution, tileSize);

        /* Prepare the Pickups */
        healthPickup.setArena(arena);
        ammoPickup.setArena(arena);

        /* Prepare the Zombies Spawn */
        numZombies = 5 * wave;
        /*deletes previously allocated zombies */
        delete[] zombies;
        zombies = createHorde(numZombies, arena, wave);
        numZombiesAlive = numZombies;

        powerup.play();

        /* Avoid fram jump */
        clock.restart();
      }
    }

    if(state == State::PLAYING)
    {
      /* Update delta time */
      Time dt = clock.restart();

      /* Update total game time */
      gameTimeTotal += dt;

      /* Make a fraction of 1 second from the delta time */
      float dtAsSeconds = dt.asSeconds();

      /* Get Mouse Pointer */
      mouseScreenPosition = Mouse::getPosition(window);

      /* Convert mouse position to world coordinate position */
      mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window), mainView);

      /* Set Crosshair to world position */
      spriteCrosshair.setPosition(mouseWorldPosition);

      /* Update the player pointing and locatio postions */
      player.update(dtAsSeconds, mouseWorldPosition);
      Vector2f playerPosition(player.getCenter());
      mainView.setCenter(player.getCenter());

      /*Update Zombies and Bullets position*/
      for(int i = 0; i < numZombies; i++)
      {
        if(zombies[i].isAlive())
        {
          zombies[i].update(dt.asSeconds(), playerPosition);
        }
      }
      for(int i = 0; i < 100; i++)
      {
        if(bullets[i].isInFlight())
        {
          bullets[i].update(dtAsSeconds);
        }
      }

      /* Update pickups position */
      healthPickup.update(dtAsSeconds);
      ammoPickup.update(dtAsSeconds);

      /* Zombies Collision Detection (Zombies being shot)*/
      for(int i = 0; i < 100; i++)
      {
        for(int j = 0; j < numZombies; j++)
        {
          if(bullets[i].isInFlight() && zombies[j].isAlive())
          {
            if(bullets[i].getPosition().intersects(zombies[j].getPosition()))
            {
              bullets[i].stop();
              if(zombies[j].hit())
              {
                score += 10;
                if(score >= hiScore)
                {
                  hiScore = score;
                }
                numZombiesAlive--;
                if(numZombiesAlive == 0)
                {
                  state = State::LEVELING_UP;
                }
              }
              splat.play();
            }
          }
        }
      }

      /* Player Collision Detection (Zombies hit the player) */
      for(int i = 0; i < numZombies; i++)
      {
        if(player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
        {
          if(player.hit(gameTimeTotal))
          {
            //
            hit.play();
          }
          if(player.getHealth() <= 0)
          {
            state = State::GAME_OVER;
            std::ofstream outputFile("gamedata/scores.txt");
            outputFile << hiScore;
            outputFile.close();
            upAtBegin = 0;
          }
        }
      }
      /* Player Pickups Detection (Player pickup either ammo or health) */
      if(player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
      {
        player.increaseHealthLevel(healthPickup.gotIt());
        pickup.play();
      }
      if(player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
      {
        bulletsSpare += ammoPickup.gotIt();
        reload.play();
      }

      /* Health Bar Display */
      healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

      /* Frames passed to update the HUD */
      framesSinceLastHUDUpdate++;
      if(framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
      {
        std::stringstream ssAmmo;
        std::stringstream ssScore;
        std::stringstream ssHiScore;
        std::stringstream ssWave;
        std::stringstream ssZombiesAlive;

        ssAmmo << bulletsInClip << "/" << bulletsSpare;
        ammoText.setString(ssAmmo.str());
        ssScore << "Score: " << score;
        scoreText.setString(ssScore.str());
        ssHiScore << "Hi Score: " << hiScore;
        hiScoreText.setString(ssHiScore.str());
        ssWave << "Wave : " << wave;
        waveNumberText.setString(ssWave.str());
        ssZombiesAlive << "Zombies: " << numZombiesAlive;
        zombiesRemainingText.setString(ssZombiesAlive.str());
        framesSinceLastHUDUpdate = 0;
      }
    }

    /* Drawn the Scene */
    if(state == State::PLAYING)
    {
      /* Clear previous window */
      window.clear();
      /* set mainview as window */
      window.setView(mainView);
      /*draw background */
      window.draw(background, &textureBackground);
      /*draw zombies*/
      for(int i = 0; i < numZombies; i++)
      {
        window.draw(zombies[i].getSprite());
      }
      /*draw bullets*/
      for(int i = 0; i < 100; i++)
      {
        if(bullets[i].isInFlight())
        {
          window.draw(bullets[i].getShape());
        }
      }
      /*draw pickups*/
      if(ammoPickup.isSpawned())
      {
        window.draw(ammoPickup.getSprite());
      }
      if(healthPickup.isSpawned())
      {
        window.draw(healthPickup.getSprite());
      }
      window.draw(spriteCrosshair);
      window.draw(player.getSprite());
      window.setView(hudView);
      window.draw(spriteAmmoIcon);
      window.draw(ammoText);
      window.draw(scoreText);
      window.draw(hiScoreText);
      window.draw(healthBar);
      window.draw(waveNumberText);
      window.draw(zombiesRemainingText);
    }

    if(state == State::LEVELING_UP)
    {
      window.draw(spriteGameOver);
      window.draw(levelUpText);
    }
    if(state == State::PAUSED)
    {
      window.draw(pausedText);
    }
    if(state == State::GAME_OVER)
    {
      window.draw(spriteGameOver);
      window.draw(gameOverText);
      window.draw(scoreText);
      window.draw(hiScoreText);
    }
    window.display();
  }
  delete[] zombies;
  return 0;
}
