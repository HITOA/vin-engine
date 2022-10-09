#pragma once

#include "vinpch.hpp"
#include "renderer/texture.hpp"
#include "renderer/program.hpp"

namespace Vin {

	class Fallback {
	public:
		static std::shared_ptr<Texture> GetFallbackTexture();
		static std::shared_ptr<Program> GetFallbackProgram();
	};

}
