#pragma once

#include "renderer/program.hpp"

namespace Vin {

	void SetDefaultProgram(std::shared_ptr<Program> program);
	std::shared_ptr<Program> GetDefaultProgram();

}
