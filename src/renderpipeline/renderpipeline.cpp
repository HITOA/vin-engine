#include "renderpipeline.hpp"

#define CSTRCPY(dst, src) memcpy(dst, src, sizeof(src))

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

#define MAX_LIGHT_COUNT 8

#define CAT(a, ...) a ## __VA_ARGS__


void Vin::RenderPipeline::SetupAddtionalLight(Material* mat, Light* lights, int lightsCount)
{
	mat->GetProgram()->SetInt("_AdditionalLightsCount", lightsCount);

	static char buff[256]{};

	CAT(MAX_LIGHT_, COUNT);

	for (int i = 0; i < lightsCount; ++i) {

		CSTRCPY(buff, "_AdditionalLightsPosition[");
		itoa(i, buff + sizeof("_AdditionalLightsPosition[") - 1, 10);
		strcat(buff, "]");
		mat->GetProgram()->SetFloat3(buff, lights[i].position.data);

		CSTRCPY(buff, "_AdditionalLightsDirection[");
		itoa(i, buff + sizeof("_AdditionalLightsDirection[") - 1, 10);
		strcat(buff, "]");
		mat->GetProgram()->SetFloat3(buff, lights[i].direction.data);

		CSTRCPY(buff, "_AdditionalLightsColor[");
		itoa(i, buff + sizeof("_AdditionalLightsColor[") - 1, 10);
		strcat(buff, "]");
		mat->GetProgram()->SetFloat3(buff, (lights[i].color * lights[i].intensity).data);

		if (lights[i].type == LightType::Spot) {
			CSTRCPY(buff, "_AdditionalLightsAttribute[");
			itoa(i, buff + sizeof("_AdditionalLightsAttribute[") - 1, 10);
			strcat(buff, "]");
			mat->GetProgram()->SetFloat3(buff, Vector3<float>{Vin::Cos<float>((lights[i].spotangle.x + lights[i].spotangle.y) * (float)Vin::deg2rad),
				Vin::Cos<float>(lights[i].spotangle.x* (float)Vin::deg2rad), lights[i].range}.data);
		}
		else {
			CSTRCPY(buff, "_AdditionalLightsAttribute[");
			itoa(i, buff + sizeof("_AdditionalLightsAttribute[") - 1, 10);
			strcat(buff, "]");
			mat->GetProgram()->SetFloat3(buff, Vector3<float>{-2, 1, lights[i].range}.data);
		}
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
