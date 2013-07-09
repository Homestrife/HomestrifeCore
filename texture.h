#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <sstream>
using namespace std;
#include <stdio.h>
#include "hslog.h"
#include "glextused.h"

#define ATTRIBUTE_BITS		0x0F //used to get the first 4 bits of a byte
#define RIGHT_ALIGN			0x10 //used to check the left/right alignment of the image pixels
#define TOP_ALIGN			0x20 //used to check the top/bottom alignment of the image pixels
#define RL_PACKET			0x80 //used to see if an RLE packet is run-length or raw
#define REP_COUNT_BITS		0x7F //used to get the first 7 bits of a byte
#define TRANSPARENT_COLOR	0xFF00FF //used to check if a color is the universal transparent color

#define PALETTE_LENGTH			256
#define PALETTE_COLOR_LENGTH	3

typedef float HSVectComp; //a single component (x, y, or z) of a vector

extern GLenum texCoordBufferID;
extern GLenum elementArrayBufferID;

extern GLenum positionLocNonIndexed;
extern GLenum positionLocIndexed;
extern GLenum texCoordInLocNonIndexed;
extern GLenum texCoordInLocIndexed;

struct HSVect2D //a two-dimensional vector
{
	HSVectComp x;
	HSVectComp y;
};

struct HSPalette
{
	GLuint textureID;
	string paletteFilePath;
	int usingCount; //number of objects using this
};

struct HSTexture
{
	GLuint textureID; //OpenGL's internal texture ID
	GLuint bufferID; //OpenGL's internal vertex buffer ID
	GLuint vaoID; //OpenGL's internal vertex array object ID
	bool indexed; //whether or not the texture is indexed
	bool rightAlign; //whether or not drawing should start from the right
	bool topAlign; //whether or not drawing should start from the top
	string textureFilePath; //needed for reloading and registry handling
	int usingCount; //number of holds using this
	HSPalette * ownPalette; //the palette data contained within the tga file
};

int fseekError(GLubyte error, string texFilePath);
int freadError(FILE * file, string texFilePath);
int LoadTGAToTexture(HSTexture * hsTex, bool openGL3, bool useTGAPalette);
int LoadHSPToPalette(HSPalette * hsPal);
int StorePaletteData(HSPalette * hsPal, GLubyte * paletteData);

#endif