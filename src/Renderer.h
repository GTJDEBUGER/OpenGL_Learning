#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
        x;\
        ASSERT(GLLogCall(#x,__FILE__,__LINE__));

//���Ŀǰ���ֵ����д���
void GLClearError(); 
//�ڿ���̨��ӡ��Ŀǰ�Ĵ���
bool GLLogCall(const char* Function, const char* File, int Line); 

class Renderer {
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib,  const Shader& shader) const;
    void Clear() const;
private:

};