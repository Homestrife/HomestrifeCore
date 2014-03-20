#ifndef __RENDERINGMANAGER_H_
#define __RENDERINGMANAGER_H_

#define TARGET_FPS 60
#define MAX_GAME_RESOLUTION_X 1920
#define MAX_GAME_RESOLUTION_Y 1080
#define MIN_GAME_RESOLUTION_X 640
#define MIN_GAME_RESOLUTION_Y 360
#define GAME_ASPECT_RATIO_X 16
#define GAME_ASPECT_RATIO_Y 9

#define PAN_DIVIDER 5
#define ZOOM_BOUNDARY_X_THRESHOLD 1152
#define ZOOM_BOUNDARY_Y_THRESHOLD 648

#include "hsobject.h"
#include "objectManager.h"

class RenderingManager
{
public:
	RenderingManager(ObjectManager * objectManager);
	
	int Initialize();
	int Execute();

protected:
	GLuint currentShaderProgramID;
	int nonIndexedPosOffsetLoc;
	int indexedPosOffsetLoc;
	int nonIndexedScaleLoc;
	int indexedScaleLoc;
	int nonIndexedResolutionLoc;
	int indexedResolutionLoc;
	int nonIndexedResScaleLoc;
	int indexedResScaleLoc;
	int nonIndexedFocusPosLoc;
	int indexedFocusPosLoc;
	int nonIndexedZoomOutLoc;
	int indexedZoomOutLoc;
	int nonIndexedDepthLoc;
	int indexedDepthLoc;
	int nonIndexedTexLoc;
	int indexedTexLoc;
	int paletteLoc;
	ObjectManager * objectManager;
	SDL_Window* surf_display;
	SDL_DisplayMode startDisplayMode;
	SDL_DisplayMode curDisplayMode;
	float resolutionScale;
	float zoomOut;
	HSVect2D focusPos;

	int DefaultVideoConfig();
	int LoadVideoConfig();
	int SaveVideoConfig();
	
	void NextLowestFullscreenResolution();
	void NextLowestWindowedResolution();
	int DropToHighestValidFullscreenResolution();
	int DropToHighestValidWindowedResolution();
	int SetBestGameResolution();
	int SetBestDisplayMode();
	int ReturnToStartDisplayMode();
	void ChangeShaderProgram(GLuint programID);
	int SetFullScreen();
	
	void AdjustCamera(bool adjustInstantly);
	int LoadTexturesAndPalettes();
	int ClearTexturesAndPalettes();
	int Render();
	int RenderTexture(HSObject * obj, TextureInstance tex); 
};

#endif