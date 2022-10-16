#include "pathnormalizer.hpp"

std::string Vin::NormalizeStringPath(const std::string& strpath)
{
	std::filesystem::path path{ strpath };

	//Why the fuck this function make the application freeze some time ? why the fuck this function is so slow ??
	//path = std::filesystem::weakly_canonical(path); 

	return path.make_preferred().string();
}