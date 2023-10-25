#ifndef VIN_ENGINE_PROJECT_H
#define VIN_ENGINE_PROJECT_H

#include <vin/vin.h>
#include <fstream>

class Project {
public:
    Project() = delete;
    explicit Project(Vin::StringView path);
    ~Project();

    void LoadProjectFile();
    void SaveProjectFile();

    void ImportAsset(Vin::StringView path, Vin::AssetType type);
    void UnimportAsset(Vin::StringView path);

private:
    Vin::String name{ "New Project" };

    Vin::UnorderedMap<Vin::String, Vin::AssetType> importedAssets{};

    Vin::String projectFilePath{};
};

#endif //VIN_ENGINE_PROJECT_H
