#pragma once

#include "resource.hpp"

namespace Vin {

	class Model : public Resource {
	public:
		void Load(std::unique_ptr<GameFile> file);
		void Unload();
	};

}
