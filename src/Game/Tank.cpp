#include "Tank.h"

#include <iostream>
#include "../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite,
     const float velocity,
     const glm::vec2 &position)
: m_eOrientation(EOrientation::Top)
, m_pSprite(std::move(pSprite))
, m_move(false)
, m_velocity(velocity)
, m_position(std::move(position))
{
     m_pSprite->setPosition(std::move(position));
     m_moveOffset.x = 0.f;
     m_moveOffset.y = 1.f;

     //std::cout << "Tank constructor" << std::endl;
}

void Tank::render() const
{
     m_pSprite->render();
}

void Tank::setOrientation(const EOrientation eOrientation)
{
     if (m_eOrientation == eOrientation) return;
     m_eOrientation = eOrientation;

     switch (m_eOrientation)
     {
     case EOrientation::Top:
          m_pSprite->setState("tankTopState");
          m_moveOffset.x = 0.f;
          m_moveOffset.y = 1.f;
          break;
     case EOrientation::Bottom:
          m_pSprite->setState("tankBottomState");
          m_moveOffset.x = 0.f;
          m_moveOffset.y = -1.f;
          break;
     case EOrientation::Left:
          m_pSprite->setState("tankLeftState");
          m_moveOffset.x = -1.f;
          m_moveOffset.y = 0.f;
          break;
     case EOrientation::Right:
          m_pSprite->setState("tankRightState");
          m_moveOffset.x = 1.f;
          m_moveOffset.y = 0.f;
          break;
     default:
          std::cerr << "help" << std::endl;
          break;
     }
}

void Tank::move(const bool move)
{
     m_move = move;
}

void Tank::update(const uint64_t delta)
{
     if (m_move)
     {
          m_position += delta * m_velocity * m_moveOffset;
          m_pSprite->setPosition(m_position);
          m_pSprite->update(delta);
     }
}