// Li Ji
// April 22, 2009
// GLUT+OpenGL on both linux and windows platforms
#ifndef _CALLBACK_H_
#define _CALLBACK_H_
#include "surface.h"
#ifdef _WIN32
#ifndef _STDCALL_SUPPORTED
#define _STDCALL_SUPPORTED
#endif // _STDCALL_SUPPORTED
#ifndef _M_IX86
#define _M_IX86
#endif // _M_IX86
// #include <windows.h>
#endif // _WIN32
#include <cstdlib> 
//#include <GL/glut.h>

namespace Callback
{
void display_surface();
void display_contour();
void display_status();

void reshape_surface(int width, int height);
void reshape_contour(int width, int height);
void reshape_status(int width, int height);

void keyboard_surface(unsigned char key, int /*mouse_x*/, int /*mouse_y*/);
void keyboard_contour(unsigned char key, int /*mouse_x*/, int /*mouse_y*/);
void special_keyboard(int key, int /*mouse_x*/, int /*mouse_y*/);

void mouse_motion(int /*x*/, int y);
void mouse_contour(int button, int state, int mouse_x, int mouse_y);

void menu_surface(int value);

void init();
}
#endif // _CALLBACK_H_
