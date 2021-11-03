#include "Game.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Level.h"
#include "GameObjects/Tank.h"

#include "../Physics/PhysicsEngine.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Resources/ResourceManager.h"

Game::Game(const glm::vec2 &windowSize)
: m_eCurrentGameState(EGameState::Active)
, m_windowSize(windowSize)
{
    m_keys.fill(false);

    //std::cout << "Game constructor" << std::endl;
}

Game::~Game()
{
    //std::cout << "Game destructor" << std::endl;
}

void Game::render()
{
    if(m_pTank)
    {
        m_pTank->render();
    }

    if(m_pLevel)
    {
        m_pLevel->render();
    }
}

void Game::update(const double delta)
{
    m_pLevel->update(delta);

    if(m_keys[GLFW_KEY_W])
    {
        m_pTank->setOrientation(Tank::EOrientation::Top);
        m_pTank->setVelocity(m_pTank->getMaxVelocity());
    }
    else if(m_keys[GLFW_KEY_A])
    {
        m_pTank->setOrientation(Tank::EOrientation::Left);
        m_pTank->setVelocity(m_pTank->getMaxVelocity());
    }
    else if(m_keys[GLFW_KEY_S])
    {
        m_pTank->setOrientation(Tank::EOrientation::Bottom);
        m_pTank->setVelocity(m_pTank->getMaxVelocity());
    }
    else if(m_keys[GLFW_KEY_D])
    {
        m_pTank->setOrientation(Tank::EOrientation::Right);
        m_pTank->setVelocity(m_pTank->getMaxVelocity());
    }
    else
    {
        m_pTank->setVelocity(0.);
    }

    if (m_keys[GLFW_KEY_SPACE])
    {
        m_pTank->fire();
    }

    m_pTank->update(delta);
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "tanksTextureAtlas" << std::endl;
    }

    m_pLevel = std::make_shared<Level>(ResourceManager::getLevels()[0]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());
    Physics::PhysicsEngine::setCurrentLevel(m_pLevel);

    glm::mat4 projectionMatrix = glm::ortho<float>(0.f, m_windowSize.x, 0.f, m_windowSize.y, -100.f, 100.f);

    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    m_pTank = std::make_shared<Tank>(0.064, m_pLevel->getPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);
    Physics::PhysicsEngine::addDynamicObject(m_pTank);
    m_pTank->setOrientation(Tank::EOrientation::Top);

    return true;
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

size_t Game::getCurrentLevelWidth() const
{
    return m_pLevel->getLevelWidth();
}

size_t Game::getCurrentLevelHeight() const
{
    return m_pLevel->getLevelHeight();
}