#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>

#include <glm/vec2.hpp>
#include "../../Renderer/SpriteAnimator.h"

namespace RenderEngine
{
    class Sprite;
}

class Water : public IGameObject
{
public:
    Water(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;
    virtual void update(const double delta) override;

private:
    std::shared_ptr<RenderEngine::Sprite> m_sprite;
    RenderEngine::SpriteAnimator m_spriteAnimator;
    std::array<glm::vec2, 4> m_blockOffsets;
};