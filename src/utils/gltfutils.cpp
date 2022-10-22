#include "gltfutils.hpp"

//#define TINYGLTF_NO_FS
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

#include "logger/logger.hpp"
#include "scene/meshrenderer.hpp"
#include "resources/default.hpp"

static bool isInit{ false };

bool FileExists(const std::string& abs_filename, void*) {
	return Vin::VFS::Exists(abs_filename);
}

std::string ExpandFilePath(const std::string& path, void*) {
	return path;
}

bool ReadWholeFile(std::vector<unsigned char>* out, std::string* err,
	const std::string& filepath, void*) {

	if (!Vin::VFS::Exists(filepath)) {
		if (err) {
			(*err) += "File open error : " + filepath + "\n";
		}
		return false;
	}

	std::shared_ptr<Vin::File> file = Vin::VFS::Open(filepath, Vin::FileMode::Read);

	size_t filesize = file->GetSize();

	if (filesize == 0) {
		if (err) {
			(*err) += "Invalid file size : " + filepath + " (does the path point to a directory?)";
		}
		return false;
	}

	out->resize(filesize);
	file->ReadBytes((char*)out->data(), filesize);
	file->Close();
	return true;
}

bool WriteWholeFile(std::string* err, const std::string& filepath, 
	const std::vector<unsigned char>& contents, void*) {
	if (err)
		(*err) += "Writing not allowed.";
	return false;
}

Vin::VertexAttribute ParseGLTFAttributeName(const std::string& name) {
	if (name == "POSITION")
		return Vin::VertexAttribute::Position;
	if (name == "NORMAL")
		return Vin::VertexAttribute::Normal;
	if (name == "TANGENT")
		return Vin::VertexAttribute::Tangent;
	if (name == "COLOR_0")
		return Vin::VertexAttribute::Color;

	if (name == "TEXCOORD_0")
		return Vin::VertexAttribute::TextureCoord0;
	if (name == "TEXCOORD_1")
		return Vin::VertexAttribute::TextureCoord1;
	if (name == "TEXCOORD_2")
		return Vin::VertexAttribute::TextureCoord2;
	if (name == "TEXCOORD_3")
		return Vin::VertexAttribute::TextureCoord3;
	if (name == "TEXCOORD_4")
		return Vin::VertexAttribute::TextureCoord4;
	if (name == "TEXCOORD_5")
		return Vin::VertexAttribute::TextureCoord5;
	if (name == "TEXCOORD_6")
		return Vin::VertexAttribute::TextureCoord6;
	if (name == "TEXCOORD_7")
		return Vin::VertexAttribute::TextureCoord7;
	
	return Vin::VertexAttribute::Position;
}

Vin::VertexAttributeType ParseGLTFAttributeType(int componentType, int type) {
	switch (componentType) {
	case TINYGLTF_COMPONENT_TYPE_FLOAT: {
		switch (type) {
		case TINYGLTF_TYPE_SCALAR: return Vin::VertexAttributeType::Float;
		case TINYGLTF_TYPE_VEC2: return Vin::VertexAttributeType::Float2;
		case TINYGLTF_TYPE_VEC3: return Vin::VertexAttributeType::Float3;
		case TINYGLTF_TYPE_VEC4: return Vin::VertexAttributeType::Float4;
		default: return Vin::VertexAttributeType::None;
		}
	}
	case TINYGLTF_COMPONENT_TYPE_INT: {
		switch (type) {
		case TINYGLTF_TYPE_SCALAR: return Vin::VertexAttributeType::Int;
		case TINYGLTF_TYPE_VEC2: return Vin::VertexAttributeType::Int2;
		case TINYGLTF_TYPE_VEC3: return Vin::VertexAttributeType::Int3;
		case TINYGLTF_TYPE_VEC4: return Vin::VertexAttributeType::Int4;
		default: return Vin::VertexAttributeType::None;
		}
	}
	default:
		return Vin::VertexAttributeType::Float3;
	}
}

Vin::TextureFormat ParseGLTFTextureFormat(int component, int depth) {
	if (component == 4 && depth == 8)
		return Vin::TextureFormat::RGBA32;
	if (component == 3 && depth == 8)
		return Vin::TextureFormat::RGB24;
	if (component == 2 && depth == 8)
		return Vin::TextureFormat::RG16;
	if (component == 1 && depth == 8)
		return Vin::TextureFormat::R8;
	if (component == 1 && depth == 16)
		return Vin::TextureFormat::R16;
	return Vin::TextureFormat::BGRA32;
}

void AddTexture(tinygltf::Model& model, Vin::Asset<Vin::Material> material, int index, 
	const std::string& assetBaseName, std::string_view textureFieldName) {

	std::string texturePath = assetBaseName + "texture_" + std::to_string(index);

	Vin::Asset<Vin::Texture> texture = Vin::AssetDatabase::GetAsset<Vin::Texture>(texturePath);

	if (texture.Get() != nullptr) {
		material->SetTexture(textureFieldName.data(), texture);
		return;
	}

	tinygltf::Texture& gltfTexture = model.textures[index];
	tinygltf::Image& gltfImage = model.images[gltfTexture.source];

	std::shared_ptr<Vin::Texture> image = Vin::Texture::Create(
		gltfImage.width, gltfImage.height, ParseGLTFTextureFormat(gltfImage.component, gltfImage.bits));

	image->SetData(gltfImage.image.data());

	material->SetTexture(textureFieldName.data(), image);

	texture = Vin::AssetDatabase::AddAsset<Vin::Texture>(image, texturePath);
}

void BuildNode(tinygltf::Model& model, std::shared_ptr<Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous>> scene, 
	int nodeIdx, Vin::EntityId parent, std::string_view path) {

	Vin::Transform<float> transform{};

	transform.parent = parent;

	if (model.nodes[nodeIdx].translation.size() == 3)
		transform.position = Vin::Vector3<float>{ 
		(float)model.nodes[nodeIdx].translation[0], 
		(float)model.nodes[nodeIdx].translation[1], 
		(float)model.nodes[nodeIdx].translation[2] };

	if (model.nodes[nodeIdx].rotation.size() == 4)
		transform.rotation = Vin::Quaternion<float>{
		(float)model.nodes[nodeIdx].rotation[3],
		(float)model.nodes[nodeIdx].rotation[0],
		(float)model.nodes[nodeIdx].rotation[1],
		(float)model.nodes[nodeIdx].rotation[2] };

	if (model.nodes[nodeIdx].scale.size() == 3)
		transform.scale = Vin::Vector3<float>{
		(float)model.nodes[nodeIdx].scale[0],
		(float)model.nodes[nodeIdx].scale[1],
		(float)model.nodes[nodeIdx].scale[2] };

	Vin::EntityId nodeId = (*scene)->CreateEntity(transform); //Transform at least

	if (model.nodes[nodeIdx].mesh >= 0) {
		std::string assetBaseName{ path };
		assetBaseName += "//";

		Vin::Asset<Vin::StaticMesh> mesh = Vin::AssetDatabase::AddAsset<Vin::StaticMesh>(Vin::StaticMesh{}, assetBaseName + "mesh_" + std::to_string(nodeIdx));
		mesh.SetFlags(Vin::AssetFlag::Persistent);

		for (tinygltf::Primitive& gltfprimitive : model.meshes[model.nodes[nodeIdx].mesh].primitives) {
			Vin::Primitive primitive{};

			primitive.vao = Vin::VertexArray::Create();

			for (auto& attribute : gltfprimitive.attributes) {
				tinygltf::Accessor& accessor = model.accessors[attribute.second];

				if (accessor.bufferView < 0)
					continue;

				Vin::VertexAttribute attribname = ParseGLTFAttributeName(attribute.first);
				Vin::VertexAttributeType attribtype = ParseGLTFAttributeType(accessor.componentType, accessor.type);
				bool normalized = accessor.normalized;

				tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				if (attribname == Vin::VertexAttribute::Position)
					primitive.vertexCount = accessor.count;
				
				std::shared_ptr<Vin::VertexBuffer> vbo = Vin::VertexBuffer::Create(accessor.count * Vin::GetVertexAttributeTypeSize(attribtype));

				vbo->SetData(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count * Vin::GetVertexAttributeTypeSize(attribtype), 0);
				vbo->SetBufferLayout(Vin::VertexBufferLayout{ {attribname, attribtype, normalized} });

				primitive.vbos.push_back(vbo);
				primitive.vao->AddVertexBuffer(vbo);
			}

			if (gltfprimitive.indices >= 0) {
				tinygltf::Accessor& accessor = model.accessors[gltfprimitive.indices];

				Vin::BufferIndexType type = accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT ?
					Vin::BufferIndexType::UnsignedInt32 : Vin::BufferIndexType::UnsignedInt16;

				primitive.ibo = Vin::IndexBuffer::Create(type);

				tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				primitive.ibo->SetData(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count);

				primitive.vao->SetIndexBuffer(primitive.ibo);

				primitive.indexed = true;
			}
			else {
				primitive.indexed = false;
			}

			if (gltfprimitive.material != -1) {
				Vin::Asset<Vin::Material> material =
					Vin::AssetDatabase::GetAsset<Vin::Material>(
						assetBaseName + "material_" + std::to_string(gltfprimitive.material));

				if (material.Get() == nullptr) {
					//Build Material

					material = Vin::AssetDatabase::AddAsset<Vin::Material>(Vin::Material{ Vin::GetDefaultProgram() },
						assetBaseName + "material_" + std::to_string(gltfprimitive.material));
					tinygltf::Material& gltfmaterial = model.materials[gltfprimitive.material];

					if (gltfmaterial.pbrMetallicRoughness.baseColorTexture.index != -1)
						AddTexture(model, material, gltfmaterial.pbrMetallicRoughness.baseColorTexture.index, assetBaseName, "_MainTex");
					if (gltfmaterial.normalTexture.index != -1)
						AddTexture(model, material, gltfmaterial.normalTexture.index, assetBaseName, "_NormalTex");
					if (gltfmaterial.pbrMetallicRoughness.metallicRoughnessTexture.index != -1)
						AddTexture(model, material, gltfmaterial.pbrMetallicRoughness.metallicRoughnessTexture.index, assetBaseName, "_MetallicRoughnessTex");


					if (gltfmaterial.alphaMode == "BLEND") {
						material->SetTransparency(true);
					}

					material->SetFloat3("_Albedo", Vin::Vector3<float>{
						(float)gltfmaterial.pbrMetallicRoughness.baseColorFactor[0],
							(float)gltfmaterial.pbrMetallicRoughness.baseColorFactor[1],
							(float)gltfmaterial.pbrMetallicRoughness.baseColorFactor[2]});

					material->SetFloat("_Metallic", gltfmaterial.pbrMetallicRoughness.metallicFactor);
					material->SetFloat("_Roughness", gltfmaterial.pbrMetallicRoughness.roughnessFactor);
					material->SetFloat("_AlphaCutoff", gltfmaterial.alphaCutoff);

					Vin::Logger::Log("Cutoff : {}", gltfmaterial.alphaCutoff);

					material->SetDoubleSided(gltfmaterial.doubleSided);
				}

				primitive.material = material;
			}

			mesh->AddPrimitive(primitive);
		}

		Vin::MeshRenderer renderer{ mesh.Get() };
		(*scene)->AddComponent(nodeId, renderer);
	}

	for (int childrenIdx : model.nodes[nodeIdx].children) {
		BuildNode(model, scene, childrenIdx, nodeId, path);
	}
}

void BuildScene(tinygltf::Model& model, std::shared_ptr<Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous>> scene, std::string_view path) {
	for (int nodeIdx : model.scenes[model.defaultScene].nodes) {
		BuildNode(model, scene, nodeIdx, 0, path);
	}
}

std::shared_ptr<Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous>> Vin::LoadGLTF(std::string_view path)
{
	tinygltf::FsCallbacks fscallback{};

	fscallback.FileExists = FileExists;
	fscallback.ExpandFilePath = ExpandFilePath;
	fscallback.ReadWholeFile = ReadWholeFile;
	fscallback.WriteWholeFile = WriteWholeFile;

	tinygltf::TinyGLTF loader{};
	std::string err;
	std::string warn;
	loader.SetFsCallbacks(fscallback);

	tinygltf::Model model;

	bool res = loader.LoadASCIIFromFile(&model, &err, &warn, std::string{ path });

	if (!warn.empty()) {
		Logger::Warn("{}", warn);
		return nullptr;
	}
	if (!err.empty()) {
		Logger::Err("{}", err);
		return nullptr;
	}

	if (!res) {
		Logger::Err("failed to load GLTF");
		return nullptr;
	}

	std::shared_ptr<Scene<ArchetypeMemoryLayout::Contiguous>> scene =
		std::make_shared< Scene<ArchetypeMemoryLayout::Contiguous>>();

	BuildScene(model, scene, path);

	return scene;
}
