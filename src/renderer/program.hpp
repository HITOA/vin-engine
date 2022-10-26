#pragma once

#include "vinpch.hpp"

#include "core/identifiable.hpp"

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

	class Program : public Identifiable<Program, unsigned short> {
	public:
		virtual ~Program() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Reset() = 0;

		virtual bool AddShader(ShaderType type, const char* src) = 0;
		virtual bool CompileProgram() = 0;
		virtual bool IsProgramComplete() = 0;

		virtual int GetField(const char* name) = 0;

		virtual void SetFloat(int location, float value) = 0;
		virtual void SetFloat(const char* name, float value) { SetFloat(GetField(name), value); };

		virtual void SetFloat2(int location, float* values) = 0;
		virtual void SetFloat2(const char* name, float* values) { SetFloat2(GetField(name), values); };

		virtual void SetFloat3(int location, float* values) = 0;
		virtual void SetFloat3(const char* name, float* values) { SetFloat3(GetField(name), values); };

		virtual void SetFloat4(int location, float* values) = 0;
		virtual void SetFloat4(const char* name, float* values) { SetFloat4(GetField(name), values); };

		virtual void SetFloats(int location, float* values, size_t count) = 0;
		virtual void SetFloats(const char* name, float* values, size_t count) { SetFloats(GetField(name), values, count); };

		virtual void SetInt(int location, int value) = 0;
		virtual void SetInt(const char* name, int value) { SetInt(GetField(name), value); };

		virtual void SetInt2(int location, int* values) = 0;
		virtual void SetInt2(const char* name, int* values) { SetInt2(GetField(name), values); };

		virtual void SetInt3(int location, int* values) = 0;
		virtual void SetInt3(const char* name, int* values) { SetInt3(GetField(name), values); };

		virtual void SetInt4(int location, int* values) = 0;
		virtual void SetInt4(const char* name, int* values) { SetInt4(GetField(name), values); };
		
		virtual void SetInts(int location, int* values, size_t count) = 0;
		virtual void SetInts(const char* name, int* values, size_t count) { SetInts(GetField(name), values, count); };

		virtual void SetMat3(int location, float* values) = 0;
		virtual void SetMat3(const char* name, float* values) { SetMat3(GetField(name), values); };

		virtual void SetMat4(int location, float* values) = 0;
		virtual void SetMat4(const char* name, float* values) { SetMat4(GetField(name), values); };

		static std::shared_ptr<Program> Create();
	};
}
