#include "renderingManager.h"

RenderingManager::RenderingManager(ObjectManager * objectManager)
{
	this->objectManager = objectManager;
	nonIndexedScaleLoc = 0;
	indexedScaleLoc = 0;
	nonIndexedPosOffsetLoc = 0;
	indexedPosOffsetLoc = 0;
	nonIndexedResolutionLoc = 0;
	indexedResolutionLoc = 0;
	nonIndexedResScaleLoc = 0;
	indexedResScaleLoc = 0;
	nonIndexedFocusPosLoc = 0;
	indexedFocusPosLoc = 0;
	nonIndexedZoomOutLoc = 0;
	indexedZoomOutLoc = 0;
	nonIndexedDepthLoc = 0;
	indexedDepthLoc = 0;
	nonIndexedTexLoc = 0;
	indexedTexLoc = 0;
	paletteLoc = 0;
	surf_display = NULL;
	zoomOut = 1;
	focusPos.x = 0;
	focusPos.y = 0;
}

int RenderingManager::Execute()
{
	//if(int i = Initialize() != 0) { objectManager->renderingErrorCode = i; return i; }
	//SDL_mutexV(objectManager->vidInitLock);
	//SDL_CondSignal(objectManager->vidInitDone);

	//while(objectManager->notDone)
	//{
		//SDL_CondWait(objectManager->processDone, objectManager->processLock);

		if(objectManager->clearTexturesAndPalettes)
		{
			objectManager->clearTexturesAndPalettes = false;
			if(int i = ClearTexturesAndPalettes() != 0) { objectManager->renderingErrorCode = i; return i; }
		}

		if(objectManager->applyVideoSettings)
		{
			objectManager->applyVideoSettings = false;
			if(int i = SetFullScreen() != 0) { objectManager->renderingErrorCode = i; return i; }
		}

		if(objectManager->loadTexturesAndPalettes)
		{
			objectManager->loadTexturesAndPalettes = false;
			if(int i = LoadTexturesAndPalettes() != 0) { objectManager->renderingErrorCode = i; return i; }
		}

		if(objectManager->centerCameraInstantly)
		{
			objectManager->centerCameraInstantly = false;
			AdjustCamera(true);
		}

		if(objectManager->doRender)
		{
			objectManager->doRender = false;
			if(int i = Render() != 0) { objectManager->renderingErrorCode = i; return i; }
		}

		//SDL_mutexV(objectManager->renderLock);
		//SDL_CondSignal(objectManager->renderDone);
	//}

	return 0;
}

int RenderingManager::Initialize()
{
	glGetError();

	/*if(objectManager->textureRegistry.size() > 0)
	{
		list<HSTexture*>::iterator trIt;
		for ( trIt=objectManager->textureRegistry.begin(); trIt != objectManager->textureRegistry.end(); trIt++)
		{
			list<HSTextureSlice*>::iterator tsIt;
			for ( tsIt=(*trIt)->textureSlices.begin(); tsIt != (*trIt)->textureSlices.end(); tsIt++)
			{
				glDeleteTextures(1, &(*tsIt)->textureID);
				glDeleteBuffers(1, &(*tsIt)->bufferID);

				(*tsIt)->textureID = 0;
				(*tsIt)->bufferID = 0;
			}

			if((*trIt)->ownPalette != NULL)
			{
				glDeleteTextures(1, &(*trIt)->ownPalette->textureID);
				(*trIt)->ownPalette->textureID = 0;
			}
		}
	}

	if(objectManager->paletteRegistry.size() > 0)
	{
		list<HSPalette*>::iterator palIt;
		for (palIt=objectManager->paletteRegistry.begin(); palIt != objectManager->paletteRegistry.end(); palIt++)
		{
			glDeleteTextures(1, &(*palIt)->textureID);

			(*palIt)->textureID = 0;
		}
	}*/

	/*if(shader_vert != 0)
	{
		glDeleteShader(shader_vert);
		shader_vert = 0;
	}
	if(shader_fragNonIndexed != 0)
	{
		glDeleteShader(shader_fragNonIndexed);
		shader_fragNonIndexed = 0;
	}
	if(shader_fragIndexed != 0)
	{
		glDeleteShader(shader_fragIndexed);
		shader_fragIndexed = 0;
	}
	if(shader_progNonIndexed != 0)
	{
		glDeleteProgram(shader_progNonIndexed);
		shader_progNonIndexed = 0;
	}
	if(shader_progIndexed != 0)
	{
		glDeleteProgram(shader_progIndexed);
		shader_progIndexed = 0;
	}

	if(texCoordBufferID != 0)
	{
		glDeleteBuffers(1, &texCoordBufferID);
		texCoordBufferID = 0;
	}

	if(elementArrayBufferID != 0)
	{
		glDeleteBuffers(1, &elementArrayBufferID);
		elementArrayBufferID = 0;
	}*/
	
	SDL_GetDesktopDisplayMode(0, &startDisplayMode);
	curDisplayMode = startDisplayMode;

	if(int error = LoadVideoConfig() != 0) { return error; }

	int options = SDL_WINDOW_OPENGL;
	int gameResolutionX = objectManager->fullscreenResolutionX;
	int gameResolutionY = objectManager->fullscreenResolutionY;
	int windowSizeX = curDisplayMode.w;
	int windowSizeY = curDisplayMode.h;

	if(!objectManager->fullScreen)
	{
		if(objectManager->windowedResolutionX >= startDisplayMode.w || objectManager->windowedResolutionY >= startDisplayMode.h)
		{
			objectManager->windowedResolutionXToApply = MAX_GAME_RESOLUTION_X;
			objectManager->windowedResolutionYToApply = MAX_GAME_RESOLUTION_Y;
			if(int error = DropToHighestValidWindowedResolution() != 0) { return error; }
			objectManager->windowedResolutionX = objectManager->windowedResolutionXToApply;
			objectManager->windowedResolutionY = objectManager->windowedResolutionYToApply;
			if(int error = SaveVideoConfig() != 0) { return error; }
		}

		gameResolutionX = objectManager->windowedResolutionX;
		gameResolutionY = objectManager->windowedResolutionY;
		windowSizeX = gameResolutionX;
		windowSizeY = gameResolutionY;
	}
	else
	{
		options = options | SDL_WINDOW_FULLSCREEN;

		if(gameResolutionX > curDisplayMode.w || gameResolutionY > curDisplayMode.h || objectManager->stretchScreen)
		{
			if(int error = SetBestDisplayMode() != 0) { return error; }
			gameResolutionX = objectManager->fullscreenResolutionX;
			gameResolutionY = objectManager->fullscreenResolutionY;
			windowSizeX = curDisplayMode.w;
			windowSizeY = curDisplayMode.h;
		}
	}

	if((surf_display = SDL_CreateWindow("Homestrife",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
						  windowSizeX,
						  windowSizeY,
                          options)) == NULL)
	{
		UpdateLog("Error setting SDL video mode.", true);
		return -1;
	}
	SDL_GL_CreateContext(surf_display);

	UpdateLog("SDL video mode set.", false);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);

	glViewport((windowSizeX - gameResolutionX) / 2, (windowSizeY - gameResolutionY) / 2, gameResolutionX, gameResolutionY);

	resolutionScale = (float)gameResolutionX / (float)MAX_GAME_RESOLUTION_X;
 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SDL_GL_SwapWindow(surf_display);

	//get the current opengl version
	int glMajorVersion = 0;
	int glMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

	//gl major/minor version doesn't work, so use an older method
	if(glMajorVersion == 0)
	{
		string version; version.assign((const char *)glGetString(GL_VERSION));

		if(version.length() >= 1)
		{
			string major = version.substr(0, 1);
			glMajorVersion = atoi(major.c_str());
		}

		if(version.length() >= 3)
		{
			string minor = version.substr(2, 1);
			glMinorVersion = atoi(minor.c_str());
		}
	}

	//we need to know this for texture loading and storage
	if(glMajorVersion >= 3)
	{
		objectManager->openGL3 = true;
	}
	else
	{
		objectManager->openGL3 = false;
	}

	stringstream sstm;
	sstm << "This machine is running OpenGL version " << glMajorVersion << "." << glMinorVersion << ".";
	UpdateLog(sstm.str(), false);

	if(glMajorVersion < 2)
	{
		UpdateLog("OpenGL version is older than 2.0.", true);
		return -1;
	}

	setupExtensions();
	if(shading_enabled == false)
	{
		UpdateLog("OpenGL shaders are not enabled.", true);
		return -1;
	}
	if(buffer_objects_enabled == false)
	{
		UpdateLog("OpenGL buffer objects are not enabled.", true);
		return -1;
	}
	if(default_array_objects_enabled == false)
	{
		UpdateLog("OpenGL default array object is not enabled.", true);
		return -1;
	}
	if(custom_array_objects_enabled == false && objectManager->openGL3)
	{
		UpdateLog("OpenGL custom array objects are not enabled.", true);
		return -1;
	}

	UpdateLog("OpenGL shaders, buffer objects, and vertex array objects enabled.", false);

	//set up all the shader stuff
	shader_progNonIndexed = glCreateProgram();
	shader_progIndexed = glCreateProgram();
	shader_vert = glCreateShader(GL_VERTEX_SHADER);
	shader_fragNonIndexed = glCreateShader(GL_FRAGMENT_SHADER);
	shader_fragIndexed = glCreateShader(GL_FRAGMENT_SHADER);

	string vertShaderFileName;
	string fragNonIndexedShaderFileName;
	string fragIndexedShaderFileName;

	if(objectManager->openGL3)
	{
		vertShaderFileName = "vertexShader3.glsl.txt";
		fragNonIndexedShaderFileName = "fragmentShaderNonIndexed3.glsl.txt";
		fragIndexedShaderFileName = "fragmentShaderIndexed3.glsl.txt";
	}
	else
	{
		vertShaderFileName = "vertexShader2.glsl.txt";
		fragNonIndexedShaderFileName = "fragmentShaderNonIndexed2.glsl.txt";
		fragIndexedShaderFileName = "fragmentShaderIndexed2.glsl.txt";
	}

	string shader_vert_source_string = loadSource(vertShaderFileName);

	if(shader_vert_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read vertex shader source file: " + vertShaderFileName, true);
		return -1;
	}

	string shader_fragNonIndexed_source_string = loadSource(fragNonIndexedShaderFileName);

	if(shader_fragNonIndexed_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read non-indexed fragment shader source file: " + fragNonIndexedShaderFileName, true);
		return -1;
	}

	string shader_fragIndexed_source_string = loadSource(fragIndexedShaderFileName);

	if(shader_fragIndexed_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read indexed fragment shader source file: " + fragIndexedShaderFileName, true);
		return -1;
	}

	//add some newlines so it compiles correctly
	unsigned int verPos = shader_vert_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_vert_source_string.insert(verPos + 12, "\n"); }

	verPos = shader_fragNonIndexed_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_fragNonIndexed_source_string.insert(verPos + 12, "\n"); }

	verPos = shader_fragIndexed_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_fragIndexed_source_string.insert(verPos + 12, "\n"); }

	//add more newlines after semicolons so warning/error positions are meaningful
	size_t scPos = shader_vert_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_vert_source_string.insert(scPos+1, "\n");
		scPos = shader_vert_source_string.find(";", scPos+1);
	}
	
	scPos = shader_fragNonIndexed_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_fragNonIndexed_source_string.insert(scPos+1, "\n");
		scPos = shader_fragNonIndexed_source_string.find(";", scPos+1);
	}
	
	scPos = shader_fragIndexed_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_fragIndexed_source_string.insert(scPos+1, "\n");
		scPos = shader_fragIndexed_source_string.find(";", scPos+1);
	}

	const char * shader_vert_source = shader_vert_source_string.data();
	const char * shader_fragNonIndexed_source = shader_fragNonIndexed_source_string.data();
	const char * shader_fragIndexed_source = shader_fragIndexed_source_string.data();

	int len = 0;
	len = shader_vert_source_string.length();
	glShaderSource(shader_vert, 1, (const GLcharARB**)&shader_vert_source, &len);
	len = shader_fragNonIndexed_source_string.length();
	glShaderSource(shader_fragNonIndexed, 1, (const GLcharARB**)&shader_fragNonIndexed_source, &len);
	len = shader_fragIndexed_source_string.length();
	glShaderSource(shader_fragIndexed, 1, (const GLcharARB**)&shader_fragIndexed_source, &len);

	//compile vertex shader
	string shaderError = "";
	GLint shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_vert);
	glGetShaderiv(shader_vert, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Vertex shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Vertex shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling vertex shader: " + shaderError, true);
		}
		return -1;
	}
	
	//compile non-indexed fragment shader
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_fragNonIndexed);
	glGetShaderiv(shader_fragNonIndexed, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Non-Indexed fragment shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Non-Indexed fragment shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling non-indexed fragment shader: " + shaderError, true);
		}
		return -1;
	}
	
	//compile indexed fragment shader
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_fragIndexed);
	glGetShaderiv(shader_fragIndexed, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Indexed fragment shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Indexed fragment shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling indexed fragment shader: " + shaderError, true);
		}
		return -1;
	}

	//link non-indexed program
	glAttachShader(shader_progNonIndexed, shader_vert);
	glAttachShader(shader_progNonIndexed, shader_fragNonIndexed);
	
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = linkProgram(shader_progNonIndexed);
	glGetProgramiv(shader_progNonIndexed, GL_LINK_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Non-Indexed shaders linked successfully.", false);
		}
		else
		{
			UpdateLog("Non-Indexed shaders linked successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error linking non-indexed shaders: " + shaderError, true);
		}
		return -1;
	}

	//link indexed program
	glAttachShader(shader_progIndexed, shader_vert);
	glAttachShader(shader_progIndexed, shader_fragIndexed);
	
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = linkProgram(shader_progIndexed);
	glGetProgramiv(shader_progIndexed, GL_LINK_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Indexed shaders linked successfully.", false);
		}
		else
		{
			UpdateLog("Indexed shaders linked successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error linking indexed shaders: " + shaderError, true);
		}
		return -1;
	}

	positionLocNonIndexed = glGetAttribLocation(shader_progNonIndexed, "position");
	positionLocIndexed = glGetAttribLocation(shader_progIndexed, "position");
	texCoordInLocNonIndexed = glGetAttribLocation(shader_progNonIndexed, "texCoordIn");
	texCoordInLocIndexed = glGetAttribLocation(shader_progIndexed, "texCoordIn");

	nonIndexedPosOffsetLoc = glGetUniformLocation(shader_progNonIndexed, "posOffset");
	indexedPosOffsetLoc = glGetUniformLocation(shader_progIndexed, "posOffset");
	nonIndexedScaleLoc = glGetUniformLocation(shader_progNonIndexed, "scale");
	indexedScaleLoc = glGetUniformLocation(shader_progIndexed, "scale");
	nonIndexedResolutionLoc = glGetUniformLocation(shader_progNonIndexed, "resolution");
	indexedResolutionLoc = glGetUniformLocation(shader_progIndexed, "resolution");
	nonIndexedResScaleLoc = glGetUniformLocation(shader_progNonIndexed, "resScale");
	indexedResScaleLoc = glGetUniformLocation(shader_progIndexed, "resScale");
	nonIndexedFocusPosLoc = glGetUniformLocation(shader_progNonIndexed, "focusPos");
	indexedFocusPosLoc = glGetUniformLocation(shader_progIndexed, "focusPos");
	nonIndexedZoomOutLoc = glGetUniformLocation(shader_progNonIndexed, "zoomOut");
	indexedZoomOutLoc = glGetUniformLocation(shader_progIndexed, "zoomOut");
	nonIndexedDepthLoc =  glGetUniformLocation(shader_progNonIndexed, "depth");
	indexedDepthLoc =  glGetUniformLocation(shader_progIndexed, "depth");
	nonIndexedTexLoc = glGetUniformLocation(shader_progNonIndexed, "tex");
	indexedTexLoc = glGetUniformLocation(shader_progIndexed, "tex");
	paletteLoc = glGetUniformLocation(shader_progIndexed, "palette");

	ChangeShaderProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedResolutionLoc, (float)gameResolutionX, (float)gameResolutionY);
	glUniform1f(nonIndexedResScaleLoc, resolutionScale);
	
	ChangeShaderProgram(shader_progIndexed);
	glUniform2f(indexedResolutionLoc, (float)gameResolutionX, (float)gameResolutionY);
	glUniform1f(indexedResScaleLoc, resolutionScale);

	ChangeShaderProgram(0);

	//set up some VBO stuff
	if(texCoordBufferID == 0)
	{
		GLfloat * texCoord = new GLfloat[8];
		texCoord[0] = 0.0f;	texCoord[1] = 1.0f;
		texCoord[2] = 1.0f;	texCoord[3] = 1.0f;
		texCoord[4] = 1.0f;	texCoord[5] = 0.0f;
		texCoord[6] = 0.0f;	texCoord[7] = 0.0f;

		glGenBuffers(1, &texCoordBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
		glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), texCoord, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] texCoord;
	}

	if(elementArrayBufferID == 0)
	{
		GLubyte * eArray = new GLubyte[6];
		eArray[0] = 0; eArray[1] = 1; eArray[2] = 3;
		eArray[3] = 1; eArray[4] = 2; eArray[5] = 3;

		glGenBuffers(1, &elementArrayBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, elementArrayBufferID);
		glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLubyte), eArray, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] eArray;
	}

	if(!objectManager->openGL3)
	{
		//bind universal things to the default vertex array object
		glEnableVertexAttribArray(positionLocNonIndexed);
		if(positionLocNonIndexed != positionLocIndexed)
		{
			glEnableVertexAttribArray(positionLocIndexed);
		}
		glEnableVertexAttribArray(texCoordInLocNonIndexed);
		if(texCoordInLocNonIndexed != texCoordInLocIndexed)
		{
			glEnableVertexAttribArray(texCoordInLocIndexed);
		}

		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
		glVertexAttribPointer(texCoordInLocNonIndexed, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		if(texCoordInLocNonIndexed != texCoordInLocIndexed)
		{
			glVertexAttribPointer(texCoordInLocIndexed, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferID);
	}

	//if(objectManager->textureRegistry.size() > 0)
	//{
	//	//need to reload all textures
	//	list<HSTexture*>::iterator trIt;
	//	for ( trIt=objectManager->textureRegistry.begin(); trIt != objectManager->textureRegistry.end(); trIt++)
	//	{
	//		if(int error = LoadTGAToTexture((*trIt), objectManager->openGL3, (*trIt)->ownPalette != NULL) != 0)
	//		{
	//			return error;
	//		}
	//	}
	//}

	//if(objectManager->paletteRegistry.size() > 0)
	//{
	//	//need to reload all palettes
	//	list<HSPalette*>::iterator palIt;
	//	for ( palIt=objectManager->paletteRegistry.begin(); palIt != objectManager->paletteRegistry.end(); palIt++)
	//	{
	//		if(int error = LoadHSPToPalette((*palIt)) != 0)
	//		{
	//			return error;
	//		}
	//	}
	//}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexDimension);

	GLenum glError = glGetError();
	if(glError != GL_NO_ERROR)
	{
		string glErrorString = "OpenGL error in SetFullScreen(): " + GetGLErrorText(glError);
		UpdateLog(glErrorString, true);
	}
	return 0;
}

int RenderingManager::DefaultVideoConfig()
{
	objectManager->fullScreen = true;
	objectManager->stretchScreen = false;
	
	return SetBestGameResolution();
}

int RenderingManager::LoadVideoConfig()
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("data/config/videoConfig.xml") != 0)
	{
		//file doesn't exist, so just set some defaults and then save the configuration
		DefaultVideoConfig();
		return SaveVideoConfig();
	}

	if(strcmp(file->RootElement()->Value(), "VideoConfiguration") != 0)
	{
		//file isn't formatted correctly, so just set some defaults and then save the configuration
		DefaultVideoConfig();
		return SaveVideoConfig();
	}

	//loop through all the parts of the configuration
	for(XMLElement * i = file->RootElement()->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the config section
		const char * configSec = i->Value();

		//create a new object of the appropriate type
		if(strcmp(configSec, "General") == 0)
		{
			//get fullscreen
			const char * fs = i->Attribute("fullscreen");
			string b;
			if(fs != NULL)
			{
				b.assign(fs);
				if(b.compare("true") == 0) { objectManager->fullScreen = true; }
				else if(b.compare("false") == 0) { objectManager->fullScreen = false; }
			}

			//get stretchScreen
			const char * ss = i->Attribute("stretchScreen");
			if(ss != NULL)
			{
				b.assign(ss);
				if(b.compare("true") == 0) { objectManager->stretchScreen = true; }
				else if(b.compare("false") == 0) { objectManager->stretchScreen = false; }
			}

			//get resolution
			i->QueryIntAttribute("windowedResolutionX", &objectManager->windowedResolutionX);
			i->QueryIntAttribute("windowedResolutionY", &objectManager->windowedResolutionY);
			i->QueryIntAttribute("fullscreenResolutionX", &objectManager->fullscreenResolutionX);
			i->QueryIntAttribute("fullscreenResolutionY", &objectManager->fullscreenResolutionY);
		}
	}

	if(objectManager->windowedResolutionX > MAX_GAME_RESOLUTION_X || objectManager->windowedResolutionX < MIN_GAME_RESOLUTION_X
		|| objectManager->windowedResolutionY > MAX_GAME_RESOLUTION_Y || objectManager->windowedResolutionY < MIN_GAME_RESOLUTION_Y
		|| objectManager->fullscreenResolutionX > MAX_GAME_RESOLUTION_X || objectManager->fullscreenResolutionX < MIN_GAME_RESOLUTION_X
		|| objectManager->fullscreenResolutionY > MAX_GAME_RESOLUTION_Y || objectManager->fullscreenResolutionY < MIN_GAME_RESOLUTION_Y)
	{
		SetBestGameResolution();
		SaveVideoConfig();
	}

	return 0;
}

int RenderingManager::SaveVideoConfig()
{
	//create a new xml structure
	XMLDocument * file = new XMLDocument();

	XMLElement * general = file->NewElement("General");
	if(objectManager->fullScreen)
	{
		general->SetAttribute("fullscreen", "true");
	}
	else
	{
		general->SetAttribute("fullscreen", "false");
	}
	
	if(objectManager->stretchScreen)
	{
		general->SetAttribute("stretchScreen", "true");
	}
	else
	{
		general->SetAttribute("stretchScreen", "false");
	}

	general->SetAttribute("windowedResolutionX", objectManager->windowedResolutionX);
	general->SetAttribute("windowedResolutionY", objectManager->windowedResolutionY);
	general->SetAttribute("fullscreenResolutionX", objectManager->fullscreenResolutionX);
	general->SetAttribute("fullscreenResolutionY", objectManager->fullscreenResolutionY);

	XMLElement * config = file->NewElement("VideoConfiguration");
	config->InsertEndChild(general);

	file->InsertEndChild(config);

	file->SaveFile("data/config/videoConfig.xml");

	return 0;
}

void RenderingManager::NextLowestFullscreenResolution()
{
	if(objectManager->fullscreenResolutionXToApply == 1920)
	{
		objectManager->fullscreenResolutionXToApply = 1680; objectManager->fullscreenResolutionYToApply = 945;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1680)
	{
		objectManager->fullscreenResolutionXToApply = 1600; objectManager->fullscreenResolutionYToApply = 900;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1600)
	{
		objectManager->fullscreenResolutionXToApply = 1440; objectManager->fullscreenResolutionYToApply = 810;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1440)
	{
		objectManager->fullscreenResolutionXToApply = 1400; objectManager->fullscreenResolutionYToApply = 787;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1400)
	{
		objectManager->fullscreenResolutionXToApply = 1366; objectManager->fullscreenResolutionYToApply = 768;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1366)
	{
		objectManager->fullscreenResolutionXToApply = 1360; objectManager->fullscreenResolutionYToApply = 765;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1360)
	{
		objectManager->fullscreenResolutionXToApply = 1280; objectManager->fullscreenResolutionYToApply = 720;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1280)
	{
		objectManager->fullscreenResolutionXToApply = 1152; objectManager->fullscreenResolutionYToApply = 648;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1152)
	{
		objectManager->fullscreenResolutionXToApply = 1024; objectManager->fullscreenResolutionYToApply = 576;
	}
	else if(objectManager->fullscreenResolutionXToApply == 1024)
	{
		objectManager->fullscreenResolutionXToApply = 800; objectManager->fullscreenResolutionYToApply = 450;
	}
	else if(objectManager->fullscreenResolutionXToApply == 800)
	{
		objectManager->fullscreenResolutionXToApply = 640; objectManager->fullscreenResolutionYToApply = 360;
	}
}

void RenderingManager::NextLowestWindowedResolution()
{
	if(objectManager->windowedResolutionXToApply == 1920)
	{
		objectManager->windowedResolutionXToApply = 1680; objectManager->windowedResolutionYToApply = 945;
	}
	else if(objectManager->windowedResolutionXToApply == 1680)
	{
		objectManager->windowedResolutionXToApply = 1600; objectManager->windowedResolutionYToApply = 900;
	}
	else if(objectManager->windowedResolutionXToApply == 1600)
	{
		objectManager->windowedResolutionXToApply = 1440; objectManager->windowedResolutionYToApply = 810;
	}
	else if(objectManager->windowedResolutionXToApply == 1440)
	{
		objectManager->windowedResolutionXToApply = 1400; objectManager->windowedResolutionYToApply = 787;
	}
	else if(objectManager->windowedResolutionXToApply == 1400)
	{
		objectManager->windowedResolutionXToApply = 1366; objectManager->windowedResolutionYToApply = 768;
	}
	else if(objectManager->windowedResolutionXToApply == 1366)
	{
		objectManager->windowedResolutionXToApply = 1360; objectManager->windowedResolutionYToApply = 765;
	}
	else if(objectManager->windowedResolutionXToApply == 1360)
	{
		objectManager->windowedResolutionXToApply = 1280; objectManager->windowedResolutionYToApply = 720;
	}
	else if(objectManager->windowedResolutionXToApply == 1280)
	{
		objectManager->windowedResolutionXToApply = 1152; objectManager->windowedResolutionYToApply = 648;
	}
	else if(objectManager->windowedResolutionXToApply == 1152)
	{
		objectManager->windowedResolutionXToApply = 1024; objectManager->windowedResolutionYToApply = 576;
	}
	else if(objectManager->windowedResolutionXToApply == 1024)
	{
		objectManager->windowedResolutionXToApply = 800; objectManager->windowedResolutionYToApply = 450;
	}
	else if(objectManager->windowedResolutionXToApply == 800)
	{
		objectManager->windowedResolutionXToApply = 640; objectManager->windowedResolutionYToApply = 360;
	}
}

int RenderingManager::DropToHighestValidFullscreenResolution()
{
	//pick a viable 16:9 resolution that's equal to or smaller than the screen resolution
	while(objectManager->fullscreenResolutionXToApply > curDisplayMode.w || objectManager->fullscreenResolutionYToApply > curDisplayMode.h)
	{
		if(objectManager->fullscreenResolutionXToApply == 640)
		{
			UpdateLog("DropToHighestValidFullscreenResolution(): Display mode too small.", true);
			return -1;
		}

		NextLowestFullscreenResolution();
	}

	objectManager->needApplyFullscreenResolutionForMenu = true;
	objectManager->fullscreenResolutionXForMenu = objectManager->fullscreenResolutionXToApply;
	objectManager->fullscreenResolutionYForMenu = objectManager->fullscreenResolutionYToApply;

	return 0;
}

int RenderingManager::DropToHighestValidWindowedResolution()
{
	//pick a viable 16:9 resolution that's equal to or smaller than the screen resolution
	while(objectManager->windowedResolutionXToApply >= startDisplayMode.w || objectManager->windowedResolutionYToApply >= startDisplayMode.h)
	{
		if(objectManager->windowedResolutionXToApply == 640)
		{
			UpdateLog("DropToHighestValidWindowedResolution(): Display mode too small.", true);
			return -1;
		}

		NextLowestWindowedResolution();
	}

	objectManager->needApplyWindowedResolutionForMenu = true;
	objectManager->windowedResolutionXForMenu = objectManager->windowedResolutionXToApply;
	objectManager->windowedResolutionYForMenu = objectManager->windowedResolutionYToApply;

	return 0;
}

int RenderingManager::SetBestGameResolution()
{
	objectManager->fullscreenResolutionXToApply = MAX_GAME_RESOLUTION_X;
	objectManager->fullscreenResolutionYToApply = MAX_GAME_RESOLUTION_Y;
	objectManager->windowedResolutionXToApply = MAX_GAME_RESOLUTION_X;
	objectManager->windowedResolutionYToApply = MAX_GAME_RESOLUTION_Y;

	if(int i = DropToHighestValidFullscreenResolution() != 0) { return i; }
	if(int i = DropToHighestValidWindowedResolution() != 0) { return i; }

	objectManager->fullscreenResolutionX = objectManager->fullscreenResolutionXToApply;
	objectManager->fullscreenResolutionY = objectManager->fullscreenResolutionYToApply;
	objectManager->windowedResolutionX = objectManager->windowedResolutionXToApply;
	objectManager->windowedResolutionY = objectManager->windowedResolutionYToApply;

	return 0;
}

int RenderingManager::SetBestDisplayMode()
{
	if(!objectManager->fullScreen || !objectManager->stretchScreen) { return 0; }
	
	SDL_DisplayMode modeToUse;
	SDL_DisplayMode highestDisplayMode;
	highestDisplayMode.w = 0;
	highestDisplayMode.h = 0;
	highestDisplayMode.refresh_rate = 0;

	//get the highest display mode
	int numModes = SDL_GetNumDisplayModes(0);
	for(int i = 0; i < numModes; i++)
	{
		SDL_DisplayMode curMode;
		SDL_GetDisplayMode(0, i, &curMode);

		if(curMode.w >= highestDisplayMode.w && curMode.h >= highestDisplayMode.h)
		{
			if(curMode.w != highestDisplayMode.w || curMode.h != highestDisplayMode.h ||
				(curMode.w == highestDisplayMode.w && curMode.h == highestDisplayMode.h && curMode.refresh_rate > highestDisplayMode.refresh_rate))
			{
				highestDisplayMode = curMode;
			}
		}
	}

	if(highestDisplayMode.w < objectManager->fullscreenResolutionX || highestDisplayMode.h < objectManager->fullscreenResolutionY)
	{
		curDisplayMode = highestDisplayMode;
		objectManager->fullscreenResolutionXToApply = MAX_GAME_RESOLUTION_X;
		objectManager->fullscreenResolutionYToApply = MAX_GAME_RESOLUTION_Y;
		if(int error = DropToHighestValidFullscreenResolution() != 0) { return error; }
		objectManager->fullscreenResolutionX = objectManager->fullscreenResolutionXToApply;
		objectManager->fullscreenResolutionY = objectManager->fullscreenResolutionYToApply;
		if(int error = SaveVideoConfig() != 0) { return error; }
		return 0;
	}

	modeToUse = highestDisplayMode;

	//find the closest display mode to the fullscreen resolution, moving downward from the highest resolution
	for(int i = 0; i < numModes; i++)
	{
		SDL_DisplayMode curMode;
		if(int error = SDL_GetDisplayMode(0, i, &curMode) != 0) { return error; }

		if(curMode.w < objectManager->fullscreenResolutionX || curMode.h < objectManager->fullscreenResolutionY)
		{
			continue;
		}

		if(curMode.w <= modeToUse.w && curMode.h <= modeToUse.h)
		{
			if(curMode.w != modeToUse.w || curMode.h != modeToUse.w ||
				(curMode.w == modeToUse.w && curMode.h == modeToUse.h && curMode.refresh_rate > modeToUse.refresh_rate))
			{
				modeToUse = curMode;
			}
		}
	}

	curDisplayMode = modeToUse;

	return 0;
}

int RenderingManager::ReturnToStartDisplayMode()
{
	curDisplayMode = startDisplayMode;

	if(objectManager->fullscreenResolutionX > curDisplayMode.w || objectManager->fullscreenResolutionY > curDisplayMode.h)
	{
		if(int error = SetBestDisplayMode() != 0) { return error; }
	}

	return 0;
}

void RenderingManager::ChangeShaderProgram(GLuint programID)
{
	glUseProgram(programID);
	currentShaderProgramID = programID;
}

int RenderingManager::SetFullScreen()
{
	objectManager->fullScreen = objectManager->fullScreenToApply;
	if(int error = SaveVideoConfig() != 0) { return error; }

	int gameResolutionX = objectManager->fullscreenResolutionX;
	int gameResolutionY = objectManager->fullscreenResolutionY;
	int windowSizeX = curDisplayMode.w;
	int windowSizeY = curDisplayMode.h;

	if(!objectManager->fullScreen)
	{
		if(objectManager->windowedResolutionX >= startDisplayMode.w || objectManager->windowedResolutionY >= startDisplayMode.h)
		{
			objectManager->windowedResolutionXToApply = MAX_GAME_RESOLUTION_X;
			objectManager->windowedResolutionYToApply = MAX_GAME_RESOLUTION_Y;
			if(int i = DropToHighestValidWindowedResolution() != 0) { return i; }
			objectManager->windowedResolutionX = objectManager->windowedResolutionXToApply;
			objectManager->windowedResolutionY = objectManager->windowedResolutionYToApply;
			if(int error = SaveVideoConfig() != 0) { return error; }
		}

		gameResolutionX = objectManager->windowedResolutionX;
		gameResolutionY = objectManager->windowedResolutionY;
		windowSizeX = gameResolutionX;
		windowSizeY = gameResolutionY;
	}
	else
	{
		if(objectManager->stretchScreen)
		{
			if(int i = SetBestDisplayMode() != 0) { return i; }
			gameResolutionX = objectManager->fullscreenResolutionX;
			gameResolutionY = objectManager->fullscreenResolutionY;
			windowSizeX = curDisplayMode.w;
			windowSizeY = curDisplayMode.h;

			if(int i = SDL_SetWindowDisplayMode(surf_display, &curDisplayMode) != 0)
			{
				UpdateLog("SDL Error setting display mode after SetBestDisplayMode().", true);
				return i;
			}

			UpdateLog("Display mode set.", false);
			
			//do this to actually get the display mode change to kick in. it seems dumb but I don't know why SetWindowDisplayMode() isn't working by itself
			if(int i = SDL_SetWindowFullscreen(surf_display, 0) != 0)
			{
				UpdateLog("SDL Error setting windowed.", true);
				return i;
			}
		}
		else
		{
			if(int i = ReturnToStartDisplayMode() != 0) { return i; }
			gameResolutionX = objectManager->fullscreenResolutionX;
			gameResolutionY = objectManager->fullscreenResolutionY;
			windowSizeX = curDisplayMode.w;
			windowSizeY = curDisplayMode.h;

			if(int i = SDL_SetWindowDisplayMode(surf_display, &curDisplayMode) != 0)
			{
				UpdateLog("SDL Error setting display mode after ReturnToStartDisplayMode().", true);
				return i;
			}

			UpdateLog("Display mode set.", false);
			
			//do this to actually get the display mode change to kick in. it seems dumb but I don't know why SetWindowDisplayMode() isn't working by itself
			if(int i = SDL_SetWindowFullscreen(surf_display, 0) != 0)
			{
				UpdateLog("SDL Error setting windowed.", true);
				return i;
			}
		}
	}

	glViewport((windowSizeX - gameResolutionX) / 2, (windowSizeY - gameResolutionY) / 2, gameResolutionX, gameResolutionY);
	
	resolutionScale = (float)gameResolutionX / (float)MAX_GAME_RESOLUTION_X;

	if(objectManager->fullScreen)
	{
		int options = SDL_WINDOW_FULLSCREEN;
		SDL_SetWindowSize(surf_display, windowSizeX, windowSizeY);
		if(int i = SDL_SetWindowFullscreen(surf_display, SDL_WINDOW_FULLSCREEN) != 0)
		{
			UpdateLog("SDL Error setting fullscreen.", true);
			return i;
		}
	}
	else
	{
		if(int i = SDL_SetWindowFullscreen(surf_display, 0) != 0)
		{
			UpdateLog("SDL Error setting windowed.", true);
			return i;
		}
		SDL_SetWindowSize(surf_display, windowSizeX, windowSizeY);
	}

	SDL_SetWindowPosition(surf_display, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	return 0;
}

void RenderingManager::AdjustCamera(bool adjustInstantly)
{
	if(zoomOut < 1) { zoomOut = 1; }

	float targetZoomOut = 1;
	HSVect2D targetFocusPos;
	targetFocusPos.x = 0;
	targetFocusPos.y = 0;
	float top = 0;
	float bottom = 0;
	float left = 0;
	float right = 0;

	//get the target camera position
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(objectManager->focusObject[i] == NULL)
		{
			continue;
		}

		HSVect2D checkPos;

		if(objectManager->focusObject[i]->IsFighter())
		{
			Fighter * f = (Fighter*)objectManager->focusObject[i];

			//get the center of the fighter's terrain box
			checkPos.x = f->pos.x + f->firstTerrainBox->offset.x + f->firstTerrainBox->width/2;
			checkPos.y = f->pos.y + f->firstTerrainBox->offset.y + f->firstTerrainBox->height/2;
		}
		else
		{
			checkPos.x = objectManager->focusObject[i]->pos.x;
			checkPos.y = objectManager->focusObject[i]->pos.y;
		}

		if(i == 0)
		{
			left = checkPos.x;
			right = checkPos.x;
			top = checkPos.y;
			bottom = checkPos.y;
		}
		else
		{
			if(checkPos.x < left) { left = checkPos.x; }
			if(checkPos.x > right) { right = checkPos.x; }
			if(checkPos.y < top) { top = checkPos.y; }
			if(checkPos.y > bottom) { bottom = checkPos.y; }
		}
	}

	targetFocusPos.x = left + (right - left) / 2;
	targetFocusPos.y = top + (bottom - top) / 2;

	//get the target camera zoom
	if(right - left <= MAX_GAME_RESOLUTION_X - ZOOM_BOUNDARY_X_THRESHOLD && bottom - top <= MAX_GAME_RESOLUTION_Y - ZOOM_BOUNDARY_Y_THRESHOLD)
	{
		targetZoomOut = 1;
	}
	else
	{
		float targetZoomOutX;
		if(MAX_GAME_RESOLUTION_X - ZOOM_BOUNDARY_X_THRESHOLD <= 0) { targetZoomOutX = objectManager->stageSize.x / MAX_GAME_RESOLUTION_X; }
		else { targetZoomOutX = (right - left) / (MAX_GAME_RESOLUTION_X - ZOOM_BOUNDARY_X_THRESHOLD); }

		float targetZoomOutY;
		if(MAX_GAME_RESOLUTION_Y - ZOOM_BOUNDARY_Y_THRESHOLD <= 0) { targetZoomOutY = objectManager->stageSize.y / MAX_GAME_RESOLUTION_Y; }
		else { targetZoomOutY = (bottom - top) / (MAX_GAME_RESOLUTION_Y - ZOOM_BOUNDARY_Y_THRESHOLD); }

		if(targetZoomOutX > targetZoomOutY) { targetZoomOut = targetZoomOutX; }
		else { targetZoomOut = targetZoomOutY; }
	}

	if(adjustInstantly)
	{
		//go to the target values instantly
		zoomOut = targetZoomOut;
		focusPos.x = targetFocusPos.x;
		focusPos.y = targetFocusPos.y;
	}
	else
	{
		//pan/zoom towards the target values gradually
		if(focusPos.x != targetFocusPos.x || focusPos.y != targetFocusPos.y)
		{
			HSVect2D posDiff;
			posDiff.x = targetFocusPos.x - focusPos.x;
			posDiff.y = targetFocusPos.y - focusPos.y;

			HSVectComp diffLength = sqrt(pow(posDiff.x, 2) + pow(posDiff.y, 2));
			HSVect2D diffNormal;
			diffNormal.x = posDiff.x / diffLength;
			diffNormal.y = posDiff.y / diffLength;
			HSVectComp panLength = diffLength / PAN_DIVIDER;

			HSVect2D posChange;
			posChange.x = diffNormal.x * panLength;
			posChange.y = diffNormal.y * panLength;

			if((focusPos.x < targetFocusPos.x && focusPos.x + posChange.x > targetFocusPos.x) ||
				(focusPos.x > targetFocusPos.x && focusPos.x + posChange.x < targetFocusPos.x))
			{
				focusPos.x = targetFocusPos.x;
			}
			else
			{
				focusPos.x += posChange.x;
			}
			
			if((focusPos.y < targetFocusPos.y && focusPos.y + posChange.y > targetFocusPos.y) ||
				(focusPos.y > targetFocusPos.y && focusPos.y + posChange.y < targetFocusPos.y))
			{
				focusPos.y = targetFocusPos.y;
			}
			else
			{
				focusPos.y += posChange.y;
			}

			if(focusPos.x == targetFocusPos.x && focusPos.y == targetFocusPos.y)
			{
				zoomOut = targetZoomOut;
			}
			else
			{
				HSVectComp changeLength = sqrt(pow(posChange.x, 2) + pow(posChange.y, 2));
				float zoomRatio = changeLength / diffLength;

				float zoomDiff = targetZoomOut - zoomOut;
				zoomOut += zoomDiff * zoomRatio;
			}
		}
		else
		{
			zoomOut = targetZoomOut;
		}
	}

	if(objectManager->matchCamera)
	{
		//adjust zoom based on stage size
		HSVect2D viewportSize;
		viewportSize.x = MAX_GAME_RESOLUTION_X * zoomOut;
		viewportSize.y = MAX_GAME_RESOLUTION_Y * zoomOut;

		if(viewportSize.x > objectManager->stageSize.x)
		{
			zoomOut = objectManager->stageSize.x / MAX_GAME_RESOLUTION_X;
			viewportSize.x = MAX_GAME_RESOLUTION_X * zoomOut;
			viewportSize.y = MAX_GAME_RESOLUTION_Y * zoomOut;
		}

		if(viewportSize.y > objectManager->stageSize.y)
		{
			zoomOut = objectManager->stageSize.y / MAX_GAME_RESOLUTION_Y;
			viewportSize.x = MAX_GAME_RESOLUTION_X * zoomOut;
			viewportSize.y = MAX_GAME_RESOLUTION_Y * zoomOut;
		}

		//adjust focus position based on stage boundary

		if(focusPos.x - (viewportSize.x / 2) < objectManager->stageSize.x / -2)
		{
			focusPos.x = (objectManager->stageSize.x / -2) + (viewportSize.x / 2);
		}
		else if(focusPos.x + (viewportSize.x / 2) > objectManager->stageSize.x / 2)
		{
			focusPos.x = (objectManager->stageSize.x / 2) - (viewportSize.x / 2);
		}
		if(focusPos.y - (viewportSize.y / 2) < objectManager->stageSize.y / -2)
		{
			focusPos.y = (objectManager->stageSize.y / -2) + (viewportSize.y / 2);
		}
		else if(focusPos.y + (viewportSize.y / 2) > objectManager->stageSize.y / 2)
		{
			focusPos.y = (objectManager->stageSize.y / 2) - (viewportSize.y / 2);
		}
	}
}

int RenderingManager::LoadTexturesAndPalettes()
{
	list<HSTexture*>::iterator texIt;
	for(texIt = objectManager->textureRegistry.begin(); texIt != objectManager->textureRegistry.end(); texIt++)
	{
		if((*texIt)->textureSlices.size() > 0) { continue; }

		HSPalette * palette = NULL;

		if(int i = LoadTGAToTexture(*texIt, objectManager->openGL3, (*texIt)->useTGAPalette, &palette) != 0) { return i; }

		if(palette != NULL)
		{
			objectManager->paletteRegistry.push_back(palette);
		}
	}
	for(texIt = objectManager->loadingScreenTextures.begin(); texIt != objectManager->loadingScreenTextures.end(); texIt++)
	{
		if((*texIt)->textureSlices.size() > 0) { continue; }

		HSPalette * palette = NULL;

		if(int i = LoadTGAToTexture(*texIt, objectManager->openGL3, (*texIt)->useTGAPalette, &palette) != 0) { return i; }

		if(palette != NULL)
		{
			objectManager->loadingScreenPalettes.push_back(palette);
		}
	}
	
	list<HSPalette*>::iterator palIt;
	for(palIt = objectManager->paletteRegistry.begin(); palIt != objectManager->paletteRegistry.end(); palIt++)
	{
		if((*palIt)->textureID != 0) { continue; }

		if(int i = LoadHSPToPalette(*palIt) != 0) { return i; }
	}
	for(palIt = objectManager->loadingScreenPalettes.begin(); palIt != objectManager->loadingScreenPalettes.end(); palIt++)
	{
		if((*palIt)->textureID != 0) { continue; }

		if(int i = LoadHSPToPalette(*palIt) != 0) { return i; }
	}

	return 0;
}

int RenderingManager::ClearTexturesAndPalettes()
{
	list<GLuint>::iterator idIt;
	for(idIt = objectManager->texturesToDelete.begin(); idIt != objectManager->texturesToDelete.end(); idIt++)
	{
		if(*idIt == 0) { continue; }
		glDeleteTextures(1, &(*idIt));
	}

	objectManager->texturesToDelete.clear();
	
	for(idIt = objectManager->buffersToDelete.begin(); idIt != objectManager->buffersToDelete.end(); idIt++)
	{
		if(*idIt == 0) { continue; }
		glDeleteBuffers(1, &(*idIt));
	}

	objectManager->buffersToDelete.clear();
	
	for(idIt = objectManager->vaosToDelete.begin(); idIt != objectManager->vaosToDelete.end(); idIt++)
	{
		if(*idIt == 0) { continue; }
		glDeleteVertexArrays(1, &(*idIt));
	}

	objectManager->vaosToDelete.clear();
	
	for(idIt = objectManager->palettesToDelete.begin(); idIt != objectManager->palettesToDelete.end(); idIt++)
	{
		if(*idIt == 0) { continue; }
		glDeleteTextures(1, &(*idIt));
	}

	objectManager->palettesToDelete.clear();

	return 0;
}

int RenderingManager::Render()
{
	glGetError();

	AdjustCamera(false);

	ChangeShaderProgram(shader_progIndexed);
	glUniform2f(indexedFocusPosLoc, focusPos.x, focusPos.y);
	glUniform1f(indexedZoomOutLoc, zoomOut - 1);
	
	ChangeShaderProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedFocusPosLoc, focusPos.x, focusPos.y);
	glUniform1f(nonIndexedZoomOutLoc, zoomOut - 1);
	
	//render objects
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}

	//left = MAX_GAME_RESOLUTION_X / -2;
	//right = MAX_GAME_RESOLUTION_X / 2;
	//bottom = MAX_GAME_RESOLUTION_Y / 2;
	//top = MAX_GAME_RESOLUTION_Y / -2;

	//render HUD
	//glPushMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(left, right, bottom, top, 1, -1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	
	ChangeShaderProgram(shader_progIndexed);
	glUniform2f(indexedFocusPosLoc, 0.0f, 0.0f);
	glUniform1f(indexedZoomOutLoc, 0.0f);
	
	ChangeShaderProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedFocusPosLoc, 0.0f, 0.0f);
	glUniform1f(nonIndexedZoomOutLoc, 0.0f);

	//ChangeShaderProgram(0);

	for ( objIt=objectManager->HUDObjects.begin(); objIt != objectManager->HUDObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			//a quick kludge to keep the HUD from being affected by parallax. Should add some sort of shader input variable later
			float depth = (*objIt)->depth;
			(*objIt)->depth = 0;
			RenderTexture((*objIt), (*texIt));
			(*objIt)->depth = depth;
		}
	}
	for ( objIt=objectManager->loadingScreenObjects.begin(); objIt != objectManager->loadingScreenObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			//a quick kludge to keep the HUD from being affected by parallax. Should add some sort of shader input variable later
			float depth = (*objIt)->depth;
			(*objIt)->depth = 0;
			RenderTexture((*objIt), (*texIt));
			(*objIt)->depth = depth;
		}
	}

	//glPopMatrix();

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_INDEX_ARRAY);
	
	SDL_GL_SwapWindow(surf_display);

	GLenum glError = glGetError();
	if(glError != GL_NO_ERROR)
	{
		string glErrorString = "OpenGL error in Render(): " + GetGLErrorText(glError);
		UpdateLog(glErrorString, true);
	}

	return 0;
}

int RenderingManager::RenderTexture(HSObject * obj, TextureInstance tex)
{
	if(tex.hsTex->textureSlices.size() <= 0) { return 0; }

	//get the actual texture offset, based on the object's current hFlip
	float hScale = tex.hScale;
	float vScale = tex.vScale;
	if(obj->hFlip)
	{
		hScale *= -1;
	}

	//get the offset
	float offsetX = obj->pos.x;
	if(obj->hFlip)
	{
		offsetX += tex.offset.x * -1;
	}
	else
	{
		offsetX += tex.offset.x;
	}
	float offsetY = obj->pos.y + tex.offset.y;

	//set up the texture
	if(tex.hsTex->indexed)
	{
		if(currentShaderProgramID != shader_progIndexed)
		{
			ChangeShaderProgram(shader_progIndexed);
		}

		//set up the palette
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(paletteLoc, 1);
		if(obj->palette == NULL || obj->useTGAPalettes)
		{
			glBindTexture(GL_TEXTURE_2D, tex.hsTex->ownPalette->textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, obj->palette->textureID);
		}
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		//set up the general texture settings
		glUniform2f(indexedScaleLoc, hScale, vScale);
		glUniform1f(indexedDepthLoc, obj->depth);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(indexedTexLoc, 0);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	else
	{
		if(currentShaderProgramID != shader_progNonIndexed)
		{
			ChangeShaderProgram(shader_progNonIndexed);
		}

		//set up the general texture settings
		glUniform2f(nonIndexedScaleLoc, hScale, vScale);
		glUniform1f(nonIndexedDepthLoc, obj->depth);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(nonIndexedTexLoc, 0);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	list<HSTextureSlice*>::iterator tsItr;
	for(tsItr = tex.hsTex->textureSlices.begin(); tsItr != tex.hsTex->textureSlices.end(); tsItr++)
	{
		//loop through the texture slices
		float sliceOffsetX = (*tsItr)->offset.x;
		if(obj->hFlip)
		{
			sliceOffsetX = sliceOffsetX * -1;
		}
		float sliceOffsetY = (*tsItr)->offset.y;
		if(tex.hsTex->indexed)
		{
			glUniform2f(indexedPosOffsetLoc, offsetX + sliceOffsetX, offsetY + sliceOffsetY);
		}
		else
		{
			glUniform2f(nonIndexedPosOffsetLoc, offsetX + sliceOffsetX, offsetY + sliceOffsetY);
		}
		glBindTexture(GL_TEXTURE_2D, (*tsItr)->textureID);

		if(objectManager->openGL3)
		{
			//get the vertex array set up
			glBindVertexArray((*tsItr)->vaoID);
		}
		else
		{
			GLenum positionLoc;

			if(tex.hsTex->indexed)
			{
				positionLoc = positionLocIndexed;
			}
			else
			{
				positionLoc = positionLocNonIndexed;
			}

			glBindBuffer(GL_ARRAY_BUFFER, (*tsItr)->bufferID);
			glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//draw stuff
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

		//unbind stuff
		if(objectManager->openGL3)
		{
			glBindVertexArray(0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}