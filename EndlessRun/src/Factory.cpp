#include "Factory.hpp"
#include "LevelUpdate.hpp"
#include "PlayerGraphics.hpp"
#include "PlayerUpdate.hpp"
#include "InputDispatcher.hpp"
#include "CameraUpdate.hpp"
#include "CameraGraphics.hpp"
#include <iostream>

using namespace std;

Factory::Factory(RenderWindow *window)
{
  m_Window = window;
  m_Texture = new Texture();
  if(!m_Texture->loadFromFile("graphics/texture.png"))
  {
    cout << "Texture not loaded";
    return;
  }
}

void Factory::loadLevel(vector<GameObject>& gameObjects, VertexArray& canvas, InputDispatcher& inputDispatcher)
{
  GameObject level;
  shared_ptr<LevelUpdate> levelUpdate = make_shared<LevelUpdate>();
  level.addComponent(levelUpdate);

  gameObjects.push_back(level);

  GameObject  player;
  shared_ptr<PlayerUpdate> playerUpdate = make_shared<PlayerUpdate>();
  playerUpdate->assemble(levelUpdate, nullptr);

  player.addComponent(playerUpdate);

  inputDispatcher.registerNewInputReceiver(playerUpdate->getInputReceiver());

  shared_ptr<PlayerGraphics> playerGraphics = make_shared<PlayerGraphics>();

  playerGraphics->assemble(canvas, playerUpdate, IntRect(PLAYER_TEX_LEFT, PLAYER_TEX_TOP, PLAYER_TEX_WIDTH, PLAYER_TEX_HEIGHT));
  player.addComponent(playerGraphics);

  gameObjects.push_back(player);
  
  levelUpdate->assemble(nullptr, playerUpdate);

  //Cameras
  const float width = float(VideoMode::getDesktopMode().width);
  const float height = float(VideoMode::getDesktopMode().height);
  const float ratio = width/height;

  GameObject camera;
  shared_ptr<CameraUpdate> cameraUpdate = make_shared<CameraUpdate>();
  cameraUpdate->assemble(nullptr, playerUpdate);
  camera.addComponent(cameraUpdate);

  shared_ptr<CameraGraphics> cameraGraphics = 
    make_shared<CameraGraphics>(
      m_Window,
      m_Texture,
      Vector2f(CAM_VIEW_WIDTH, CAM_VIEW_WIDTH/ratio),
      FloatRect(CAM_SCREEN_RATIO_LEFT, CAM_SCREEN_RATIO_TOP, CAM_SCREEN_RATIO_WIDTH, CAM_SCREEN_RATIO_HEIGHT)
    );
  
  cameraGraphics->assemble(canvas, cameraUpdate, IntRect(CAM_TEX_LEFT, CAM_TEX_TOP, CAM_TEX_WIDTH, CAM_TEX_HEIGHT));
  camera.addComponent(cameraGraphics);
  gameObjects.push_back(camera);
  levelUpdate->connectToCameraTime(cameraGraphics->getTimeConnection());

  //MapCamera
  GameObject mapCamera;
  shared_ptr<CameraUpdate> mapCameraUpdate = make_shared<CameraUpdate>();
  mapCameraUpdate->assemble(nullptr, playerUpdate);
  mapCamera.addComponent(mapCameraUpdate);
  
  inputDispatcher.registerNewInputReceiver(mapCameraUpdate->getInputReceiver());

  shared_ptr<CameraGraphics> mapCameraGraphics = make_shared<CameraGraphics>(
      m_Window,
      m_Texture,
      Vector2f(MAP_CAM_VIEW_WIDTH, MAP_CAM_VIEW_HEIGHT / ratio),
      FloatRect(
        MAP_CAM_SCREEN_RATIO_LEFT, 
        MAP_CAM_SCREEN_RATIO_TOP,
        MAP_CAM_SCREEN_RATIO_WIDTH,
        MAP_CAM_SCREEN_RATIO_HEIGHT));

  mapCameraGraphics->assemble(
      canvas,
      mapCameraUpdate,
      IntRect(
        MAP_CAM_TEX_LEFT,
        MAP_CAM_TEX_TOP,
        MAP_CAM_TEX_WIDTH,
        MAP_CAM_TEX_HEIGHT
      )
  );

  mapCamera.addComponent(mapCameraGraphics);
  gameObjects.push_back(mapCamera);
}
