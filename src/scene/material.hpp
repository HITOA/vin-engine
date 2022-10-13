#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"
#include "renderer/program.hpp"
#include "renderer/texture.hpp"
#include "renderer/framebuffer.hpp"
#include "renderer/bindable.hpp"

namespace Vin {
	struct MaterialTextureData {
		bool used{ false };
		std::shared_ptr<Bindable<unsigned short>> texture{ nullptr };
	};

	class Material {
	public:
		Material() = default;
		Material(std::shared_ptr<Program> program);

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

		void SetTexture(int location, std::shared_ptr<Texture> texture);
		void SetTexture(const char* name, std::shared_ptr<Texture> texture) { SetTexture(GetField(name), texture); };

		void SetTexture(int location, std::shared_ptr<RenderTexture> renderTexture);
		void SetTexture(const char* name, std::shared_ptr<RenderTexture> renderTexture) { SetTexture(GetField(name), renderTexture); };

	private:
		std::shared_ptr<Program> m_Program{ nullptr };
		MaterialTextureData m_Textures[16]{};
	};
}
