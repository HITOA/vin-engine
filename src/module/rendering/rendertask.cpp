#include "rendertask.hpp"

Vin::RenderTask::RenderTask(std::shared_ptr<Camera> camera, Primitive& primitive) : m_Id{ 0 }, m_Camera{ camera }, m_Primitive{ primitive }
{
    unsigned char viewportId = camera->GetId();
    unsigned char transluencyType = primitive.material->GetTransparency();
    unsigned int depth = 0; //Calculate depth
    unsigned int materialId = primitive.material->GetId();

    m_Id += viewportId;
    m_Id = (m_Id << 4) + transluencyType;
    m_Id = (m_Id << 24) + (depth & 0xFFFFFF);
    m_Id = (m_Id << 32) + (materialId);
}

unsigned long long Vin::RenderTask::GetId()
{
    return m_Id;
}