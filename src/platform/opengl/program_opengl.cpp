#include "program_opengl.hpp"

#include "core/assert.hpp"
#include "core/logger.hpp"

#include "glad/gl.h"

Vin::OpenGLProgram::OpenGLProgram()
{
	m_ProgramId = glCreateProgram();
	for (auto shaderId : m_Shaders)
		glDeleteShader(shaderId);
	m_Shaders.clear();
}

Vin::OpenGLProgram::~OpenGLProgram()
{
	glDeleteProgram(m_ProgramId);
}

void Vin::OpenGLProgram::Bind()
{
	glUseProgram(m_ProgramId);
}

void Vin::OpenGLProgram::Unbind()
{
	glUseProgram(0);
}

bool Vin::OpenGLProgram::AddShader(ShaderType type, const char* src)
{
	unsigned int shaderId;
	shaderId = glCreateShader(ParseShaderType(type));
	glShaderSource(shaderId, 1, &src, NULL);
	glCompileShader(shaderId);
	if (!CheckForShaderCompilationErr(shaderId))
		return false;
	m_Shaders.push_back(shaderId);
	return true;
}

bool Vin::OpenGLProgram::CompileProgram()
{
	for (auto shaderId : m_Shaders)
		glAttachShader(m_ProgramId, shaderId);

	glLinkProgram(m_ProgramId);
	if (!CheckForProgramCompilationErr(m_ProgramId))
		return false;

	for (auto shaderId : m_Shaders)
		glDeleteShader(shaderId);

	m_IsComplete = true;
	m_Shaders.clear();
	return true;
}

bool Vin::OpenGLProgram::IsShaderComplete()
{
	return m_IsComplete;
}

int Vin::OpenGLProgram::ParseShaderType(ShaderType type)
{
	switch (type) {
	case ShaderType::ComputeShader: return GL_COMPUTE_SHADER;
	case ShaderType::VertexShader: return GL_VERTEX_SHADER;
	case ShaderType::TessControl: return GL_TESS_CONTROL_SHADER;
	case ShaderType::TessEvaluation: return GL_TESS_EVALUATION_SHADER;
	case ShaderType::GeometryShader: return GL_GEOMETRY_SHADER;
	case ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
	}
	return -1;
}

bool Vin::OpenGLProgram::CheckForShaderCompilationErr(unsigned int shaderId)
{
	int result;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		Logger::Err("Shader compilation error : {}", infoLog);
		return false;
	}
}

bool Vin::OpenGLProgram::CheckForProgramCompilationErr(unsigned int programId)
{
	int result{ 0 };
	char infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		Logger::Err("Program compilation error : {}", infoLog);
		return false;
	}
}
