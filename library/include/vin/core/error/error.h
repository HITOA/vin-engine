#ifndef VIN_ENGINE_ERROR_H
#define VIN_ENGINE_ERROR_H

#ifdef VIN_BUILD_NDEBUG
#define ASSERT(condition, msg) (void*)0;
#else
#define ASSERT(condition, msg) if (!(condition)) { abort(); }
#endif

#endif //VIN_ENGINE_ERROR_H
