#pragma once

#include "assets/assetregistry.hpp"

namespace Vin {
	class EditorAssetExplorer {
	public:
		void Draw(bool* drawWindow);
	private:
		void DrawAssetExplorer();
	};
}
