#ifndef VIN_ENGINE_INSPECTORCONTENT_H
#define VIN_ENGINE_INSPECTORCONTENT_H

#include "../editor.h"

class Inspector {
public:
    virtual ~Inspector() = default;

    virtual void Draw() = 0;
};

#endif //VIN_ENGINE_INSPECTOR_H
