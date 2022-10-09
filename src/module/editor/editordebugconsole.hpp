#pragma once

#include <vector>
#include <sstream>

#define EDITOR_CONSOLE_MAX_ENTRY 256

namespace Vin {
	class EditorDebugConsole : std::stringbuf {
	private:
		struct EntryData {
			std::string str;
			enum Type {
				Error,
				Warning,
				Info
			} type;

			EntryData(std::string&& str, Type type) : str{ str }, type{ type } {};
		};

	public:
		EditorDebugConsole();
		~EditorDebugConsole();

	public:
		std::streamsize xsputn(const char* s, std::streamsize n) override;
		void Draw(bool* drawWindow);

	private:
		int m_CurrentItemsPos{ 0 };
		std::vector<EntryData> m_Items{};
		std::ostream m_Ostream{ this };
	};
}
