#ifndef VIN_ENGINE_RENDERPIPELINE_H
#define VIN_ENGINE_RENDERPIPELINE_H

#include <vin/scene/scene.h>

namespace Vin::Modules {
    struct RenderContext {
        Vector<Ref<Scene>, Core::AllocationStrategy::DoubleFrame>& scene;
    };

    class RenderPipeline {
    public:
        virtual void Render(RenderContext& context) = 0;
    };
}

#endif //VIN_ENGINE_RENDERPIPELINE_H
