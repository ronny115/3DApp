
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLAS_H_
#include <fmod.hpp>
using namespace FMOD;

class SoundClass
{
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();
	bool Initialize();
	void Shutdown();

	void Load();
	void Play();
	//void Stop();
	//void Pause();
	//void Resume();
	//void Next(int& trackNo, float& totalTrackLength);
	//void Previous(int& trackNo, float& totalTrackLength);

private:
	bool m_isFirstRun;
	bool m_isLoad;
	bool m_isPlay;
	int  m_currentTrackNo;
	unsigned int* m_trackLength;

	FMOD_RESULT result;
	System* m_system;
	Channel* m_channel;
	Sound* m_sound1; //*m_sound2, *m_sound3;
	unsigned int version;
	void **extraDriverData;
};


#endif