#include "Ice.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Ice::Ice(const glm::vec2& position,
             const glm::vec2& size,
             const float rotation,
             const float layer)
: IGameObject(EObjectType::Ice, position, size, rotation, layer)
, m_sprite(ResourceManager::getSprite("ice"))
, m_blockOffsets{
    glm::vec2(0            , m_size.y/2.f),
    glm::vec2(m_size.x/2.f , m_size.y/2.f),
    glm::vec2(0            , 0           ),
    glm::vec2(m_size.x/2.f , 0           ) }
{
}

void Ice::render() const
{
    for (int i = 0; i < 4; i++)
    {
        m_sprite->render(m_position + m_blockOffsets[i], m_size/2.f, m_rotation, m_layer);
    }
}