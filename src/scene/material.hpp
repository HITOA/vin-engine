#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"
#include "renderer/program.hpp"
#include "renderer/texture.hpp"
#include "renderer/framebuffer.hpp"
#include "renderer/bindable.hpp"
#include "math/math.hpp"

#define VIN_MATERIAL_MAX_TEXTURES 128
#define VIN_MATERIAL_MAX_PROPERTIES 1024

namespace Vin {
	struct MaterialTextureData {
		int location{ -1 };
		std::shared_ptr<Bindable<unsigned short>> texture{ nullptr };
	};

	enum class MaterialPropertyType {
		None = 0,
		Float, Float2, Float3, Float4, Floats,
		Int, Int2, Int3, Int4, Ints,
		Mat3, Mat4
	};

	struct MaterialProperty {
		MaterialPropertyType type{ MaterialPropertyType::None };
		char data[64]{};
		int location{ -1 };
	};

	class Material {
	public:
		Material() = default;
		Material(std::shared_ptr<Program> program);

	public:
		void Bind();
		void Unbind();

		std::shared_ptr<Program> GetProgram();

		int GetField(const char* name);

		void SetFloat(int location, float value);
		void SetFloat(const char* name, float value) { SetFloat(GetField(name), value); };

		void SetFloat2(int location, Vector2<float> values);
		void SetFloat2(const char* name, Vector2<float> values) { SetFloat2(GetField(name), values); };

		void SetFloat3(int location, Vector3<float> values);
		void SetFloat3(const char* name, Vector3<float> values) { SetFloat3(GetField(name), values); };

		void SetFloat4(int location, Vector4<float> values);
		void SetFloat4(const char* name, Vector4<float> values) { SetFloat4(GetField(name), values); };

		void SetFloats(int location, float* values, size_t count);
		void SetFloats(const char* name, float* values, size_t count) { SetFloats(GetField(name), values, count); };

		void SetInt(int location, int value);
		void SetInt(const char* name, int value) { SetInt(GetField(name), value); };

		void SetInt2(int location, Vector2<int> values);
		void SetInt2(const char* name, Vector2<int> values) { SetInt2(GetField(name), values); };

		void SetInt3(int location, Vector3<int> values);
		void SetInt3(const char* name, Vector3<int> values) { SetInt3(GetField(name), values); };

		void SetInt4(int location, Vector4<int> values);
		void SetInt4(const char* name, Vector4<int> values) { SetInt4(GetField(name), values); };

		void SetInts(int location, int* values, size_t count);
		void SetInts(const char* name, int* values, size_t count) { SetInts(GetField(name), values, count); };

		void SetMat3(int location, Matrix3x3<float> values);
		void SetMat3(const char* name, Matrix3x3<float> values) { SetMat3(GetField(name), values); };

		void SetMat4(int location, Matrix4x4<float> values);
		void SetMat4(const char* name, Matrix4x4<float> values) { SetMat4(GetField(name), values); };

		void SetTexture(int location, std::shared_ptr<Texture> texture);
		void SetTexture(const char* name, std::shared_ptr<Texture> texture) { SetTexture(GetField(name), texture); };

		void SetTexture(int location, std::shared_ptr<RenderTexture> renderTexture);
		void SetTexture(const char* name, std::shared_ptr<RenderTexture> renderTexture) { SetTexture(GetField(name), renderTexture); };

		void SetTransparency(bool v);
		bool GetTransparency();

		void SetDoubleSided(bool v);
		bool GetDoubleSided();

		unsigned int GetId();
	private:
		void RecalculateId();

		void AddTextureData(MaterialTextureData&& data);
		void AddPropertyData(MaterialProperty&& data);

	private:
		std::shared_ptr<Program> m_Program{ nullptr };
		std::vector<MaterialTextureData> m_Textures{};
		std::vector<MaterialProperty> m_Properties{};
		//MaterialTextureData m_Textures[VIN_MATERIAL_MAX_TEXTURES]{};
		//MaterialProperty m_Properties[VIN_MATERIAL_MAX_PROPERTIES]{};
		bool m_Transparency{ false };
		bool m_DoubleSided{ false };
		unsigned int m_Id{};
	};
}
