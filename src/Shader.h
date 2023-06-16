#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"


//用于存储多个着色器的结构
struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniform
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1i(const std::string& name, int i);

private:
	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	unsigned int CompileShader(const std::string& Source, unsigned int Type);
	ShaderProgramSource ParseShader(const std::string& FilePath);

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCashe;
};