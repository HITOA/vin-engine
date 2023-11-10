#ifndef VIN_EDITOR_INSPECTORCONTENT_H
#define VIN_EDITOR_INSPECTORCONTENT_H

#include "../editor.h"

class Inspector {
public:
    virtual ~Inspector() = default;

    virtual void Draw() = 0;
};

#endif //VIN_EDITOR_INSPECTOR_H
