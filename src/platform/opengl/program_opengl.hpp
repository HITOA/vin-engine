#pragma once

#include "vinpch.hpp"

#include "renderer/program.hpp"

namespace Vin {
	class OpenGLProgram : public Program {
	public:
		void Bind() final;
		void Unbind() final;

		bool AddShader(ShaderType type, const char* src) final;
		bool CompileProgram() final;
		bool IsShaderComplete() final;
	private:
		int ParseShaderType(ShaderType type);
		bool CheckForShaderCompilationErr(unsigned int shaderId);
		bool CheckForProgramCompilationErr(unsigned int programId);
	private:
		unsigned int m_ProgramId{0};
		bool m_IsComplete{false};
		std::vector<unsigned int> m_Shaders{};
	};
}
