#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //PNG从左上角开始为像素计数而OpenGL是从左下角开始的，所以要先做以此反转
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height , &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR));//设置当texture比网格小的应对情况
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//设置当texture比网格大的应对情况
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width,m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));//将图片送往GPU
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);//从CPU上把图片卸载
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1,&m_RendererID));
}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
