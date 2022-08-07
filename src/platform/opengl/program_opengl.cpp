#include "program_opengl.hpp"

#include "core/assert.hpp"
#include "core/logger/logger.hpp"

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

int Vin::OpenGLProgram::GetField(const char* name)
{
	return glGetUniformLocation(m_ProgramId, name);
}

void Vin::OpenGLProgram::SetFloat(int location, float value)
{
	glUseProgram(m_ProgramId);
	glUniform1f(location, value);
}

void Vin::OpenGLProgram::SetFloat2(int location, float* values)
{
	glUseProgram(m_ProgramId);
	glUniform2f(location, values[0], values[1]);
}

void Vin::OpenGLProgram::SetFloat3(int location, float* values)
{
	glUseProgram(m_ProgramId);
	glUniform3f(location, values[0], values[1], values[2]);
}

void Vin::OpenGLProgram::SetFloat4(int location, float* values)
{
	glUseProgram(m_ProgramId);
	glUniform4f(location, values[0], values[1], values[2], values[3]);
}

void Vin::OpenGLProgram::SetFloats(int location, float* values, size_t count)
{
	glUseProgram(m_ProgramId);
	glUniform1fv(location, count, values);
}

void Vin::OpenGLProgram::SetInt(int location, int value)
{
	glUseProgram(m_ProgramId);
	glUniform1i(location, value);
}

void Vin::OpenGLProgram::SetInt2(int location, int* values)
{
	glUseProgram(m_ProgramId);
	glUniform2i(location, values[0], values[1]);
}

void Vin::OpenGLProgram::SetInt3(int location, int* values)
{
	glUseProgram(m_ProgramId);
	glUniform3i(location, values[0], values[1], values[2]);
}

void Vin::OpenGLProgram::SetInt4(int location, int* values)
{
	glUseProgram(m_ProgramId);
	glUniform4i(location, values[0], values[1], values[2], values[3]);
}

void Vin::OpenGLProgram::SetInts(int location, int* values, size_t count)
{
	glUseProgram(m_ProgramId);
	glUniform1iv(location, count, values);
}

void Vin::OpenGLProgram::SetMat3(int location, float* values)
{
	glUseProgram(m_ProgramId);
	glUniformMatrix3fv(location, 1, GL_FALSE, values);
}

void Vin::OpenGLProgram::SetMat4(int location, float* values)
{
	glUseProgram(m_ProgramId);
	glUniformMatrix4fv(location, 1, GL_FALSE, values);
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
	return true;
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
	return true;
}
