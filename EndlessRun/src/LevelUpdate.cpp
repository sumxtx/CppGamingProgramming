#include "LevelUpdate.hpp"
#include "SoundEngine.hpp"
#include "PlayerUpdate.hpp"
#include <random>

using namespace std;

void LevelUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate,
    shared_ptr<PlayerUpdate > playerUpdate)
{
  m_PlayerPosition = playerUpdate->getPositionPointer();
  SoundEngine::startMusic();
}

void LevelUpdate::connectToCameraTime(float* cameraTime)
{
  m_CameraTime = cameraTime;
}

void LevelUpdate::addPlatformPosition(FloatRect* newPosition)
{
  m_PlatformPositions.push_back(newPosition);
  m_NumberOfPlatforms++;
}

bool* LevelUpdate::getIsPausedPointer()
{
  return &m_IsPaused;
}

void LevelUpdate::positionLevelAtStart()
{
  float startOffset = m_PlatformPositions[0]->left;
  for(int i = 0; i < m_NumberOfPlatforms; i++)
  {
    m_PlatformPositions[i]->left = i * 100 + startOffset;
    m_PlatformPositions[i]->top = 0;
    m_PlatformPositions[i]->width = 100;
    m_PlatformPositions[i]->height = 20;
  }
  m_PlayerPosition->left = m_PlatformPositions[m_NumberOfPlatforms /2]->left + 2;
  m_PlayerPosition->top = m_PlatformPositions[m_NumberOfPlatforms /2]->top - 22;

  m_MoveRelativeToPlatform = m_NumberOfPlatforms - 1;
  m_NextPlatformToMove = 0;
}

int LevelUpdate::getRandomNumber(int minHeight, int maxHeight)
{
  random_device rd;
  mt19937 gen(rd());

  uniform_int_distribution<int> distribution(minHeight, maxHeight);
  int randomHeight = distribution(gen);
  return randomHeight;
}

void LevelUpdate::update(float timeSinceLastUpdate)
{
  if(!m_IsPaused)
  {
    if(m_GameOver)
    {
      m_GameOver = false;
      *m_CameraTime = 0;
      m_TimeSinceLastPlatform = 0;
      int platformToPlacePlayerOn;
      positionLevelAtStart();
    }

    *m_CameraTime += timeSinceLastUpdate;
    m_TimeSinceLastPlatform += timeSinceLastUpdate;

    if(m_TimeSinceLastPlatform > m_PlatformCreationInterval)
    {
      m_PlatformPositions[m_NextPlatformToMove]->top = m_PlatformPositions[m_MoveRelativeToPlatform]->top + 
        getRandomNumber(-40, 40);

      if(m_PlatformPositions[m_MoveRelativeToPlatform]->top < m_PlatformPositions[m_NextPlatformToMove]->top)
      {
        m_PlatformPositions[m_NextPlatformToMove]->left = m_PlatformPositions[m_MoveRelativeToPlatform]->left + 
          m_PlatformPositions[m_MoveRelativeToPlatform]->width + getRandomNumber(20,40);
      }
      else
      {
        m_PlatformPositions[m_NextPlatformToMove]->left = m_PlatformPositions[m_MoveRelativeToPlatform]->left +
          m_PlatformPositions[m_MoveRelativeToPlatform]->width + getRandomNumber(0, 20);
      }
      m_PlatformPositions[m_NextPlatformToMove]->width = getRandomNumber(20,200);
      m_PlatformPositions[m_NextPlatformToMove]->height = getRandomNumber(10,20);

      m_PlatformCreationInterval = m_PlatformPositions[m_NextPlatformToMove]->width / 90;
      m_MoveRelativeToPlatform = m_NextPlatformToMove;
      m_NextPlatformToMove++;

      if(m_NextPlatformToMove == m_NumberOfPlatforms)
      {
        m_NextPlatformToMove = 0;
      }

      m_TimeSinceLastPlatform = 0;
    }

    bool laggingBehind = true;
    for(auto platformPosition : m_PlatformPositions)
    {
      if(platformPosition->left < m_PlayerPosition->left)
      {
        laggingBehind = false;
        break;
      }
      else
      {
        laggingBehind = true;
      }
    }

    if(laggingBehind)
    {
      m_IsPaused = true;
      m_GameOver = true;

      SoundEngine::pauseMusic;
    }
  }
}
