#include "gltfutils.hpp"
#include "resources/resources.hpp"
#include "resources/rawfile.hpp"

//#define TINYGLTF_NO_FS
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

std::shared_ptr<Vin::StaticMesh> Vin::LoadGLB(const char* path)
{
	/*std::shared_ptr<RawFile> src = Resources::Load<RawFile>(path);

	if (!src) {
		Vin::Logger::Err("File {} does not exists.", path);
		return nullptr;
	}

	tinygltf::Model model{};
	tinygltf::TinyGLTF loader{};
	std::string err{};
	std::string warn{};

	bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, (const unsigned char*)src->GetData(), src->GetSize());

	if (!err.empty())
		Vin::Logger::Err("GLB Loader error : {}", err.c_str());

	if (!warn.empty())
		Vin::Logger::Warn("GLB Loader warning : {}", warn.c_str());

	if (!ret) {
		Vin::Logger::Err("Failed to load glb file : {}", path);
		return nullptr;
	}

	tinygltf::Mesh& mesh = model.meshes[0];

	for (auto& pair : mesh.primitives[0].attributes) {
		tinygltf::Accessor& accessor = model.accessors[pair.second];
		tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
		Logger::Log("{}, {}", pair.first, pair.second);
	}
	Logger::Log("INDICES, {}", mesh.primitives[0].indices);
	*/
	return nullptr;
	//std::shared_ptr<StaticMesh> staticmesh = std::make_shared<StaticMesh>(;
}
