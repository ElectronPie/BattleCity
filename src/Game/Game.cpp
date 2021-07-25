#include "Game.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
}

Game::~Game()
{
    ;
}

void Game::render()
{
    ResourceManager::getSprite("NewSprite")->render();
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
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

    return true;
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}