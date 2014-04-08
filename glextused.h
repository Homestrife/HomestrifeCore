//this was all pretty much taken from http://rainwarrior.ca/dragon/sdl_glsl.html

#ifndef __GLEXTUSED_H_
#define __GLEXTUSED_H_

#define NO_SDL_GLEXT
#include <SDL.h>
#include <SDL_opengl.h>

extern GLenum shader_vert;
extern GLenum shader_fragNonIndexed;
extern GLenum shader_fragIndexed;
extern GLenum shader_progNonIndexed;
extern GLenum shader_progIndexed;

extern bool shading_enabled;
extern bool buffer_objects_enabled;
extern bool default_array_objects_enabled;
extern bool custom_array_objects_enabled;

extern void setupExtensions();
extern string loadSource(string file);
extern string compileShader(GLenum shader);
extern string linkProgram(GLenum program);

#ifdef __APPLE__
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
#else
	#include "glext.h"

	/* Shading language prototypes. */
	//extern PFNGLACTIVETEXTUREPROC				glActiveTexture; //Commented out for Linux
	//extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
	extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
	//extern PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
	extern PFNGLDELETESHADERPROC				glDeleteShader;
	extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
	extern PFNGLCREATESHADERPROC				glCreateShader;
	extern PFNGLSHADERSOURCEPROC				glShaderSource;
	extern PFNGLCOMPILESHADERPROC				glCompileShader;
	//extern PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;
	extern PFNGLATTACHSHADERPROC				glAttachShader;
	extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
	extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
	extern PFNGLLINKPROGRAMPROC					glLinkProgram;
	extern PFNGLUSEPROGRAMPROC					glUseProgram;
	extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
	extern PFNGLUNIFORM1FPROC					glUniform1f;
	extern PFNGLUNIFORM1IPROC					glUniform1i;
	//extern PFNGLUNIFORM1FVARBPROC				glUniform1fvARB;
	//extern PFNGLUNIFORM1IVARBPROC				glUniform1ivARB;
	extern PFNGLUNIFORM2FPROC					glUniform2f;
	//extern PFNGLUNIFORM4FARBPROC				glUniform4fARB;
	extern PFNGLGETSHADERIVPROC					glGetShaderiv;
	extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
	//extern PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocation;
	extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;

	/* buffer object prototypes */
	extern PFNGLBINDBUFFERPROC					glBindBuffer;
	extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
	extern PFNGLGENBUFFERSPROC					glGenBuffers;
	//extern PFNGLISBUFFERARBPROC				glIsBuffer;
	extern PFNGLBUFFERDATAPROC					glBufferData;
	//extern PFNGLBUFFERSUBDATAARBPROC			glBufferSubData;
	//extern PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubData;
	//extern PFNGLMAPBUFFERARBPROC				glMapBuffer;
	//extern PFNGLUNMAPBUFFERARBPROC			glUnmapBuffer;
	//extern PFNGLGETBUFFERPARAMETERIVARBPROC	glGetBufferParameteriv;
	//extern PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointerv;
	//extern PFNGLVERTEXATTRIB2FARBPROC			glVertexAttrib2f;
	//extern PFNGLVERTEXATTRIB3FARBPROC			glVertexAttrib3f;

	/* default array object prototypes */
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
	extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
	extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;

	/* custom array object prototypes */
	extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
	extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
	extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
#endif

#endif