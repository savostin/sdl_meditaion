#include "sound.h"
#include <iostream>
#include <sstream>

using namespace std;
vector<Sound *> Sound::sounds;

Sound::Sound(const string &__filename) : playing(false), chunk(NULL), _filename(__filename), _length(0) {}
bool Sound::load()
{
	unload();
	// cout << "Loading " << _file << endl;
	chunk = Mix_LoadWAV(_filename.c_str());
	if (chunk == NULL)
	{
		cerr << "Failed to load file. SDL_Mixer error: " << Mix_GetError() << endl;
		return false;
	}
	_length = getChunkTimeMilliseconds();
	// cout << "Length: " << length << endl;
	return true;
}

void Sound::unload()
{
	if (chunk != NULL)
		Mix_FreeChunk(chunk);
	chunk = NULL;
	playing = false;
}

Uint32 Sound::length() const
{
	return _length;
}

string Sound::name() const
{
	return _filename;
}

void Sound::play()
{
	playing = true;
}

Sound::~Sound()
{
	unload();
}

Mix_Chunk *Sound::data() const
{
	return chunk;
}

bool Sound::add(const string &file)
{
	//	cout << "Adding " << file << endl;
	Sound *s = new Sound(file);
	sounds.push_back(s);
	return true;
}

Sound *Sound::get()
{
	int i = 10;
	while (i-- > 0)
	{
		Sound *s = Sound::sounds[rand() % Sound::sounds.size()];
		if (!s->playing)
		{
			return s;
		}
	}
	return NULL;
}

void Sound::cleanup()
{
	for (int i = 0; i < Sound::sounds.size(); i++)
	{
		delete Sound::sounds[i];
	}
	Sound::sounds.clear();
}

Uint32 Sound::getChunkTimeMilliseconds()
{
	Uint32 points = 0;
	Uint32 frames = 0;
	int freq = 0;
	Uint16 fmt = 0;
	int chans = 0;
	if (!Mix_QuerySpec(&freq, &fmt, &chans))
		return 0;

	/* bytes / samplesize == sample points */
	points = (chunk->alen / ((fmt & 0xFF) / 8));

	/* sample points / channels == sample frames */
	frames = (points / chans);

	/* (sample frames * 1000) / frequency == play length in ms */
	return (frames * 1000) / freq;
}