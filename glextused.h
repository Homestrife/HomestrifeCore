//this was all pretty much taken from http://rainwarrior.ca/dragon/sdl_glsl.html

#ifndef __GLEXTUSED_H_
#define __GLEXTUSED_H_

#define NO_SDL_GLEXT
#include <SDL.h>
#include <SDL_opengl.h>

extern GLenum shader_vert;
extern GLenum shader_frag;
extern GLenum shader_prog;

extern bool shading_enabled;
extern bool buffer_objects_enabled;

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
	extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB;
	extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB;
	extern PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
	extern PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB;
	extern PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
	extern PFNGLSHADERSOURCEARBPROC				glShaderSourceARB;
	extern PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
	extern PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB;
	extern PFNGLATTACHOBJECTARBPROC				glAttachObjectARB;
	extern PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
	extern PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
	extern PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgramObjectARB;
	extern PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
	extern PFNGLUNIFORM1FARBPROC				glUniform1fARB;
	extern PFNGLUNIFORM1IARBPROC				glUniform1iARB;
	extern PFNGLUNIFORM1FVARBPROC				glUniform1fvARB;
	extern PFNGLUNIFORM1IVARBPROC				glUniform1ivARB;
	extern PFNGLGETSHADERIVPROC					glGetShaderiv;
	extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;

	/* buffer object prototypes */
	extern PFNGLBINDBUFFERARBPROC				glBindBuffer;
	extern PFNGLDELETEBUFFERSARBPROC			glDeleteBuffers;
	extern PFNGLGENBUFFERSARBPROC				glGenBuffers;
	extern PFNGLISBUFFERARBPROC					glIsBuffer;
	extern PFNGLBUFFERDATAARBPROC				glBufferData;
	extern PFNGLBUFFERSUBDATAARBPROC			glBufferSubData;
	extern PFNGLGETBUFFERSUBDATAARBPROC			glGetBufferSubData;
	extern PFNGLMAPBUFFERARBPROC				glMapBuffer;
	extern PFNGLUNMAPBUFFERARBPROC				glUnmapBuffer;
	extern PFNGLGETBUFFERPARAMETERIVARBPROC		glGetBufferParameteriv;
	extern PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointerv;
#endif

#endif