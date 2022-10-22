#include "rendertask.hpp"

Vin::RenderTask::RenderTask(std::shared_ptr<Camera> camera, Primitive& primitive, Matrix4x4<float> model)
    : m_Id{ 0 }, m_Camera{ camera }, m_Primitive{ &primitive }, m_Model{ model }, m_Valid{ true }
{
    unsigned char viewportId = camera->GetId();
    unsigned char transluencyType = primitive.material->GetTransparency() ? 0 : 1;
    unsigned int depth = 0; //Calculate depth
    unsigned int materialId = primitive.material->GetId();

    m_Id += viewportId;
    m_Id = (m_Id << 4) + transluencyType;
    m_Id = (m_Id << 24) + (depth & 0xFFFFFF);
    m_Id = (m_Id << 32) + (materialId);
}

std::shared_ptr<Vin::Camera> Vin::RenderTask::GetCamera()
{
    return m_Camera;
}

Vin::Primitive& Vin::RenderTask::GetPrimitive()
{
    return *m_Primitive;
}

Vin::Matrix4x4<float>& Vin::RenderTask::GetModel()
{
    return m_Model;
}

bool Vin::RenderTask::IsValid()
{
    return m_Valid;
}

unsigned long long Vin::RenderTask::GetId()
{
    return m_Id;
}