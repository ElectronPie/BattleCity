#include "Trees.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Trees::Trees(const glm::vec2& position,
             const glm::vec2& size,
             const float rotation,
             const float layer)
: IGameObject(position, size, rotation, layer)
, m_sprite(ResourceManager::getSprite("trees"))
, m_blockOffsets{
    glm::vec2(0            , m_size.y/2.f),
    glm::vec2(m_size.x/2.f , m_size.y/2.f),
    glm::vec2(0            , 0           ),
    glm::vec2(m_size.x/2.f , 0           ) }
{
}

void Trees::render() const
{
    for (int i = 0; i < 4; i++)
    {
        m_sprite->render(m_position + m_blockOffsets[i], m_size/2.f, m_rotation, m_layer);
    }
}