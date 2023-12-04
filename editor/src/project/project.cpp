#include "project.h"
#include <filesystem>
#include <fstream>

Project::Project(Vin::StringView workingDir) : workingDirectory{ workingDir } {
    if (!std::filesystem::exists(workingDir)) {
        Vin::Logger::Err("Working directory does not exist");
        return;
    }

    projectFilePath = workingDir;
    projectFilePath /= "project.vin";

    assetRegistry = Vin::MakeRef<AssetRegistry>();

    LoadProject();
}

Vin::StringView Project::GetWorkingDirectory() {
    return workingDirectory;
}

Vin::Ref<AssetRegistry> Project::GetAssetRegistry() {
    return assetRegistry;
}

void Project::SaveProject() {
    std::fstream projectFile{ projectFilePath, std::fstream::out | std::fstream::trunc | std::fstream::binary };
    nlohmann::json serializedProjectData{};
    serializedProjectData["Assets"] = assetRegistry->Serialize();
    std::string jsonData = serializedProjectData.dump(4);
    projectFile.write(jsonData.data(), jsonData.size());
    projectFile.close();
}

void Project::LoadProject() {
    std::fstream projectFile{ projectFilePath, std::fstream::in };
    nlohmann::json serializedProjectData = nlohmann::json::parse(projectFile);
    projectFile.close();

    if (serializedProjectData.count("Assets"))
        assetRegistry->Deserialize(serializedProjectData["Assets"]);
}