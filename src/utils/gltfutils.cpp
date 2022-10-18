#include "gltfutils.hpp"

//#define TINYGLTF_NO_FS
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

#include "logger/logger.hpp"

/*
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
	//return nullptr;
	//std::shared_ptr<StaticMesh> staticmesh = std::make_shared<StaticMesh>(;
//}

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

void BuildScene(tinygltf::Model& model, std::shared_ptr<Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous>> scene) {

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

	BuildScene(model, scene);

	return scene;
}
