#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class IGameState;

namespace RenderEngine
{
    class ShaderProgram;
}

class Game
{
public:
    Game(const glm::uvec2 &windowSize);
    ~Game();

    void render();
    void update(const double delta);
    void setKey(const int key, const int action);
    bool init();
    size_t getCurrentWidth() const;
    size_t getCurrentHeight() const;
    void startNewLevel(const size_t level);
    void updateViewport();
    void setWindowSize(const glm::uvec2& windowSize);

private:
    enum class EGameState {
        StartScreen,
        LevelStart,
        Level,
        Pause,
        Scores,
        GameOver
    };

    std::array<bool, 349> m_keys;

    glm::uvec2 m_windowSize;
    EGameState m_eCurrentGameState;
    std::shared_ptr<IGameState> m_pCurrentGameState;
    std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
};