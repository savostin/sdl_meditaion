#ifndef CHANNEL_H
#define CHANNEL_H
#include "sound.h"
#include <vector>
#include <pthread.h>

#define DEFAULT_VOLUME 80

class Channel
{
private:
	Sound *sound;
	const int id;
	int volume;
	pthread_t thread;
	static vector<Channel *> channels;
	static void *work(void *data);

public:
	Channel(const int _id);
	~Channel();
	void setVolume(const int val = DEFAULT_VOLUME);
	const int getVolume() const;
	bool play(Sound *s, int count = 0);
	void finish();

	static int init(const int channels = 4);
	static void cleanup();
	static void check();
};

#endif