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

#include "Tank.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
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
    m_tank->render();
}

void Game::update(const uint64_t delta)
{
    if(m_keys[GLFW_KEY_W])
    {
        m_tank->setOrientation(Tank::EOrientation::Top);
        m_tank->move(true);
    }
    else if(m_keys[GLFW_KEY_A])
    {
        m_tank->setOrientation(Tank::EOrientation::Left);
        m_tank->move(true);
    }
    else if(m_keys[GLFW_KEY_S])
    {
        m_tank->setOrientation(Tank::EOrientation::Bottom);
        m_tank->move(true);
    }
    else if(m_keys[GLFW_KEY_D])
    {
        m_tank->setOrientation(Tank::EOrientation::Right);
        m_tank->move(true);
    }
    else
    {
        m_tank->move(false);
    }

    m_tank->update(delta);
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShaders");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShaders" << std::endl;
        return false;
    }

    auto pMapTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (!pMapTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "mapTextureAtlas" << std::endl;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "tanksTextureAtlas" << std::endl;
    }

    glm::mat4 projectionMatrix = glm::ortho<float>(0.f, m_windowSize.x, 0.f, m_windowSize.y, -100.f, 100.f);

    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pTankAnimatedSprite = ResourceManager::getAnimatedSprite("tankAnimatedSprite");
    if (!pTankAnimatedSprite)
    {
        std::cerr << "Can't find animated sprite: " << "tankAnimatedSprite" << std::endl;
    }

    pTankAnimatedSprite->setState("tankTopState");

    m_tank = std::make_unique<Tank>(pTankAnimatedSprite, 1e-7, glm::vec2(100.f, 100.f));
    m_tank->setOrientation(Tank::EOrientation::Top);

    return true;
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}