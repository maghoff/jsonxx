#include "platform.hpp"


// Sensible fallback if JSONXX_LINK_VARIANT is not defined:
#ifndef JSONXX_LINK_VARIANT
#define JSONXX_LINK_VARIANT 3
#endif


#if JSONXX_LINK_VARIANT == 1

#define JSONXX_DECLSPEC JSONXX_IMPORT_DECLSPEC

#elif JSONXX_LINK_VARIANT == 2

#define JSONXX_DECLSPEC JSONXX_EXPORT_DECLSPEC

#elif JSONXX_LINK_VARIANT == 3

#define JSONXX_DECLSPEC

#else

#error "JSONXX_LINK_VARIANT must be 1 for shared import, 2 for shared export or 3 for static"

#endif
