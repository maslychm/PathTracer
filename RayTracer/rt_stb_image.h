#ifndef RT_STB_IMAGE_H
#define RT_STB_IMAGE_H

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
	// microsoft compiler
	# pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

// Restore warning levels
#ifdef _MSC_VER
	// microsoft compiler
	#pragma warning (pop)
#endif

#endif // !RT_STB_IMAGE_H
