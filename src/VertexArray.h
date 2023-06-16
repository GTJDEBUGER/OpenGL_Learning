#pragma once

#include "VertexBuffer.h"

class  VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	//设置顶点buffer的内存分布
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;

};