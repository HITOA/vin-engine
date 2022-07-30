#pragma once

namespace vin {
	enum class ShaderType {
		Unknown,
		ComputeShader,
		VertexShader,
		TessControl,
		TessEvaluation,
		GeometryShader,
		FragmentShader
	};

	class Program {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddShader(ShaderType type, const char* src) = 0;
		virtual void CompileProgram() = 0;
	};
}
