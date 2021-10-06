#pragma once

#include "IGameObject.h"

#include <memory>
#include <array>

#include <glm/vec2.hpp>

namespace RenderEngine
{
    class Sprite;
}

class Trees : public IGameObject
{
public:
    Trees(const glm::vec2& position, const glm::vec2& size, const float rotation);
    virtual void render() const override;

private:
    std::shared_ptr<RenderEngine::Sprite> m_sprite;
    std::array<glm::vec2, 4> m_blockOffsets;
};