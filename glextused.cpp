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
bool default_array_objects_enabled = false;
bool custom_array_objects_enabled = false;

#ifdef __WIN32__
	#define WIN32_OR_X11
#elif __APPLE__
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	void setupExtensions()
	{ shading_enabled = true; buffer_objects_enabled = true; default_array_objects_enabled = true; custom_array_objects_enabled = true; }; // OS X already has these extensions
#elif __X11__
	#include <GL/glx.h>
	#include <GL/glxext.h>
	#define WIN32_OR_X11
#else
	void setupExtensions()
	{ shading_enabled = false; buffer_objects_enabled = false; default_array_objects_enabled = false; custom_array_objects_enabled = false; }; // just fail otherwise?
#endif

#ifdef WIN32_OR_X11
PFNGLACTIVETEXTUREPROC				glActiveTexture = NULL;
//PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB = NULL;
PFNGLCREATEPROGRAMPROC				glCreateProgram = NULL;
PFNGLDELETESHADERPROC				glDeleteShader = NULL;
PFNGLDELETEPROGRAMPROC				glDeleteProgram = NULL;
PFNGLCREATESHADERPROC				glCreateShader = NULL;
PFNGLSHADERSOURCEPROC				glShaderSource = NULL;
PFNGLCOMPILESHADERPROC				glCompileShader = NULL;
//PFNGLGETOBJECTPARAMETERIVARBPROC    glGetObjectParameterivARB = NULL;
PFNGLATTACHSHADERPROC				glAttachShader = NULL;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog = NULL;
PFNGLLINKPROGRAMPROC				glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC					glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC					glUniform1f = NULL;
PFNGLUNIFORM1IPROC					glUniform1i = NULL;
//PFNGLUNIFORM1FVARBPROC              glUniform1fvARB = NULL;
//PFNGLUNIFORM1IVARBPROC              glUniform1ivARB = NULL;
PFNGLUNIFORM2FPROC					glUniform2f = NULL;
//PFNGLUNIFORM4FARBPROC				glUniform4fARB = NULL;
PFNGLGETSHADERIVPROC				glGetShaderiv = NULL;
PFNGLGETPROGRAMIVPROC				glGetProgramiv = NULL;
//PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation = NULL;

PFNGLBINDBUFFERPROC					glBindBuffer = NULL;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers = NULL;
PFNGLGENBUFFERSARBPROC				glGenBuffers = NULL;
//PFNGLISBUFFERARBPROC				glIsBuffer = NULL;
PFNGLBUFFERDATAPROC					glBufferData = NULL;
//PFNGLBUFFERSUBDATAARBPROC			glBufferSubData = NULL;
//PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubData = NULL;
//PFNGLMAPBUFFERARBPROC				glMapBuffer = NULL;
//PFNGLUNMAPBUFFERARBPROC				glUnmapBuffer = NULL;
//PFNGLGETBUFFERPARAMETERIVARBPROC	glGetBufferParameteriv = NULL;
//PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointerv = NULL;
//PFNGLVERTEXATTRIB2FARBPROC		glVertexAttrib2f = NULL;
//PFNGLVERTEXATTRIB3FARBPROC		glVertexAttrib3f = NULL;

PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer = NULL;

PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC			glBindVertexArray = NULL;

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
  /*if( findString("GL_ARB_shader_objects",extensionList) &&
      findString("GL_ARB_shading_language_100",extensionList) &&
      findString("GL_ARB_vertex_shader",extensionList) &&
      findString("GL_ARB_fragment_shader",extensionList) )
  {*/
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)
      SDL_GL_GetProcAddress("glActiveTexture");
    //glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)
    //  SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)
      SDL_GL_GetProcAddress("glCreateProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)
      SDL_GL_GetProcAddress("glDeleteShader");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)
      SDL_GL_GetProcAddress("glDeleteProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)
     SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)
      SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)
      SDL_GL_GetProcAddress("glCompileShader");
    //glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)
    //  SDL_GL_GetProcAddress("glGetObjectParameterivARB");
    glAttachShader = (PFNGLATTACHSHADERPROC)
      SDL_GL_GetProcAddress("glAttachShader");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
      SDL_GL_GetProcAddress("glGetShaderInfoLog");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
      SDL_GL_GetProcAddress("glGetProgramInfoLog");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)
      SDL_GL_GetProcAddress("glLinkProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)
      SDL_GL_GetProcAddress("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)
      SDL_GL_GetProcAddress("glGetUniformLocation");
    glUniform1f = (PFNGLUNIFORM1FPROC)
      SDL_GL_GetProcAddress("glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)
      SDL_GL_GetProcAddress("glUniform1i");
    //glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)
    //  SDL_GL_GetProcAddress("glUniform1fvARB");
    //glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC)
    //  SDL_GL_GetProcAddress("glUniform1ivARB");
    glUniform2f = (PFNGLUNIFORM2FPROC)
      SDL_GL_GetProcAddress("glUniform2f");
    //glUniform4fARB = (PFNGLUNIFORM4FARBPROC)
    //  SDL_GL_GetProcAddress("glUniform4fARB");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)
      SDL_GL_GetProcAddress("glGetShaderiv");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)
      SDL_GL_GetProcAddress("glGetProgramiv");
	//glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONARBPROC)
	//  SDL_GL_GetProcAddress("glBindAttribLocationARB");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)
	  SDL_GL_GetProcAddress("glGetAttribLocation");

    if( false
     || glActiveTexture == NULL
     //|| glMultiTexCoord2fARB == NULL
     || glCreateProgram == NULL
     || glDeleteShader == NULL
     || glDeleteProgram == NULL
     || glCreateShader == NULL
     || glShaderSource == NULL
     || glCompileShader == NULL
     //|| glGetObjectParameterivARB == NULL
     || glAttachShader == NULL
     || glGetShaderInfoLog == NULL
     || glGetProgramInfoLog == NULL
     || glLinkProgram == NULL
     || glUseProgram == NULL
     || glGetUniformLocation == NULL
     || glUniform1f == NULL
     || glUniform1i == NULL
     //|| glUniform1fvARB == NULL
     //|| glUniform1ivARB == NULL
	 || glUniform2f == NULL
	 //|| glUniform4fARB == NULL
	 || glGetShaderiv == NULL
	 //|| glBindAttribLocation == NULL
	 || glGetAttribLocation == NULL)
	{
      shading_enabled = false;
	}
    else 
	{
		shading_enabled = true;
	}
  /*}
  else
  {
    shading_enabled = false;
  }*/

  //get the vertex buffer object extension functions
  /*if( findString("GL_ARB_vertex_buffer_object",extensionList) )
  {*/
    glBindBuffer = (PFNGLBINDBUFFERPROC)
      SDL_GL_GetProcAddress("glBindBuffer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)
      SDL_GL_GetProcAddress("glDeleteBuffers");
    glGenBuffers = (PFNGLGENBUFFERSPROC)
      SDL_GL_GetProcAddress("glGenBuffers");
    //glIsBuffer = (PFNGLISBUFFERARBPROC)
    //  SDL_GL_GetProcAddress("glIsBufferARB");
    glBufferData = (PFNGLBUFFERDATAPROC)
      SDL_GL_GetProcAddress("glBufferData");
    //glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)
    //  SDL_GL_GetProcAddress("glBufferSubDataARB");
    //glGetBufferSubData = (PFNGLGETBUFFERSUBDATAARBPROC)
    //  SDL_GL_GetProcAddress("glGetBufferSubDataARB");
    //glMapBuffer = (PFNGLMAPBUFFERARBPROC)
    //  SDL_GL_GetProcAddress("glMapBufferARB");
    //glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)
    //  SDL_GL_GetProcAddress("glUnmapBufferARB");
    //glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVARBPROC)
    //  SDL_GL_GetProcAddress("glGetBufferParameterivARB");
    //glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVARBPROC)
    //  SDL_GL_GetProcAddress("glGetBufferPointervARB");
	//glVertexAttrib2f = (PFNGLVERTEXATTRIB2FARBPROC)
	//  SDL_GL_GetProcAddress("glVertexAttrib2fARB");
	//glVertexAttrib3f = (PFNGLVERTEXATTRIB3FARBPROC)
	//  SDL_GL_GetProcAddress("glVertexAttrib3fARB");

    if( false
     || glBindBuffer == NULL
     || glDeleteBuffers == NULL
     || glGenBuffers == NULL
     //|| glIsBuffer == NULL
     || glBufferData == NULL)
     //|| glBufferSubData == NULL
     //|| glGetBufferSubData == NULL
     //|| glMapBuffer == NULL
     //|| glUnmapBuffer == NULL
     //|| glGetBufferParameteriv == NULL
     //|| glGetBufferPointerv == NULL
	 //|| glVertexAttrib2f == NULL
	 //|| glVertexAttrib3f == NULL
	{
      buffer_objects_enabled = false;
	}
    else 
	{
	  buffer_objects_enabled = true;
	}
  /*}
  else
  {
    buffer_objects_enabled = false;
  }*/

  //get the default vertex array object extension functions
  /*if( findString("GL_ARB_vertex_program",extensionList) )
  {*/
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
	  SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)
	  SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)
	  SDL_GL_GetProcAddress("glVertexAttribPointer");

	  if( false
		 || glEnableVertexAttribArray == NULL
		 || glDisableVertexAttribArray == NULL
		 || glVertexAttribPointer == NULL)
	  {
		  default_array_objects_enabled = false;
	  }
	  else
	  {
		  default_array_objects_enabled = true;
	  }
  /*}
  else
  {
	  default_array_objects_enabled = false;
  }*/

  //get the custom vertex array object extension functions
  if( findString("GL_ARB_vertex_array_object",extensionList) )
  {
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)
	  SDL_GL_GetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)
	  SDL_GL_GetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)
	  SDL_GL_GetProcAddress("glBindVertexArray");

	if( false
	 || glGenVertexArrays == NULL
	 || glBindVertexArray == NULL
	 || glDeleteVertexArrays == NULL)
	{
		custom_array_objects_enabled = false;
	}
	else
	{
		custom_array_objects_enabled = true;
	}
  }
  else
  {
	  custom_array_objects_enabled = false;
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
	glCompileShader(shader);

	char infobuffer[1000];
	int infobufferlen = 0;
	glGetShaderInfoLog(shader, 999, &infobufferlen, infobuffer);
	infobuffer[infobufferlen] = 0;

	string info = infobuffer;

	return info;
}

string linkProgram(GLenum program)
{
	glLinkProgram(program);

	char infobuffer[1000];
	int infobufferlen = 0;
	glGetProgramInfoLog(program, 999, &infobufferlen, infobuffer);
	infobuffer[infobufferlen] = 0;

	string info = infobuffer;

	return info;
}