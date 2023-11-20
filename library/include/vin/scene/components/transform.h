#ifndef VIN_ENGINE_TRANSFORM_H
#define VIN_ENGINE_TRANSFORM_H

#include <vin/math/glm.h>

namespace Vin {

    struct Transform {
        GLM::vec3 position{ 0.0f };
        GLM::quat rotation{ GLM::identity<GLM::quat>() };
        GLM::vec3 scale{ 1.0f };
        GLM::mat4 model{ GLM::identity<GLM::mat4>() };
        bool dirty{ false };
    };

}

#endif //VIN_ENGINE_TRANSFORM_H
