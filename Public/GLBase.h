#pragma once

// We use this so we can include any platform specific OpenGL headers, eg
// OpenGL ES vs OpenGL

// Desktop platforms
// We do our own wrangling for DSA to get an effecient wrapper
#include <GL/glew.h>

// GLSUPPORTS wraps wrangler functions to check support for extensions
#define GLSUPPORTS(x) (glewIsSupported(#x) == GL_TRUE)