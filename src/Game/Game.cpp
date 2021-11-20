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

#include "GameStates/StartScreen.h"
#include "GameStates/Level.h"
#include "GameObjects/Tank.h"

#include "../Physics/PhysicsEngine.h"
#include "../Renderer/Renderer.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Resources/ResourceManager.h"

Game::Game(const glm::uvec2 &windowSize)
: m_eCurrentGameState(EGameState::StartScreen)
, m_windowSize(windowSize)
{
    m_keys.fill(false);
    //std::cout << "Game constructor" << std::endl;
}

void Game::setWindowSize(const glm::uvec2& windowSize)
{
    m_windowSize = windowSize;
    updateViewport();
}

void Game::updateViewport()
{
    const float aspect_ratio = static_cast<float>(getCurrentWidth()) / getCurrentHeight();
    unsigned int viewPortWidth = m_windowSize.x;
    unsigned int viewPortHeight = m_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(m_windowSize.x) / m_windowSize.y > aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(viewPortHeight * aspect_ratio);
        viewPortLeftOffset = (m_windowSize.x - viewPortWidth) / 2;
    }
    else if (static_cast<float>(m_windowSize.x) / m_windowSize.y < aspect_ratio)
    {
        viewPortHeight = static_cast<unsigned int>(viewPortWidth / aspect_ratio);
        viewPortBottomOffset = (m_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);

    glm::mat4 projectionMatrix = glm::ortho<float>(0.f, getCurrentWidth(), 0.f, getCurrentHeight(), -100.f, 100.f);
    m_pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
}

Game::~Game()
{
    //std::cout << "Game destructor" << std::endl;
}

void Game::startNewLevel(const size_t level)
{
    auto pLevel = std::make_shared<Level>(ResourceManager::getLevels()[0]);
    m_pCurrentGameState = pLevel;
    Physics::PhysicsEngine::setCurrentLevel(pLevel);
    updateViewport();
}

void Game::render()
{
    m_pCurrentGameState->render();
}

void Game::update(const double delta)
{
    m_pCurrentGameState->processInput(m_keys);
    m_pCurrentGameState->update(delta);

    /*
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        if (m_keys[GLFW_KEY_ENTER])
        {
            m_eCurrentGameState = EGameState::Level;
            startNewLevel(0);
        }
        break;
    case EGameState::Level:
        break;
    }
    */
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    m_pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!m_pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    m_pSpriteShaderProgram->use();
    m_pSpriteShaderProgram->setInt("tex", 0);

    m_pCurrentGameState = std::make_shared<StartScreen>(ResourceManager::getStartScreen(), this);
    setWindowSize(m_windowSize);
    return true;
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

size_t Game::getCurrentWidth() const
{
    return m_pCurrentGameState->getStateWidth();
}

size_t Game::getCurrentHeight() const
{
    return m_pCurrentGameState->getStateHeight();
}