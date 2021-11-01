#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class Level;
class Tank;

class Game
{
public:
    Game(const glm::vec2 &windowSize);
    ~Game();

    void render();
    void update(const double delta);
    void setKey(const int key, const int action);
    bool init();
    size_t getCurrentLevelWidth() const;
    size_t getCurrentLevelHeight() const;

private:
    std::array<bool, 349> m_keys;

    enum EGameState {
        Active,
        Pause
    };

    glm::vec2 m_windowSize;
    EGameState m_eCurrentGameState;
    std::shared_ptr<Tank> m_pTank;
    std::shared_ptr<Level> m_pLevel;
};