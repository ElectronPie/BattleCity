#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace RenderEngine
{
    ShaderProgram::ShaderProgram(const std::string &vertex_shader, const std::string &fragment_shader)
    {
        GLuint vertexShaderID;
        if(!createShader(vertex_shader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "VERTEX SHADER compile-time error!" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if(!createShader(fragment_shader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "FRAGMENT SHADER compile-time error!" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
        }
        else
        {
            m_isCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        //std::cout << "Shader program constructor" << std::endl;
    }

    bool ShaderProgram::createShader(const std::string &source, const GLenum shaderType, GLuint &shaderID)
    {
        shaderID = glCreateShader(shaderType);
        const char *code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);

        //std::cout << "Shader program destructor" << std::endl;
    }

    void ShaderProgram::use() const
    {
        glUseProgram(m_ID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;

        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }

    void ShaderProgram::setInt(const std::string &name, GLint value)
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void ShaderProgram::setFloat(const std::string &name, GLfloat value)
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void ShaderProgram::setMatrix4(const std::string &name, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}