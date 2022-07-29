#pragma once

namespace Vin {

	struct WindowResizeInfo {
		int width;
		int height;

		WindowResizeInfo(int width, int height) : width{ width }, height{ height } {};
	};

}
