#include "channel.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

vector<Channel *> Channel::channels;
#define FADE_LENGTH 5000.
#define SHORT_FADE 2000
struct thread_info
{
	Channel *channel;
	int count;
	Sound *sound;
	pthread_t id;
};

void *Channel::work(void *data)
{
	struct thread_info *tinfo = (struct thread_info *)data;
	if (tinfo->sound->load())
	{
		int count = min(tinfo->count, (int)max(1., (float)tinfo->sound->length() / 2*FADE_LENGTH) );
		if (tinfo->sound->length() < 30 * 1000)
			count = 1;
		int fout = min(count * tinfo->sound->length() * 0.2, FADE_LENGTH);
		int fin = min(count * tinfo->sound->length() * 0.2, FADE_LENGTH);
		int to = count * tinfo->sound->length() - fout;
		cout << "[" << tinfo->channel->id << "] " << tinfo->sound->name() << " , volume = " << tinfo->channel->getVolume() << ", length = " << int(tinfo->sound->length() / 1000.) << "s, " << count << " times, fade out in " << int(to / 1000.) << " s" << endl;
		int rch = Mix_FadeInChannel(-1, tinfo->sound->data(), 0, fin);
		for (int i = 0; i < count - 1; i++)
		{
			SDL_Delay(tinfo->sound->length() - SHORT_FADE);
			Mix_FadeOutChannel(rch, SHORT_FADE);
			rch = Mix_FadeInChannel(-1, tinfo->sound->data(), 0, SHORT_FADE);
			Mix_Volume(rch, MIX_MAX_VOLUME * tinfo->channel->getVolume() / 100);
		}
		SDL_Delay(tinfo->sound->length() - fout);
		Mix_FadeOutChannel(rch, fout);
		SDL_Delay(fout);
		tinfo->channel->finish();
	}
	free(tinfo);
	pthread_join(tinfo->id, NULL);
	return NULL;
}

Channel::Channel(const int _id) : id(_id), sound(NULL), volume(0)
{
	setVolume(10 + rand() % 80);
}
Channel::~Channel() {}

void Channel::setVolume(const int val)
{
	//	cout << "volume: " << val << " / " << (MIX_MAX_VOLUME * max(0, min(val, 100))) / 100 << endl;
	volume = max(0, min(val, 100));
}

const int Channel::getVolume() const
{
	return volume;
}

bool Channel::play(Sound *s, int count)
{
	if (s == NULL)
		return false;
	sound = s;
	struct thread_info *tinfo = (struct thread_info *)malloc(sizeof(struct thread_info));
	tinfo->channel = this;
	tinfo->sound = s;
	tinfo->id = 0;
	tinfo->count = count;
	return (pthread_create(&tinfo->id, NULL, Channel::work, (void *)tinfo) != -1);
}

void Channel::finish()
{
	//cout << "Channel " << id << " finished" << endl;
	sound->unload();
	sound = NULL;
}

int Channel::init(const int channels)
{
	Mix_Init(MIX_INIT_MP3);
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cerr << "SDL_Mixer couldnt init. " << Mix_GetError() << endl;
		return 0;
	}

	const int real = Mix_AllocateChannels(channels * 2);
	for (int i = 0; i < channels; i++)
	{
		Channel::channels.push_back(new Channel(i));
	}
	// cout << "Created " << real << " channels" << endl;
	return channels;
}

void Channel::cleanup()
{
	for (int i = 0; i < Channel::channels.size(); i++)
	{
		delete Channel::channels[i];
	}
	Channel::channels.clear();
	Sound::cleanup();
	Mix_Quit();
}

void Channel::check()
{
	//cout << "Checking channels..." << endl;
	for (int i = 0; i < Channel::channels.size(); i++)
	{
		Channel *ch = Channel::channels[i];
		if (ch->sound == NULL)
		{
			ch->play(Sound::get(), 1);// + rand() % 5);
//			break;
		}
	}
}

