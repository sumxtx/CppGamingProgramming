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

void PlayerUpdate::update(float timeTakenThisFrame)
{
  if(!*m_IsPaused)
  {
    m_Position.top += m_Gravity * timeTakenThisFrame;
    handleInput();

    if(m_IsGrounded)
    {
      if(m_RightIsHeldDown)
      {
        m_Position.left += timeTakenThisFrame + m_RunSpeed;
      }
      if(m_LeftIsHeldDown)
      {
        m_Position.left -= timeTakenThisFrame * m_RunSpeed;
      }
    }
    if(m_BoostIsHeldDown)
    {
      m_Position.top -= timeTakenThisFrame * m_BoostSpeed;
      if(m_RightIsHeldDown)
      {
        m_Position.left += timeTakenThisFrame * m_RunSpeed /4;
      }
      if(m_LeftIsHeldDown)
      {
        m_Position.left -= timeTakenThisFrame * m_RunSpeed / 4;
      }
    }
    if(m_SpaceHeldDown && !m_InJump && m_IsGrounded)
    {
      SoundEngine::playJump();
      m_InJump = true;
      m_JumpClock.restart();
    }
    if(!m_SpaceHeldDown)
    {

    }
    if(m_InJump)
    {
      if(m_JumpClock.getElapsedTime().asSeconds() < m_JumpDuration / 2)
      {
        m_Position.top -= m_JumpSpeed * timeTakenThisFrame;
      }
      else
      {
        m_Position.top += m_JumpSpeed * timeTakenThisFrame;
      }

      if(m_JumpClock.getElapsedTime().asSeconds() > m_JumpDuration)
      {
        m_InJump = false;
      }
      if(m_RightIsHeldDown)
      {
        m_Position.left += timeTakenThisFrame * m_RunSpeed;
      }
      if(m_LeftIsHeldDown)
      {
        m_Position.left += timeTakenThisFrame * m_RunSpeed;
      }
    }
    m_IsGrounded = false;
  }
}

void PlayerUpdate::handleInput()
{
  for(const Event& event : m_InputReceiver.getEvents())
  {
    if(event.type == Event::KeyPressed)
    {
      if(event.key.code == Keyboard::D)
      {
        m_RightIsHeldDown = true;
      }
      if(event.key.code == Keyboard::A)
      {
        m_LeftIsHeldDown = true;
      }
      if(event.key.code == Keyboard::W)
      {
        m_BoostIsHeldDown = true;
      }
      if(event.key.code == Keyboard::Space)
      {
        m_SpaceHeldDown = true;
      }
    }

    if(event.type == Event::KeyReleased)
    {
      if(event.key.code == Keyboard::D)
      {
        m_RightIsHeldDown = false;
      }
      if(event.key.code == Keyboard::A)
      {
        m_LeftIsHeldDown = false;
      }
      if(event.key.code == Keyboard::W)
      {
        m_BoostIsHeldDown = false;
      }
      if(event.key.code == Keyboard::Space)
      {
        m_SpaceHeldDown = false;
      }
    }
  }
    m_InputReceiver.clearEvents();
}
