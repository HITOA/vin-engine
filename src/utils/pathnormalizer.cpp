#include "pathnormalizer.hpp"

std::string Vin::NormalizeStringPath(const std::string& strpath)
{
	std::filesystem::path path{ strpath };
	path = std::filesystem::weakly_canonical(path);
	return path.make_preferred().string();
}