#ifndef VIN_ENGINE_RENDERPIPELINE_H
#define VIN_ENGINE_RENDERPIPELINE_H

namespace Vin::Modules {
    class RenderPipeline {
    public:
        virtual void Render() = 0;
    };
}

#endif //VIN_ENGINE_RENDERPIPELINE_H
