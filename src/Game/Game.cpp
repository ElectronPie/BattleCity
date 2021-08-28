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
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShaders", "res/shaders/vertex.vert", "res/shaders/fragment.frag");
    if(!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShaders" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShaders", "res/shaders/vSprite.vert", "res/shaders/fSprite.frag");
    if(!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "SpriteShaders" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

    std::vector<std::string> subTextureNames = {
        "block",
        "topBlock",
        "bottomBlock",
        "leftBlock",
        "rightBlock",
        "topLeftBlock",
        "topRightBlock",
        "bottomLeftBlock",

        "bottomRightBlock",
        "concrete",
        "topConcrete",
        "bottomConcrete",
        "leftConcrete",
        "rightConcrete",
        "topLeftConcrete",
        "topRightConcrete",

        "bottomLeftConcrete",
        "bottomRightConcrete",
        "water1",
        "water2",
        "water3",
        "trees",
        "ice",
        "wall",

        "eagle",
        "deadEagle",
        "nothing",
        "respawn1",
        "respawn2",
        "respawn3",
        "respawn4"
    };
    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

    auto pSprite = ResourceManager::loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShaders", 100, 100, "concrete");
    pSprite->setPosition(glm::vec2(300, 100));

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShaders", 100, 100, "wall");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1e9));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1e9));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1e9));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1e9));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1e9));

    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));

    pAnimatedSprite->setState("waterState");

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho<float>(0.f, m_windowSize.x, 0.f, m_windowSize.y, -100.f, 100.f);

    pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    std::vector<std::string> tanksSubTextureNames = {
        "tankTop1",
        "tankTop2",
        "tankLeft1",
        "tankLeft2",
        "tankBottom1",
        "tankBottom2",
        "tankRight1",
        "tankRight2"
    };

    auto pTanksTextureAtlas = ResourceManager::loadTextureAtlas("TanksTextureAtlas", "res/textures/tanks.png", std::move(tanksSubTextureNames), 16, 16);
    auto pTankAnimatedSprite = ResourceManager::loadAnimatedSprite("TankAnimatedSprite", "TanksTextureAtlas", "SpriteShaders", 100, 100, "tankTop1");

    std::vector<std::pair<std::string, uint64_t>> tankTopState;
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop1", 5e8));
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop2", 5e8));

    std::vector<std::pair<std::string, uint64_t>> tankBottomState;
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom1", 5e8));
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom2", 5e8));

    std::vector<std::pair<std::string, uint64_t>> tankLeftState;
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft1", 5e8));
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft2", 5e8));

    std::vector<std::pair<std::string, uint64_t>> tankRightState;
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight1", 5e8));
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight2", 5e8));

    pTankAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
    pTankAnimatedSprite->insertState("tankBottomState", std::move(tankBottomState));
    pTankAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
    pTankAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

    m_tank = std::make_unique<Tank>(pTankAnimatedSprite, 1e-7, glm::vec2(100.f, 100.f));

    return true;
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}