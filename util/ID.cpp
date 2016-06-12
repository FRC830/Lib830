#include "ID.h"

std::string GetProjectName(std::string default_) {
#ifdef CMAKE_PROJECT_NAME
    (void)default_;
    return CMAKE_PROJECT_NAME;
#else
    return default_;
#endif
}
