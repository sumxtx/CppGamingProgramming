#include "PlayerUpdate.hpp"
#include "SoundEngine.hpp"
#include "LevelUpdate.hpp"
#include <memory>

FloatRect* PlayerUpdate::getPositionPointer()
{
  return &m_Position;
}

bool* PlayerUpdate::getGroundedPointer()
{
  return &m_IsGrounded;
}

InputReceiver* PlayerUpdate::getInputReceiver()
{
  return &m_InputReceiver;
}

void PlayerUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate,
                            shared_ptr<PlayerUpdate> playerUpdate)
{
    SoundEngine();

    m_Position.width = PLAYER_WIDTH;
    m_Position.height = PLAYER_HEIGHT;
    m_IsPaused = levelUpdate->getIsPausedPointer();
}

void PlayerUpdate::handleInput()
{
  m_InputReceiver.clearEvents();
}

void PlayerUpdate::update(float timeTakenThisFrame)
{
  handleInput();
}
