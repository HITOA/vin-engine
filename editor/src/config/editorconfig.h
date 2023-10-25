#ifndef VIN_ENGINE_EDITORCONFIG_H
#define VIN_ENGINE_EDITORCONFIG_H

#include <vin/vin.h>

struct Section {
    Vin::UnorderedMap<Vin::String, Vin::String> properties{};
};

struct Config {
    Vin::UnorderedMap<Vin::String, Section> sections{};
};

class EditorConfig {
public:
    virtual void Serialize(Config& config) = 0;
    virtual void Deserialize(const Config& config) = 0;
};

Vin::String SerializeConfig(Config& config);
Config DeserializeConfig(Vin::StringView data);

Vin::String GetConfigDir();

#endif //VIN_ENGINE_EDITORCONFIG_H
