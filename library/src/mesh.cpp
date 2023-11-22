#include <vin/scene/resources/mesh.h>

Vin::Mesh::~Mesh() {
    bgfx::destroy(vbo);
    bgfx::destroy(ibo);
}

const bgfx::VertexLayout& Vin::Mesh::GetVertexLayout() {
    return vertexLayout;
}

bgfx::VertexBufferHandle Vin::Mesh::GetVertexBufferHandle() {
    return vbo;
}

bgfx::IndexBufferHandle Vin::Mesh::GetIndexBufferHandle() {
    return ibo;
}

size_t Vin::Mesh::GetPrimitivesCount() {
    return primitives.size();
}

const Vin::Primitive& Vin::Mesh::GetPrimitive(size_t idx) {
    return primitives[idx];
}

Vin::Vector<Vin::Primitive>::const_iterator Vin::Mesh::begin() const {
    return primitives.begin();
}

Vin::Vector<Vin::Primitive>::const_iterator Vin::Mesh::end() const {
    return primitives.end();
}

Vin::Vector<Vin::Primitive>::const_iterator Vin::Mesh::cbegin() const {
    return primitives.cbegin();
}

Vin::Vector<Vin::Primitive>::const_iterator Vin::Mesh::cend() const {
    return primitives.cend();
}