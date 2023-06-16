#include "Shader.h"
#include "Renderer.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>


Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource Source = ParseShader(filepath);
    m_RendererID = CreateShader(Source.VertexShader, Source.FragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const 
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int i) {
    GLCall(glUniform1i(GetUniformLocation(name), i));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCashe.find(name) != m_UniformLocationCashe.end())
        return m_UniformLocationCashe[name];

    GLCall(int location=glGetUniformLocation(m_RendererID,name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    m_UniformLocationCashe[name] = location;

    return location;
}

//加载shader代码
ShaderProgramSource Shader::ParseShader(const std::string& FilePath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream Stream(FilePath);
    std::string Line;
    std::stringstream ss[2];
    ShaderType Type = ShaderType::NONE;

    while (getline(Stream, Line))
    {
        if (Line.find("#shader") != std::string::npos) {
            if (Line.find("vertex") != std::string::npos) {
                Type = ShaderType::VERTEX;
            }
            else if (Line.find("fragment") != std::string::npos) {
                Type = ShaderType::FRAGMENT;
            }
        }
        else if ((int)Type >= 0) {
            ss[(int)Type] << Line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}
//传入shader的glsl代码，根据类型编译为不同的着色器
unsigned int Shader::CompileShader(const std::string& Source, unsigned int Type) {
    unsigned int Id = glCreateShader(Type);
    const char* Src = Source.c_str();
    glShaderSource(Id, 1, &Src, nullptr);
    glCompileShader(Id);

    int Result;
    glGetShaderiv(Id, GL_COMPILE_STATUS, &Result);

    if (Result == GL_FALSE) {//为可能出现的着色器编译错误做调试
        int Length;
        glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &Length);
        char* Message = (char*)malloc(Length * sizeof(char));
        glGetShaderInfoLog(Id, Length, &Length, Message);

        std::cout << "Failed to compile" << (Type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << Message << std::endl;

        glDeleteShader(Id);
        return 0;
    }

    return Id;
}
//传入shader的glsl代码，编译shader返回索引
unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader) {
    unsigned int Program = glCreateProgram();
    unsigned int Vs = CompileShader(VertexShader, GL_VERTEX_SHADER);
    unsigned int Fs = CompileShader(FragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(Program, Vs);
    glAttachShader(Program, Fs);
    glLinkProgram(Program);
    glValidateProgram(Program);

    glDeleteShader(Vs);
    glDeleteShader(Fs);

    return Program;
}

