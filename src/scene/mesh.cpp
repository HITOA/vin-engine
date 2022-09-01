#include "mesh.hpp"

Vin::StaticMesh::StaticMesh(VertexBufferLayout layout) : m_VertexLayout{ layout } {}

void Vin::StaticMesh::AddSubMeshData(SubMeshData&& submesh)
{
}

void Vin::StaticMesh::AddMaterial(eastl::shared_ptr<Material> material)
{
	m_Materials.push_back(material);
}

Vin::VertexBufferLayout Vin::StaticMesh::GetVertexLayout() const
{
	return m_VertexLayout;
}

bool Vin::StaticMesh::HasVertexAttribute(VertexAttribute attribute) const
{
	for (auto& element : m_VertexLayout)
		if (element.attribute == attribute)
			return true;
	return false;
}

Vin::VertexAttributeType Vin::StaticMesh::GetVertexAttributeType(VertexAttribute attribute) const
{
	for (auto& element : m_VertexLayout)
		if (element.attribute == attribute)
			return element.type;
	return VertexAttributeType::None;
}
