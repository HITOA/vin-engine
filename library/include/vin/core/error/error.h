#ifndef VIN_ENGINE_ERROR_H
#define VIN_ENGINE_ERROR_H

#include <vin/core/logger/logger.h>

#ifdef VIN_BUILD_NDEBUG
#define ASSERT(condition, msg) (condition);
#else
#define ASSERT(condition, msg) if (!(condition)) {    \
           Vin::Core::Logger::Logger::Err("Assertion failed in ", __FILE__, ":", __LINE__, " in function \"", __FUNCTION__, "\" : ", #msg, ", ", #condition);  \
           abort();                                   \
       }
#endif

#endif //VIN_ENGINE_ERROR_H
