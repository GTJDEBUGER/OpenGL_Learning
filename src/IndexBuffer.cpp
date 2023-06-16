#include "IndexBuffer.h"
#include "Renderer.h"

/// <summary>
/// 创建一个IndexBuffer
/// </summary>
/// <param name="data">索引数组</param>
/// <param name="size">索引数组的元素数量</param>
IndexBuffer::IndexBuffer(const unsigned  int* data, unsigned int count) : m_Count(count) {
    ASSERT(sizeof(unsigned int)==sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void IndexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}