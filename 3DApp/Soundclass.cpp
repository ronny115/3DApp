#include "soundclass.h"

SoundClass::SoundClass()
{
	m_system = 0;
	m_sound1 = 0;
	//m_sound2 = 0;
	//m_sound3 = 0;
	version = 0;
	extraDriverData = 0;
}

SoundClass::SoundClass(const SoundClass& other)
{
}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize()
{
	//bool playing = 0;
	//Create object and initialize
	result = System_Create(&m_system);
	if (result != FMOD_OK){return false;}

	if (!m_system){return false;}

	result = m_system->getVersion(&version);
	if (result != FMOD_OK) { return false; }
	if (version < FMOD_VERSION){return false;}

	result = m_system->init(32, FMOD_INIT_NORMAL, extraDriverData);
	if (result != FMOD_OK) { return false; }

	return true;
}

void SoundClass::Load()
{
	m_system->createStream("data/sound/Sound01_1.wav", FMOD_DEFAULT, 0, &m_sound1);
	if (result != FMOD_OK) { return; }
	//result = m_system->createStream("data/sound/Sound01_1.wav", FMOD_DEFAULT, 0, &m_sound2);
	//if (result != FMOD_OK) { return false; }
	//result = m_system->createStream("data/sound/Sound01.wav", FMOD_DEFAULT, 0, &m_sound3);
	//if (result != FMOD_OK) { return false; }
}

void SoundClass::Play()
{
	m_system->playSound(m_sound1, 0, false, &m_channel);
	if (result != FMOD_OK) { return; }
	//result = m_system->playSound(m_sound2, 0, false, &m_channel);
	m_system->update();

}

void SoundClass::Shutdown()
{

	m_system->release();
	//result = m_sound2->release();
	//result = m_sound3->release();
	//result = m_system->close();
	//m_system->release();
}
