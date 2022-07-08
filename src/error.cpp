#include <error.hpp>
#include "msgbox.hpp"

namespace Vin {

	void HandleUnmanagedError(const std::exception& err, int abort)
	{
		Msgbox::ShowErr("Error", err.what());

		if (abort)
			std::abort();
	}

}
