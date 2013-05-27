//this was all pretty much taken from http://rainwarrior.thenoos.net/dragon/sdl_glsl.html

#define NO_SDL_GLEXT
#include <string>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glext.h"
using namespace std;

bool shading_enabled = false;
bool buffer_objects_enabled = false;

#ifdef __WIN32__
	#define WIN32_OR_X11
#elif __APPLE__
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	void setupExtensions()
	{ shading_enabled = true; buffer_objects_enabled = true; }; // OS X already has these extensions
#elif __X11__
	#include <GL/glx.h>
	#include <GL/glxext.h>
	#define WIN32_OR_X11
#else
	void setupExtensions()
	{ shading_enabled = false; buffer_objects_enabled = false; }; // just fail otherwise?
#endif

#ifdef WIN32_OR_X11
PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC     glCreateProgramObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC            glDeleteObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC      glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC            glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC           glCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC    glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC            glAttachObjectARB = NULL;
PFNGLGETINFOLOGARBPROC              glGetInfoLogARB = NULL;
PFNGLLINKPROGRAMARBPROC             glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC        glUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC      glGetUniformLocationARB = NULL;
PFNGLUNIFORM1FARBPROC               glUniform1fARB = NULL;
PFNGLUNIFORM1IARBPROC               glUniform1iARB = NULL;
PFNGLUNIFORM1FVARBPROC              glUniform1fvARB = NULL;
PFNGLUNIFORM1IVARBPROC              glUniform1ivARB = NULL;
PFNGLGETSHADERIVPROC				glGetShaderiv = NULL;
PFNGLGETPROGRAMIVPROC				glGetProgramiv = NULL;

PFNGLBINDBUFFERARBPROC				glBindBuffer = NULL;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffers = NULL;
PFNGLGENBUFFERSARBPROC				glGenBuffers = NULL;
PFNGLISBUFFERARBPROC				glIsBuffer = NULL;
PFNGLBUFFERDATAARBPROC				glBufferData = NULL;
PFNGLBUFFERSUBDATAARBPROC			glBufferSubData = NULL;
PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubData = NULL;
PFNGLMAPBUFFERARBPROC				glMapBuffer = NULL;
PFNGLUNMAPBUFFERARBPROC				glUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC	glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointerv = NULL;

#include <cstring>

static bool findString(char* in, char* list)
{
  int thisLength = strlen(in);
  while (*list != 0)
  {
    int length = strcspn(list," ");
    
    if( thisLength == length )
      if (!strncmp(in,list,length))
        return true;
        
    list += length + 1;
  }
  return false;
}

void setupExtensions()
{
  char* extensionList = (char*)glGetString(GL_EXTENSIONS);

  //get the shader extension functions
  if( findString("GL_ARB_shader_objects",extensionList) &&
      findString("GL_ARB_shading_language_100",extensionList) &&
      findString("GL_ARB_vertex_shader",extensionList) &&
      findString("GL_ARB_fragment_shader",extensionList) )
  {
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
      SDL_GL_GetProcAddress("glActiveTextureARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)
      SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
    glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)
      SDL_GL_GetProcAddress("glCreateProgramObjectARB");
    glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)
      SDL_GL_GetProcAddress("glDeleteObjectARB");
    glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)
     SDL_GL_GetProcAddress("glCreateShaderObjectARB");
    glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)
      SDL_GL_GetProcAddress("glShaderSourceARB");
    glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)
      SDL_GL_GetProcAddress("glCompileShaderARB");
    glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)
      SDL_GL_GetProcAddress("glGetObjectParameterivARB");
    glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)
      SDL_GL_GetProcAddress("glAttachObjectARB");
    glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)
      SDL_GL_GetProcAddress("glGetInfoLogARB");
    glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)
      SDL_GL_GetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)
      SDL_GL_GetProcAddress("glUseProgramObjectARB");
    glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)
      SDL_GL_GetProcAddress("glGetUniformLocationARB");
    glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
      SDL_GL_GetProcAddress("glUniform1fARB");
    glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
      SDL_GL_GetProcAddress("glUniform1iARB");
    glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)
      SDL_GL_GetProcAddress("glUniform1fvARB");
    glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC)
      SDL_GL_GetProcAddress("glUniform1ivARB");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)
      SDL_GL_GetProcAddress("glGetShaderiv");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)
      SDL_GL_GetProcAddress("glGetProgramiv");

    if( false
     || glActiveTextureARB == NULL
     || glMultiTexCoord2fARB == NULL
     || glCreateProgramObjectARB == NULL
     || glDeleteObjectARB == NULL
     || glCreateShaderObjectARB == NULL
     || glShaderSourceARB == NULL
     || glCompileShaderARB == NULL
     || glGetObjectParameterivARB == NULL
     || glAttachObjectARB == NULL
     || glGetInfoLogARB == NULL
     || glLinkProgramARB == NULL
     || glUseProgramObjectARB == NULL
     || glGetUniformLocationARB == NULL
     || glUniform1fARB == NULL
     || glUniform1iARB == NULL
     || glUniform1fvARB == NULL
     || glUniform1ivARB == NULL
	 || glGetShaderiv == NULL)
	{
      shading_enabled = false;
	}
    else 
	{
		shading_enabled = true;
	}
  }
  else
  {
    shading_enabled = false;
  }

  //get the vertex buffer object extension functions
  if( findString("GL_ARB_vertex_buffer_object",extensionList) )
  {
    glBindBuffer = (PFNGLBINDBUFFERARBPROC)
      SDL_GL_GetProcAddress("glBindBufferARB");
    glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)
      SDL_GL_GetProcAddress("glDeleteBuffersARB");
    glGenBuffers = (PFNGLGENBUFFERSARBPROC)
      SDL_GL_GetProcAddress("glGenBuffersARB");
    glIsBuffer = (PFNGLISBUFFERARBPROC)
      SDL_GL_GetProcAddress("glIsBufferARB");
    glBufferData = (PFNGLBUFFERDATAARBPROC)
      SDL_GL_GetProcAddress("glBufferDataARB");
    glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)
      SDL_GL_GetProcAddress("glBufferSubDataARB");
    glGetBufferSubData = (PFNGLGETBUFFERSUBDATAARBPROC)
      SDL_GL_GetProcAddress("glGetBufferSubDataARB");
    glMapBuffer = (PFNGLMAPBUFFERARBPROC)
      SDL_GL_GetProcAddress("glMapBufferARB");
    glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)
      SDL_GL_GetProcAddress("glUnmapBufferARB");
    glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVARBPROC)
      SDL_GL_GetProcAddress("glGetBufferParameterivARB");
    glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVARBPROC)
      SDL_GL_GetProcAddress("glGetBufferPointervARB");

    if( false
     || glBindBuffer == NULL
     || glDeleteBuffers == NULL
     || glGenBuffers == NULL
     || glIsBuffer == NULL
     || glBufferData == NULL
     || glBufferSubData == NULL
     || glGetBufferSubData == NULL
     || glMapBuffer == NULL
     || glUnmapBuffer == NULL
     || glGetBufferParameteriv == NULL
     || glGetBufferPointerv == NULL)
	{
      buffer_objects_enabled = false;
	}
    else 
	{
	  buffer_objects_enabled = true;
	}
  }
  else
  {
    buffer_objects_enabled = false;
  }
}
#endif

string loadSource(string filePath)
{
	string source;
	string line;

	ifstream file(filePath);
	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);
			source.append(line);
		}
		file.close();
	}
	else
	{
		return "READ ERROR";
	}

	return source;
}

string compileShader(GLenum shader)
{
	glCompileShaderARB(shader);

	char infobuffer[1000];
	int infobufferlen = 0;
	glGetInfoLogARB(shader, 999, &infobufferlen, infobuffer);
	infobuffer[infobufferlen] = 0;

	string info = infobuffer;

	return info;
}

string linkProgram(GLenum program)
{
	glLinkProgramARB(program);

	char infobuffer[1000];
	int infobufferlen = 0;
	glGetInfoLogARB(program, 999, &infobufferlen, infobuffer);
	infobuffer[infobufferlen] = 0;

	string info = infobuffer;

	return info;
}