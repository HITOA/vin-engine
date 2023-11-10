#ifndef VIN_EDITOR_INSPECTOR_H
#define VIN_EDITOR_INSPECTOR_H

#include "../editor.h"
#include "../inspector/inspector.h"

class InspectorWindow : public EditorWindow {
public:
    void Initialize() final;

    void Draw(bool* open) final;

    static void SetInspector(Vin::Ref<Inspector> window);

private:
    static Vin::Ref<Inspector> inspectorWindow;
};

#endif //VIN_EDITOR_INSPECTOR_H
