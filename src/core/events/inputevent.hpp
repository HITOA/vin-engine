#pragma once

namespace Vin {

	struct WindowKeyInfo {
		int key;
		int scancode;
		int mods;

		WindowKeyInfo(int key, int scancode, int mods) : key{ key }, scancode{ scancode }, mods{ mods } {};
	};

	struct WindowMouseButtonInfo {
		int button;
		int mods;

		WindowMouseButtonInfo(int button, int mods) : button{ button }, mods{ mods } {};
	};

	struct WindowMouseScrollInfo {
		double xOffset;
		double yOffset;

		WindowMouseScrollInfo(double xOffset, double yOffset) : xOffset{ xOffset }, yOffset{ yOffset } {};
	};

	struct WindowMouseMoveInfo {
		double xPos;
		double yPos;

		WindowMouseMoveInfo(double xPos, double yPos) : xPos{ xPos }, yPos{ yPos } {};
	};

}
