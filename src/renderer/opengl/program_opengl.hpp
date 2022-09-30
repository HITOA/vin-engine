#pragma once

#include "vinpch.hpp"

#include "renderer/program.hpp"

namespace Vin {
	class OpenGLProgram : public Program {
	public:
		OpenGLProgram();
		~OpenGLProgram();

		void Bind() final;
		void Unbind() final;

		bool AddShader(ShaderType type, const char* src) final;
		bool CompileProgram() final;
		bool IsProgramComplete() final;

		int GetField(const char* name) final;

		void SetFloat(int location, float value) final;
		void SetFloat2(int location, float* values) final;
		void SetFloat3(int location, float* values) final;
		void SetFloat4(int location, float* values) final;
		void SetFloats(int location, float* values, size_t count) final;
		void SetInt(int location, int value) final;
		void SetInt2(int location, int* values) final;
		void SetInt3(int location, int* values) final;
		void SetInt4(int location, int* values) final;
		void SetInts(int location, int* values, size_t count) final;
		void SetMat3(int location, float* values) final;
		void SetMat4(int location, float* values) final;

	private:
		int ParseShaderType(ShaderType type);
		bool CheckForShaderCompilationErr(unsigned int shaderId);
		bool CheckForProgramCompilationErr(unsigned int programId);
	private:
		unsigned int m_ProgramId{ 0 };
		bool m_IsShaderComplete{ true };
		bool m_IsProgramComplete{ false };
		eastl::fixed_vector<unsigned int, 5, false> m_Shaders{};
	};
}
