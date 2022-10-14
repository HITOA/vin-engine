#pragma once

#include "assets/assetserdes.hpp"
#include "scene/mesh.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "logger/logger.hpp"

namespace Vin {

	struct Vertex {
		Vector3<float> position{ -1 };
		Vector3<float> normals{ -1 };
		Vector2<float> uv{ -1 };
	};

	template<>
	struct AssetDeserializer<StaticMesh> {
		static std::shared_ptr<StaticMesh> Deserialize(std::unique_ptr<File> file) {
			size_t filesize = file->GetSize();
			char* buff = new char[filesize];

			file->ReadBytes(buff, filesize);

			tinyobj::ObjReader objreader{};

			tinyobj::ObjReaderConfig config{};
			config.triangulate = true;

			objreader.ParseFromString(std::string{ buff }, "", config);

			delete[] buff;

			if (!objreader.Valid()) {
				Logger::Err("Error while loading static mesh : {}", objreader.Error());
				return nullptr;
			}

			if (!objreader.Warning().empty())
				Logger::Err("{}", objreader.Warning());

			const tinyobj::attrib_t& attrib = objreader.GetAttrib();
			const std::vector<tinyobj::shape_t>& shapes = objreader.GetShapes();

			std::shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>();
			
			for (const auto& shape : shapes) {
				Primitive prim{};

				std::vector<Vertex> vertices{};
				for (const auto& index : shape.mesh.indices) {
					Vertex vertex{};

					if (index.vertex_index >= 0) {
						vertex.position = {
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
						};
					}

					if (index.normal_index >= 0) {
						vertex.normals = {
							attrib.normals[3 * index.normal_index + 0],
							attrib.normals[3 * index.normal_index + 1],
							attrib.normals[3 * index.normal_index + 2]
						};
					}

					if (index.texcoord_index >= 0) {
						vertex.uv = {
							attrib.texcoords[2 * index.texcoord_index + 0],
							attrib.texcoords[2 * index.texcoord_index + 1]
						};
					}

					vertices.push_back(vertex);
				}

				std::shared_ptr<VertexBuffer> vbo = VertexBuffer::Create(sizeof(Vertex) * vertices.size());
				vbo->SetData(vertices.data(), sizeof(Vertex) * vertices.size(), 0);
				vbo->SetBufferLayout({ 
					{ Vin::VertexAttribute::Position, Vin::VertexAttributeType::Float3 },
					{ Vin::VertexAttribute::Normal, Vin::VertexAttributeType::Float3 },
					{ Vin::VertexAttribute::TextureCoord0, Vin::VertexAttributeType::Float2 } });

				prim.vbos.push_back(vbo);
				prim.vao = VertexArray::Create();
				prim.vao->AddVertexBuffer(vbo);
				prim.indexed = false;
				prim.vertexCount = vertices.size();

				mesh->AddPrimitive(prim);
			}

			return mesh;
		}
	};

}

