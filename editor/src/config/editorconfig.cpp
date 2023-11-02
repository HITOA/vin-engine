#include "editorconfig.h"
#include <filesystem>

bool IsValidIdentifier(Vin::StringView identifier) {
    return std::all_of(identifier.begin(), identifier.end(), isalnum);
}

Vin::String SerializeConfig(Config& config) {
    Vin::String data{};
    for (auto& section : config.sections) {
        if (!IsValidIdentifier(section.first)) {
            Vin::Logger::Err("Invalid section name \"", section.first, "\" while trying to serialize config file.");
            return "";
        }

        data += "[" + section.first + "]\n";
        for (auto& property : section.second.properties) {
            if (!IsValidIdentifier(property.first)) {
                Vin::Logger::Err("Invalid key name \"", property.first, "\" while trying to serialize config file.");
                return "";
            }
            data += property.first + " = " + property.second + "\n";
        }
    }
    return data;
}

size_t CountWhitespace(Vin::StringView str) {
    size_t c = 0;
    while (c < str.size() && isspace(str[c]))
        ++c;
    return c;
}

Config DeserializeConfig(Vin::StringView data) {
    Config config{};
    Vin::String currentSection{};
    config.sections.insert(std::make_pair(currentSection, Section{}));

    size_t c = 0;
    while (c < data.size()) {
        if (isalnum(data[c])) {
            size_t l = 0;
            while (c + l < data.size() && isalnum(data[c + l]))
                ++l;
            if (c + l >= data.size()) {
                Vin::Logger::Err("Config deserialization error, incomplete property.");
                return Config{};
            }
            Vin::String key{ data.substr(c, l) };
            c += l;
            c += CountWhitespace(&data[c]);
            if (data[c++] != '=') {
                Vin::Logger::Err("Config deserialization error, missing symbol.");
                return Config{};
            }
            c += CountWhitespace(&data[c]);
            l = 0;
            while (c + l < data.size() && data[c + l] != '\n')
                ++l;
            if (l == 0) {
                Vin::Logger::Err("Config deserialization error, missing value.");
                return Config{};
            }
            Vin::String value{ data.substr(c, l) };
            if (config.sections[currentSection].properties.count(key)) {
                Vin::Logger::Err("Config deserialization error, duplicated key.");
                return Config{};
            }
            config.sections[currentSection].properties[key] = value;
            c += l;
        }
        if (data[c] == '[') {
            ++c;
            auto it = std::find(data.begin() + c, data.end(), ']');
            if (it == data.end()) {
                Vin::Logger::Err("Config deserialization error, bad section.");
                return Config{};
            }
            size_t l = std::distance(data.begin() + c, it);
            currentSection = data.substr(c, l);
            c += l;
            if (config.sections.count(currentSection)) {
                Vin::Logger::Err("Config deserialization error, you can't have section with the same name.");
                return Config{};
            }
            config.sections[currentSection] = Section{};
        }
        ++c;
    }

    return config;
}

Vin::String GetConfigDir() {
#ifdef VIN_WIN32
    std::filesystem::path configDir{};

    const char* baseDir = std::getenv("LOCALAPPDATA");

    if (!baseDir) {
        Vin::Logger::Err("Can't get config path for the editor");
        return Vin::String{};
    }

    configDir = baseDir;
    configDir /= "vin";

    if (!std::filesystem::exists(configDir))
        std::filesystem::create_directory(configDir);

    return PATH_TO_STRING(configDir);

#elif VIN_LINUX
    std::filesystem::path configDir{};

    const char* baseDir = std::getenv("XDG_CONFIG_HOME");
    if (!baseDir) {
        baseDir = std::getenv("HOME");
        if (!baseDir) {
            Vin::Logger::Err("Can't get config path for the editor.");
            return Vin::String{};
        }
        configDir = baseDir;
        configDir /= ".config";

        if (!std::filesystem::exists(configDir))
            std::filesystem::create_directory(configDir);
    } else {
        configDir = baseDir;
    }

    configDir /= "vin";
    if (!std::filesystem::exists(configDir))
        std::filesystem::create_directory(configDir);

    return PATH_TO_STRING(configDir);
#endif
}