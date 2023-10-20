#ifndef VIN_ENGINE_CONSOLE_H
#define VIN_ENGINE_CONSOLE_H

#include "../editor.h"
#include <sstream>

#define EDITOR_CONSOLE_MAX_ENTRY 256

class Console : public EditorWindow, std::stringbuf {
private:
    struct EntryData {
        Vin::String str;
        enum Type {
            Error,
            Warning,
            Info
        } type;

        EntryData(Vin::String&& str, Type type) : str{ str }, type{ type } {};
    };

public:
    Console();

    std::streamsize xsputn(const char* s, std::streamsize n) override;
    void Draw(bool* open) final;

private:
    int currentItemsPos{ 0 };
    Vin::Vector<EntryData> items{};
    std::ostream ostream{ this };
};

#endif //VIN_ENGINE_CONSOLE_H
