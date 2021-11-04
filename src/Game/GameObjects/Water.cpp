#include "Water.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Water::Water(const glm::vec2& position,
             const glm::vec2& size,
             const float rotation,
             const float layer)
: IGameObject(EObjectType::Water, position, size, rotation, layer)
, m_sprite(ResourceManager::getSprite("water"))
, m_spriteAnimator(m_sprite)
, m_blockOffsets{
    glm::vec2(0            , m_size.y/2.f),
    glm::vec2(m_size.x/2.f , m_size.y/2.f),
    glm::vec2(0            , 0           ),
    glm::vec2(m_size.x/2.f , 0           ) }
{
    m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Water::render() const
{
    for (int i = 0; i < 4; i++)
    {
        m_sprite->render(m_position + m_blockOffsets[i], m_size/2.f, m_rotation, m_layer, m_spriteAnimator.getCurrentFrame());
    }
}

void Water::update(const double delta)
{
    m_spriteAnimator.update(delta);
}

bool Water::collides(const EObjectType objectType)
{
    if (objectType == EObjectType::Bullet)
    {
        return false;
    }
    return true;
}