#ifndef VIN_ENGINE_EDITOR_H
#define VIN_ENGINE_EDITOR_H

#include <vin/vin.h>
#include <bgfx/bgfx.h>

#define EDITOR_GUI_VIEW_ID 200

class EditorWindow {

};

class EditorModule : public Vin::Module {
public:
    Vin::DependencyList<Vin::Modules::WindowModule, Vin::Modules::RenderingModule> dependencies{ windowModule, renderingModule };

    void Initialize() final;
    void Uninitialize() final;

    void Update(Vin::TimeStep) final;

    void DrawDockSpace();

    void InitImguiWithBgfx();
    void ShutdownImguiWithBgfx();

    void Begin();
    void End();

    const unsigned char* GetFSOcornut();
    const unsigned char* GetVSOcornut();
    unsigned int GetFSOcornutLength();
    unsigned int GetVSOcornutLength();

private:
    Vin::Ref<Vin::Modules::WindowModule> windowModule{};
    Vin::Ref<Vin::Modules::RenderingModule> renderingModule{};

    bgfx::VertexLayout  imguiVertexLayout{};
    bgfx::TextureHandle imguiFontTexture{};
    bgfx::UniformHandle imguiFontUniform{};
    bgfx::ProgramHandle imguiProgram{};
};

#endif //VIN_ENGINE_EDITOR_H
