#ifndef __AUDIO_H_
#define __AUDIO_H_

#include <list>
#include <sstream>
#include <SDL.h>
#include "hslog.h"
using namespace std;

struct HSAudio
{
	Uint8 * data; //the actual sound data
	Uint32 length; //size of the data
	string audioFilePath; //for reloading and registry handling
	int usingCount; //number of holds using this
};

int LoadWAVToAudio(HSAudio * aud, SDL_AudioSpec * obtainedAudioSpec);

#endif