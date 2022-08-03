#pragma once

#include "vinpch.hpp"

namespace Vin {
	enum class ShaderType {
		Unknown = 0,
		ComputeShader,
		VertexShader,
		TessControl,
		TessEvaluation,
		GeometryShader,
		FragmentShader
	};

	class Program {
	public:
		virtual ~Program() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual bool AddShader(ShaderType type, const char* src) = 0;
		virtual bool CompileProgram() = 0;
		virtual bool IsShaderComplete() = 0;

		static std::unique_ptr<Program> Create();
	};
}
