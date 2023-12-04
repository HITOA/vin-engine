#include "editorfilewatcher.h"
#include <filesystem>
#include "editor.h"

void EditorFileWatcher::handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename,
                                         efsw::Action action, std::string oldFilename) {

    /*std::filesystem::path fullPath{ dir };
    fullPath = std::filesystem::weakly_canonical(fullPath / filename);
    std::filesystem::path relPath{ std::filesystem::relative(fullPath, editor->GetWorkingDirectory()) };

    if (!editor->CanFileBeAsset(PATH_TO_STRING(fullPath.extension())))
        return;

    switch ( action ) {
        case efsw::Actions::Add:
            if (editor->GetEditorImportSettings()->importNewFileInWorkingDir) {
                //editor->ImportAsset(PATH_TO_STRING(fullPath));
            }
            break;
        case efsw::Actions::Delete:
            if (editor->IsAssetImported(PATH_TO_STRING(relPath))) {
                editor->RemoveAssetFromProject(PATH_TO_STRING(relPath));
            }
            break;
        case efsw::Actions::Modified:
            if (editor->IsAssetImported(PATH_TO_STRING(relPath))) {
                //editor->ImportAsset(PATH_TO_STRING(fullPath));
            }
            break;
        /*case efsw::Actions::Moved:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from ("
                      << oldFilename << ")" << std::endl;
            break;
        default:
            return;
    }*/
}