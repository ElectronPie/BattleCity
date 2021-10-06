#include "Eagle.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Eagle::Eagle(const glm::vec2& position,
             const glm::vec2& size,
             const float rotation)
: IGameObject(position, size, rotation)
, m_isAlive(true)
, m_sprites{ ResourceManager::getSprite("eagle_dead"),
             ResourceManager::getSprite("eagle") }
{
}

void Eagle::render() const
{
    m_sprites[static_cast<size_t>(m_isAlive)]->render(m_position, m_size, m_rotation);
}