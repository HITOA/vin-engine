#include "renderpipeline.hpp"

void Vin::RenderPipeline::SetupMaterial(Material* mat)
{
	if (mat->GetDoubleSided())
		Renderer::SetCullMode(CullMode::None);
	else
		Renderer::SetCullMode(CullMode::Back);

	if (mat->GetTransparency())
		Renderer::SetBlendMode(BlendMode::Blend);
	else
		Renderer::SetBlendMode(BlendMode::None);
}

void Vin::RenderPipeline::SetupMainLight(Material* mat, Light light)
{
	mat->GetProgram()->SetFloat3("_MainLightDirection", light.direction.data);
	mat->GetProgram()->SetFloat3("_MainLightColor", (light.color * light.intensity).data);
}

void Vin::RenderPipeline::SetupAddtionalLight(Material* mat, Light* lights, int lightsCount)
{
	mat->GetProgram()->SetInt("_AdditionalLightsCount", lightsCount);

	static char buff[256]{};

	for (int i = 0; i < lightsCount; ++i) {
		sprintf(buff, "_AdditionalLightsPosition[%i]", i);
		mat->GetProgram()->SetFloat3(buff, lights[i].position.data);
		sprintf(buff, "_AdditionalLightsDirection[%i]", i);
		mat->GetProgram()->SetFloat3(buff, lights[i].direction.data);
		sprintf(buff, "_AdditionalLightsColor[%i]", i);
		mat->GetProgram()->SetFloat3(buff, (lights[i].color * lights[i].intensity).data);
		sprintf(buff, "_AdditionalLightsAttribute[%i]", i);
		mat->GetProgram()->SetFloat3(buff, Vector3<float>{lights[i].spotangle.xy, lights[i].range}.data);
		sprintf(buff, "_AdditionalLightsScale[%i]", i);
		mat->GetProgram()->SetFloat3(buff, lights[i].scale.data);
	}
}

void Vin::RenderPipeline::Draw(Primitive* primitive)
{
	if (primitive->indexed) {
		Renderer::DrawIndexed(primitive->vao);
	}
	else {
		Renderer::DrawArrays(primitive->vao, primitive->vertexCount);
	}
}
