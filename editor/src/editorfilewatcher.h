#ifndef VIN_ENGINE_EDITORFILEWATCHER_H
#define VIN_ENGINE_EDITORFILEWATCHER_H

#include <vin/vin.h>
#include <efsw/efsw.hpp>
#include "config/editorimportsettings.h"

class EditorModule;

class EditorFileWatcher : public efsw::FileWatchListener {
public:
    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
                          const std::string& filename, efsw::Action action,
                          std::string oldFilename) final;

public:
    EditorModule* editor;
};

#endif //VIN_ENGINE_EDITORFILEWATCHER_H
