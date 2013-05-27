#include "audio.h"

int LoadWAVToAudio(HSAudio * aud, SDL_AudioSpec * obtainedAudioSpec)
{
	SDL_AudioSpec wav_spec; 
	SDL_AudioCVT wav_cvt; 
	Uint32 wav_len; 
	Uint8 *wav_buf;
	int ret;

	/* Load the test.wav */ 
	if( SDL_LoadWAV(aud->audioFilePath.data(), &wav_spec, &wav_buf, &wav_len) == NULL )
	{ 
		UpdateLog("Failed to load wav file: " + aud->audioFilePath, false);
		return -1; //problem loading the wav
	}

	/* Build AudioCVT */ 
	ret = SDL_BuildAudioCVT(&wav_cvt, wav_spec.format, wav_spec.channels, wav_spec.freq, obtainedAudioSpec->format, obtainedAudioSpec->channels, obtainedAudioSpec->freq); 

	/* Check that the convert was built */ 
	if(ret==-1)
	{ 
		UpdateLog("Failed to convert audio: " + aud->audioFilePath, false);
		return -1; //problem building audio converter
	} 

	/* Setup for conversion */ 
	wav_cvt.buf=(Uint8 *)malloc(wav_len*wav_cvt.len_mult); 
	wav_cvt.len=wav_len; 
	memcpy(wav_cvt.buf, wav_buf, wav_len); 

	/* We can delete to original WAV data now It is coming up
	next*/ 
	SDL_FreeWAV(wav_buf); 

	/* And now we're ready to convert */ 
	SDL_ConvertAudio(&wav_cvt);

	//store the data in the struct
	aud->data = wav_cvt.buf;
	aud->length = wav_cvt.len_cvt;

	return 0;
}