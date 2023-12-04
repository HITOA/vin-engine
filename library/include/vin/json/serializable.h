#ifndef VIN_ENGINE_SERIALIZABLE_H
#define VIN_ENGINE_SERIALIZABLE_H

#include <vin/json/json.hpp>

namespace Vin {

    struct Serializable {
        virtual ~Serializable() = default;

        [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
        virtual void Deserialize(const nlohmann::json& data) = 0;
    };

}

#endif //VIN_ENGINE_SERIALIZABLE_H
