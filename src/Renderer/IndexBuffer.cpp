#include "IndexBuffer.h"

//#include <iostream>

namespace RenderEngine
{
    IndexBuffer::IndexBuffer()
    : m_id(0)
    , m_count(0)
    {}

    IndexBuffer::~IndexBuffer()
    {
        //std::cout << "Deleting IBO " << m_id <<std::endl;

        glDeleteBuffers(1, &m_id);
    }

    void IndexBuffer::init(const void *data, const unsigned int count)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

        m_count = count;

        //std::cout << "Created IBO " << m_id << std::endl;
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
    {
        m_id = indexBuffer.m_id;
        m_count = indexBuffer.m_count;
        indexBuffer.m_id = 0;
        indexBuffer.m_count = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
    {
        m_id = indexBuffer.m_id;
        m_count = indexBuffer.m_count;
        indexBuffer.m_id = 0;
        indexBuffer.m_count = 0;
        return *this;
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}