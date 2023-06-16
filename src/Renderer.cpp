#include "Renderer.h"
#include <iostream>

void GLClearError(){
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* Function, const char* File, int Line){
    while (GLenum Error = glGetError())
    {
        std::cout << "[OpenGL_Error] (" << Error << "):" << Function << " " <<
            File << ":" << Line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    //����������
    shader.Bind();
    va.Bind();
    ib.Bind();

    //���ݶ�������buffer����������
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const{
    glClear(GL_COLOR_BUFFER_BIT);
}