#include "VertexBuffer.h"
#include "Renderer.h"

/// <summary>
/// 创建一个VertextBuffer
/// </summary>
/// <param name="data">顶点数组</param>
/// <param name="size">顶点数组的大小</param>
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}