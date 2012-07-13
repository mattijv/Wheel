#ifndef WHEEL_PLATFORM_GL_HEAD
#define WHEEL_PLATFORM_GL_HEAD
/*
#ifdef __APPLE__
#elif defined _WIN32
#elif defined __linux__
#else
#error Couldn't detect OS
#endif
*/

#ifdef __APPLE__
#include <OpenGL/glew.h>
#include <OpenGL/glfw3.h>
#else
#include <GL/glew.h>
#include <GL/glfw3.h>
#endif

#endif
