#include "texture.h"

int fseekError(GLubyte error, string texFilePath)
{
	stringstream sstm;
	sstm << "TGA file seek error. Code: " << error << " File: " << texFilePath;
	UpdateLog(sstm.str(), true);

	return -1;
}

int freadError(FILE * file, string texFilePath)
{
	if(GLubyte error = ferror(file) != 0)
	{
		stringstream sstm;
		sstm << "TGA file read error. Code: " << error << " File: " << texFilePath;
		UpdateLog(sstm.str(), true);
	}
	if(GLubyte error = feof(file) != 0)
	{
		stringstream sstm;
		sstm << "TGA file end-of-file error. Code: " << error << " File: " << texFilePath;
		UpdateLog(sstm.str(), true);
	}
	return -1;
}

int LoadTGAToTexture(HSTexture * hsTex, bool openGL3, bool useTGAPalette)
{
	if(hsTex == NULL)
	{
		UpdateLog("HSTexture object is null.", true);
		return -1; //given texture is null
	}

	//the data that OpenGL ultimately needs
	GLushort imageWidth = 0;
	GLushort imageHeight = 0;
	GLubyte * imageData;
	GLuint textureID;
	GLuint bufferID;
	
	//variables to help us get the above data
	FILE * file;
	bool opaque = true;
	GLubyte imageIDLength;
	GLubyte colorMapType;
	GLubyte imageType;
	GLushort colorMapLength;
	GLubyte colorMapEntrySize;
	GLubyte pixelDepth;
	GLubyte bytesPerPixel;
	GLubyte colorBytesPerPixel = 3; //BGR
	GLubyte bytesPerColorMapEntry;
	GLubyte GLbytesPerPixel;
	GLubyte imageDescriptor;
	bool topAlign = true;
	bool rightAlign = true;

	//open the file
	string texFilePath = hsTex->textureFilePath;
	if(GLuint error = fopen_s(&file, texFilePath.data(), "rb") != 0)
	{
		UpdateLog("Could not open texture file: " + texFilePath, true);
		return error; //couldn't open the file
	}

	//gather all the general info about the TGA file
	if(GLubyte error = fseek(file, 0L, SEEK_SET) != 0) {return fseekError(error, texFilePath);} //move to the beginning of the file
	if(fread(&imageIDLength, 1, 1, file) != 1) {return freadError(file, texFilePath);} //get the length of the image id field
	if(fread(&colorMapType, 1, 1, file) != 1) {return freadError(file, texFilePath);} //see if the file has a color map in it or not
	if(fread(&imageType, 1, 1, file) != 1) {return freadError(file, texFilePath);} //find out what kind of image file this is
	if(imageType != 9 && imageType != 10)
	{
		UpdateLog("TGA file is not run-lengh encoded: " + texFilePath, true);
		return -1; //this needs to be RLE, either indexed or truecolor
	}
	if(GLubyte error = fseek(file, 2L, SEEK_CUR) != 0) {return fseekError(error, texFilePath);} //skip the first two bytes of the color map specification
	if(fread(&colorMapLength, 2, 1, file) != 1) {return freadError(file, texFilePath);} //get the length of the color map
	if(fread(&colorMapEntrySize, 1, 1, file) != 1) {return freadError(file, texFilePath);} //get the size of each color map entry
	if(colorMapEntrySize == 15)
	{
		colorMapEntrySize = 16; //make sure it's a multiple of 8
	}
	bytesPerColorMapEntry = colorMapEntrySize/8; //get the size of each color map entry in bytes
	if(GLubyte error = fseek(file, 4L, SEEK_CUR) != 0) {return fseekError(error, texFilePath);} //skip the first four bytes of the image specification
	if(fread((void *)&imageWidth, 2, 1, file) != 1) {return freadError(file, texFilePath);} //get the image width
	if(fread((void *)&imageHeight, 2, 1, file) != 1) {return freadError(file, texFilePath);} //get the image height
	if(fread(&pixelDepth, 1, 1, file) != 1) {return freadError(file, texFilePath);} //get the size of each pixel in bits
	bytesPerPixel = pixelDepth/8; //get the size of each pixel in bytes
	if((imageType == 9 && pixelDepth != 8) || (imageType == 10 && pixelDepth != 24))
	{
		UpdateLog("TGA must be indexed 8-bit or truecolor 24-bit: " + texFilePath, true);
		return -1; //only allowed configurations are: indexed with 8-bit, or truecolor with 24 bit
	}
	if(fread(&imageDescriptor, 1, 1, file) != 1) {return freadError(file, texFilePath);} //get the image descriptor, and pull some data from it
	if((imageDescriptor & ATTRIBUTE_BITS) > 0)
	{
		UpdateLog("TGA must not have any attributes defined: " + texFilePath, true);
		return -1; //no attributes should be defined
	}
	if((imageDescriptor & TOP_ALIGN) == 0)
	{
		topAlign = false; //the pixels are bottom-aligned
	}
	if((imageDescriptor & RIGHT_ALIGN) == 0)
	{
		rightAlign = false; //the pixels are left-aligned
	}
	if(GLubyte error = fseek(file, (long)imageIDLength, SEEK_CUR) != 0) {return fseekError(error, texFilePath);} //skip the imageID
	if(useTGAPalette && colorMapType != 0)
	{
		//save the palette data
		HSPalette * hsPal = new HSPalette();

		if(bytesPerColorMapEntry != 3 || colorMapLength != 256)
		{
			UpdateLog("Malformed palette data in indexed TGA file: " + texFilePath, true);
			return -1;
		}

		GLubyte * paletteData = (GLubyte*)malloc(1024 * sizeof(GLubyte));

		for(int i = 0; i < 256; i++)
		{
			if(fread((void *)(paletteData + i*4), bytesPerColorMapEntry, 1, file) != 1) {return freadError(file, texFilePath);}

			*(paletteData + i*4 + 3) = 0x00;
		}

		if(StorePaletteData(hsPal, paletteData) != 0)
		{
			UpdateLog("Error reading palette data from indexed TGA file: " + texFilePath, true);
			return -1;
		}

		hsTex->ownPalette = hsPal;
	}
	else
	{
		if(GLubyte error = fseek(file, (long)(colorMapLength * bytesPerColorMapEntry), SEEK_CUR) != 0) {return fseekError(error, texFilePath);} //skip the color map
	}
	
	//okay, time for the fun part: picking through the image data.
	//it's ALWAYS going to be in RLE format so we can't just grab the raw data.
	//we need to take the compressed data and turn it into a 32bit BGRA format to pass to opengl
	GLuint maxPixels = imageWidth * imageHeight; //we need this so we know when to stop
	GLuint curPixels = 0; //keeps track of how far we've gone through the actual output buffer
	GLubyte * color;
	if(imageType == 9)
	{
		GLbytesPerPixel = 1;
		colorBytesPerPixel = 1;
	}
	else if(imageType == 10)
	{
		GLbytesPerPixel = 4;
	}
	imageData = (GLubyte*)malloc(maxPixels * GLbytesPerPixel); //set up our image data buffer
	color = (GLubyte*)malloc(colorBytesPerPixel); //create a buffer to hold color data before it gets passed to the imageData
	GLubyte repCount; //this'll hold each repetition count field
	//if(imageType == 9)
	//{
	//	//open the palette file, if this is an indexed image
	//	if(GLuint error = fopen_s(&palFile, palFilePath.data(), "rb") != 0)
	//	{
	//		UpdateLog("Could not open palette file: " + texFilePath, true);
	//		return error; //couldn't open the file
	//	}
	//}
	while(curPixels < maxPixels)
	{
		if(fread(&repCount, 1, 1, file) != 1) { return freadError(file, texFilePath);} //get the repetition count field
		if((repCount & RL_PACKET) == 0)
		{
			//this is a raw packet.
			//Just shove the pixel data into the image data buffer
			repCount = repCount & REP_COUNT_BITS; //get the actual number of following pixels
			//put the pixels into the image data buffer
			for(int i = 0; i <= repCount; i++)
			{
				//get the color data for this pixel
				if(imageType == 9)
				{
					//the data in the file represents indicies, so just pull it directly into the buffer
					if(fread(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel), bytesPerPixel, 1, file) != 1) {return freadError(file, texFilePath);}
				}
				else if(imageType == 10)
				{
					//the data in the file represents the actual color, so just pull it directly into the buffer
					if(fread(color, bytesPerPixel, 1, file) != 1) {return freadError(file, texFilePath);}
					memcpy(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel), color, colorBytesPerPixel);
					//if the color is the transparent color then make the pixel transparent
					if(*color == 255 && *(color + 1) == 0 && *(color + 2) == 255) { opaque = false; } else { opaque = true; }
					//add the alpha channel
					if(opaque)
					{
						*(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel) + 3) = 0xFF;
					}
					else
					{
						*(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel) + 3) = 0x00;
					}
				}
				//for(int j = bytesPerPixel - 1; j >= 0; j--)
				//{
				//	//gotta read each individual pixel in backwards since TGA stores color backwards
				//	if(fread(imageData + (curPixels * (bytesPerPixel + alphaBytes)) + (i * (bytesPerPixel + alphaBytes)) + j, 1, 1, file) != 1) { return -1; }
				//}
			}
		}
		else
		{
			//this is a run-length packet.
			//Put the specificed number of pixels of the specified color into the image data buffer
			repCount = repCount & REP_COUNT_BITS; //get the actual number of following pixels
			//get the color of this run
			if(imageType == 9)
			{
				//the data in the file represents indicies, so just pull it directly into the buffer
				if(fread(color, bytesPerPixel, 1, file) != 1) {return freadError(file, texFilePath);}
			}
			else if(imageType == 10)
			{
				//the data in the file represents the actual color, so just pull it directly into the buffer
				if(fread(color, bytesPerPixel, 1, file) != 1) {return freadError(file, texFilePath);}
				//if the color is the transparent color then make the pixel transparent
				if(*color == 255 && *(color + 1) == 0 && *(color + 2) == 255) { opaque = false; } else { opaque = true; }
			}
			//for(int i = bytesPerPixel - 1; i >= 0; i--)
			//{
			//	//since TGA stores colors as BGR, we need to read it backwards to get RGB
			//	if(fread(runColor + i, 1, 1, file) != 1) { return -1; }
			//}
			for(int i = 0; i <= repCount; i++)
			{
				//put a pixel of the specified packet color into the image data buffer
				memcpy(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel), color, colorBytesPerPixel);
				if(imageType == 10)
				{
					//add the alpha channel
					if(opaque)
					{
						*(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel) + colorBytesPerPixel) = 0xFF;
					}
					else
					{
						*(imageData + (curPixels * GLbytesPerPixel) + (i * GLbytesPerPixel) + colorBytesPerPixel) = 0x00;
					}
				}
			}
		}
		curPixels += repCount + 1; //update how far we've moved through the actual pixels
	}

	fclose(file);

	delete(color);

	//create and bind the texture
	glActiveTextureARB(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	//set some options
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLint internalFormat;
	GLint format;
	if(imageType == 9)
	{
		if(openGL3)
		{
			internalFormat = GL_R8;
			format = GL_RED;
		}
		else
		{
			internalFormat = GL_ALPHA8;
			format = GL_ALPHA;
		}
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	else if(imageType == 10)
	{
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	//just... just jam it in!
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);

	delete(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	//make an array of coordinates based on the texture dimensions
	GLshort * dim = new GLshort[12];

	dim[0] = 0;				dim[1]  = 0;			dim[2]  = 0;
	dim[3] = imageWidth;	dim[4]  = 0;			dim[5]  = 0;
	dim[6] = imageWidth;	dim[7]  = imageHeight;	dim[8]  = 0;
	dim[9] = 0;				dim[10] = imageHeight;	dim[11] = 0;

	//make a buffer object
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLushort), dim, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//get the fuck outta here
	delete[] dim;

	//now, put all the results in the texture struct
	//hsTex->defined = true;
	hsTex->textureID = textureID;
	hsTex->bufferID = bufferID;
	hsTex->rightAlign = rightAlign;
	hsTex->topAlign = topAlign;
	if(imageType == 9) { hsTex->indexed = true; }
	else if(imageType == 10) { hsTex->indexed = false; }

	return 0;
}

int LoadHSPToPalette(HSPalette * hsPal)
{
	if(hsPal == NULL)
	{
		UpdateLog("HSPalette object is null.", true);
		return -1; //given palette is null
	}

	FILE * file;

	if(GLuint error = fopen_s(&file, hsPal->paletteFilePath.data(), "rb") != 0)
	{
		UpdateLog("Could not open palette file: " + hsPal->paletteFilePath, true);
		return error; //couldn't open the file
	}

	GLubyte * paletteData = (GLubyte*)malloc(1024 * sizeof(GLubyte));

	for(int i = 0; i < 256; i++)
	{
		if(fread((void *)(paletteData + i*4), 3, 1, file) != 1) {return freadError(file, hsPal->paletteFilePath);}

		*(paletteData + i*4 + 3) = 0x00;
	}

	fclose(file);

	return StorePaletteData(hsPal, paletteData);
}

int StorePaletteData(HSPalette * hsPal, GLubyte * paletteData)
{
	GLuint textureID;

	glActiveTextureARB(GL_TEXTURE1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, paletteData);

	delete(paletteData);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	////allocate space
	//unsigned int * paletteData = (unsigned int*)malloc(PALETTE_LENGTH * PALETTE_COLOR_LENGTH * sizeof(unsigned int));

	////have to turn each byte into an unsigned int. THANKS SHADERS
	//for(int i = 0; i < PALETTE_LENGTH * PALETTE_COLOR_LENGTH; i++)
	//{
	//	//get each color byte
	//	GLubyte byte;
	//	unsigned int intColor;
	//	if(fread(&byte, 1, 1, file) != 1) {return freadError(file, hsPal->paletteFilePath);}
	//	intColor = (unsigned int)byte;
	//	memcpy(paletteData + i, &intColor, sizeof(unsigned int));
	//}

	//save the palette data
	//hsPal->paletteData = paletteData;
	hsPal->textureID = textureID;

	return 0;
}