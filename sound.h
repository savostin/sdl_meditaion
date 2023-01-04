#ifndef SOUND_H
#define SOUND_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

using namespace std;

class Sound
{
private:
	bool playing;
	string _filename;
	Uint32 _length;
	Mix_Chunk* chunk;
	Uint32 getChunkTimeMilliseconds();

	Sound(const string &filename);

	static vector<Sound *> sounds;
public:
	~Sound();
	bool load();
	void unload();
	Uint32 length() const;
	string name() const;
	void play();
	Mix_Chunk* data() const;

	static bool add(const string &file);
	static Sound *get();
	static void cleanup();
};

#endif