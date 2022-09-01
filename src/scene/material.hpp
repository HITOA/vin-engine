#pragma once

#include "vinpch.hpp"
#include "renderer/program.hpp"
#include "renderer/texture.hpp"

namespace Vin {
	struct MaterialTextureData {
		bool used{ false };
		eastl::shared_ptr<Texture> texture{ nullptr };
	};

	class Material {
	public:
		Material() = delete;
		Material(eastl::shared_ptr<Program> program) : m_Program{ program }, m_Textures{} {};

	public:
		void Bind();

		int GetField(const char* name);

		void SetFloat(int location, float value);
		void SetFloat(const char* name, float value) { SetFloat(GetField(name), value); };

		void SetFloat2(int location, float* values);
		void SetFloat2(const char* name, float* values) { SetFloat2(GetField(name), values); };

		void SetFloat3(int location, float* values);
		void SetFloat3(const char* name, float* values) { SetFloat3(GetField(name), values); };

		void SetFloat4(int location, float* values);
		void SetFloat4(const char* name, float* values) { SetFloat4(GetField(name), values); };

		void SetFloats(int location, float* values, size_t count);
		void SetFloats(const char* name, float* values, size_t count) { SetFloats(GetField(name), values, count); };

		void SetInt(int location, int value);
		void SetInt(const char* name, int value) { SetInt(GetField(name), value); };

		void SetInt2(int location, int* values);
		void SetInt2(const char* name, int* values) { SetInt2(GetField(name), values); };

		void SetInt3(int location, int* values);
		void SetInt3(const char* name, int* values) { SetInt3(GetField(name), values); };

		void SetInt4(int location, int* values);
		void SetInt4(const char* name, int* values) { SetInt4(GetField(name), values); };

		void SetInts(int location, int* values, size_t count);
		void SetInts(const char* name, int* values, size_t count) { SetInts(GetField(name), values, count); };

		void SetMat3(int location, float* values);
		void SetMat3(const char* name, float* values) { SetMat3(GetField(name), values); };

		void SetMat4(int location, float* values);
		void SetMat4(const char* name, float* values) { SetMat4(GetField(name), values); };

		void SetTexture(int location, eastl::shared_ptr<Texture> texture);
		void SetTexture(const char* name, eastl::shared_ptr<Texture> texture) { SetTexture(GetField(name), texture); };

		static eastl::shared_ptr<Material> Create(eastl::shared_ptr<Program> program);

	private:
		eastl::shared_ptr<Program> m_Program{ nullptr };
		MaterialTextureData m_Textures[16]{};
	};
}
