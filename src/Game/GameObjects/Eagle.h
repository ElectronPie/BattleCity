#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>

#include <glm/vec2.hpp>

namespace RenderEngine
{
    class Sprite;
}

class Eagle : public IGameObject
{
public:
    Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation);
    virtual void render() const override;

private:
    std::array<std::shared_ptr<RenderEngine::Sprite>, 2> m_sprites;
    bool m_isAlive;
};