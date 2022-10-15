#pragma once

#include "vinpch.hpp"

#include "renderer/program.hpp"

namespace Vin {

	std::shared_ptr<Program> LoadProgram(std::string_view vspath, std::string_view fspath);

}
