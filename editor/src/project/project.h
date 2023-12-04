#ifndef VIN_EDITOR_PROJECT_H
#define VIN_EDITOR_PROJECT_H

#include <vin/vin.h>
#include "assetregistry.h"

class Project {
public:
    Project() = delete;
    explicit Project(Vin::StringView workingDir);

    Vin::StringView GetWorkingDirectory();
    Vin::Ref<AssetRegistry> GetAssetRegistry();

    void SaveProject();
    void LoadProject();

private:
    Vin::String workingDirectory{};
    std::filesystem::path projectFilePath{};
    Vin::Ref<AssetRegistry> assetRegistry{};
};

#endif //VIN_EDITOR_PROJECT_H
