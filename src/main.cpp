#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <chrono>

#include "Resources/ResourceManager.h"
#include "Game/Game.h"
#include "Renderer/Renderer.h"

glm::ivec2 g_windowSize(13 * 16, 14 * 16);
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);

void glfwWindowSizeCallback(GLFWwindow *pwindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;

    const float aspect_ratio = static_cast<float>(g_game->getCurrentLevelWidth() / g_game->getCurrentLevelHeight());
    unsigned int viewPortWidth = g_windowSize.x;
    unsigned int viewPortHeight = g_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(g_windowSize.x) / g_windowSize.y > aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(viewPortHeight * aspect_ratio);
        viewPortLeftOffset = (g_windowSize.x - viewPortWidth) / 2;
    }
    else if (static_cast<float>(g_windowSize.x) / g_windowSize.y < aspect_ratio)
    {
        viewPortHeight = static_cast<unsigned int>(viewPortWidth / aspect_ratio);
        viewPortBottomOffset = (g_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    else g_game->setKey(key, action);
}

int main(int argc, char **argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit() failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "BattleCity", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow() failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if(!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << std::endl;
    std::cout << "OpenGL version: " << RenderEngine::Renderer::getVersionStr() << std::endl;

    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);

    {
        ResourceManager::setExecutablePath(argv[0]);
        if(!g_game->init())
        {
            std::cerr << "Couldn't initialize game resources!" << std::endl;
            return -1;
        }

        glfwSetWindowSize(pWindow, g_game->getCurrentLevelWidth(), g_game->getCurrentLevelHeight());

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the pWindow */
        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game->update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_game->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        g_game = nullptr;
        ResourceManager::unloadAllResources();
    }

    //glfwTerminate();
    return 0;
}   