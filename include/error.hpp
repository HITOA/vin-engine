#include <exception>

namespace Vin {
	void HandleUnmanagedError(const std::exception& err, int abort = 0);
}