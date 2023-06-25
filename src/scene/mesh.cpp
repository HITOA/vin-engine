#include "mesh.hpp"

void Vin::StaticMesh::AddPrimitive(Primitive primitive)
{
	m_Primitives.push_back(primitive);
}

Vin::DynamicMesh::DynamicMesh(const VertexBufferLayout& layout, BufferIndexType type)
{
	m_VBO = VertexBuffer::Create(0);
	m_VBO->SetBufferLayout(layout);

	m_IBO = IndexBuffer::Create(type);

	m_VAO = VertexArray::Create();
	m_VAO->AddVertexBuffer(m_VBO);
	m_VAO->SetIndexBuffer(m_IBO);
}

void Vin::DynamicMesh::SetVertexBufferLayout(const VertexBufferLayout& layout)
{
	m_VBO->SetBufferLayout(layout);
	m_VAO->AddVertexBuffer(m_VBO);
}

void Vin::DynamicMesh::SetIndexBufferType(BufferIndexType type)
{
	m_IBO = IndexBuffer::Create(type);
	m_VAO->SetIndexBuffer(m_IBO);

	for (int i = 0; i < GetMaterialCount(); ++i) {
		m_Primitives[i].ibo = m_IBO;
	}
}

void Vin::DynamicMesh::SetVertexData(void* data, uint32_t vertexCount, BufferUsageType usage)
{
	size_t size = vertexCount * m_VBO->GetBufferLayout().GetStride();
	m_VBO->SetData(data, size, 0, false, usage);
}

void Vin::DynamicMesh::SetIndexData(void* data, uint32_t indexCount, BufferUsageType usage)
{
	m_IBO->SetData(data, indexCount, usage);
}

void Vin::DynamicMesh::SetMaterialCount(size_t count)
{
	if (count == GetMaterialCount())
		return;

	if (count < GetMaterialCount()) {
		m_Primitives.resize(count);
		return;
	}

	for (int i = GetMaterialCount(); i < count; ++i) {
		Primitive primitive{};
		primitive.vbos.push_back(m_VBO);
		primitive.ibo = m_IBO;
		primitive.vao = m_VAO;
		primitive.indexed = true;
		primitive.vertexCount = vertexCount;
		m_Primitives.push_back(primitive);
	}
}

size_t Vin::DynamicMesh::GetMaterialCount()
{
	return m_Primitives.size();
}

void Vin::DynamicMesh::SetMaterial(std::shared_ptr<Material> material, size_t idx)
{
	if (idx < 0 || idx >= m_Primitives.size())
		return;
	m_Primitives[idx].material = material;
}

std::shared_ptr<Vin::Material> Vin::DynamicMesh::GetMaterial(size_t idx)
{
	if (idx < 0 || idx >= m_Primitives.size())
		return std::shared_ptr<Material>();
	return m_Primitives[idx].material;
}
