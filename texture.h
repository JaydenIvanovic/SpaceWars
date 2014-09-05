#ifndef TEXTURE
#define TEXTURE

#ifdef __APPLE__
#  include <Carbon/Carbon.h>
#  include <QuickTime/Movies.h>
#  include <QuickTime/QuickTimeComponents.h>
#  include <OpenGL/gl.h>
#  if TARGET_RT_BIG_ENDIAN
#    define ARGB_TYPE GL_UNSIGNED_INT_8_8_8_8_REV
#  elif TARGET_RT_LITTLE_ENDIAN
#    define ARGB_TYPE GL_UNSIGNED_INT_8_8_8_8
#  else
#    error Target endianness not specified, please use Apple gcc.
#  endif
#else
#  ifdef WIN32
#    ifndef WIN32_LEAN_AND_MEAN
#      define WIN32_LEAN_AND_MEAN
#    endif
#    include <windows.h>
#  endif
#  include <gdk-pixbuf/gdk-pixbuf.h>
#  include <GL/gl.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char *data;
	unsigned int width, height;
	GLenum internalFormat, format, type;
} I3DImage;

void freeImage(I3DImage *image);
bool isPowerOf2(const unsigned int val);
void flipData(unsigned char *data, const unsigned int pitch, const unsigned int height);
I3DImage* loadImage(char *filename);
GLuint loadTexture(I3DImage *image);
#ifdef __cplusplus
}
#endif

#endif
