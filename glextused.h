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
	extern PFNGLUNIFORM2FARBPROC				glUniform2fARB;
	extern PFNGLUNIFORM4FARBPROC				glUniform4fARB;
	extern PFNGLGETSHADERIVPROC					glGetShaderiv;
	extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
	extern PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;
	extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;

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
	extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC	glEnableVertexAttribArray;
	extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArray;
	extern PFNGLVERTEXATTRIBPOINTERARBPROC		glVertexAttribPointer;
	extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
	extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
	extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
	extern PFNGLVERTEXATTRIB2FPROC				glVertexAttrib2f;
	extern PFNGLVERTEXATTRIB3FPROC				glVertexAttrib3f;
#endif

#endif