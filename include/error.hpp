#pragma once

#include <exception>

namespace Vin {
	namespace Error {

	}
	void HandleUnmanagedError(const std::exception& err, int abort = 0);
}