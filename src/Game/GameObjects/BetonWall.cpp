#include "BetonWall.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

BetonWall::BetonWall(const EBetonWallType eBetonWallType,
                     const glm::vec2& position,
                     const glm::vec2& size,
                     const float rotation,
                     const float layer)
: IGameObject(position, size, rotation, layer)
, m_currentBlockState{ false,
                       false,
                       false,
                       false }
, m_blockOffsets{
    glm::vec2(0            , m_size.y/2.f),
    glm::vec2(m_size.x/2.f , m_size.y/2.f),
    glm::vec2(0            , 0           ),
    glm::vec2(m_size.x/2.f , 0           ) }
{
    m_sprite = ResourceManager::getSprite("betonWall");

    switch(eBetonWallType)
    {
    case EBetonWallType::All:
        m_currentBlockState.fill(true);
        break;
    case EBetonWallType::Top:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]     = true;
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopRight)]    = true;
        break;
    case EBetonWallType::Bottom:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]  = true;
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = true;
        break;
    case EBetonWallType::Left:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]     = true;
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]  = true;
        break;
    case EBetonWallType::Right:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopRight)]    = true;
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = true;
        break;
    case EBetonWallType::TopLeft:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]     = true;
        break;
    case EBetonWallType::TopRight:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::TopRight)]    = true;
        break;
    case EBetonWallType::BottomLeft:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]  = true;
        break;
    case EBetonWallType::BottomRight:
        m_currentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = true;
        break;
    }
}

void BetonWall::render() const
{
    for (int i = 0; i < 4; i++)
    {
        if (m_currentBlockState[i])
        {
            m_sprite->render(m_position + m_blockOffsets[i], m_size/2.f, m_rotation, m_layer);
        }
    }
}

void BetonWall::update(const double delta)
{}