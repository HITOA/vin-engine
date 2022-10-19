#include "default.hpp"

static std::shared_ptr<Vin::Program> defaultProgram{};

void Vin::SetDefaultProgram(std::shared_ptr<Program> program)
{
	defaultProgram = program;
}

std::shared_ptr<Vin::Program> Vin::GetDefaultProgram()
{
	return defaultProgram;
}
