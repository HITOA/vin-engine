#pragma once

#include "vinpch.hpp"
#include "renderer/texture.hpp"
#include "renderer/program.hpp"

namespace Vin {

	class Fallback {
	public:
		static eastl::shared_ptr<Texture> GetFallbackTexture();
		static eastl::shared_ptr<Program> GetFallbackProgram();
	};

}
