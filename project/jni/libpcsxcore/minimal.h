#ifndef __ROOT_MINIMAL_H__
#define __ROOT_MINIMAL_H__

#ifdef ANDROID
// Include path issues
#include "android/minimal.h"
#elif defined(IPHONE)
#include "iphone/minimal.h"
#else
#include "minimal.h"
#endif

#endif