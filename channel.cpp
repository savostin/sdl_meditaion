#include "channel.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

vector<Channel *> Channel::channels;

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
		int count = min(tinfo->count, int(2600 / (0.01 * tinfo->sound->length())));
		int fout = min(tinfo->count * tinfo->sound->length() * 0.2, 10000.);
		int to = tinfo->count * tinfo->sound->length() - fout;
		// cout << "[" << tinfo->channel->id << "] " << tinfo->sound->name() << " , length = " << (tinfo->sound->length() / 1000) << ", " << (count + 1) << " times, fade out in " << int(to / 1000.) << " seconds" << endl;
		if (Mix_FadeInChannel(tinfo->channel->id, tinfo->sound->data(), tinfo->count, min(10000., tinfo->sound->length() * 0.2)) != -1)
		{
			usleep(1000 * to);
			Mix_FadeOutChannel(tinfo->channel->id, fout);
		}
	}
	pthread_join(tinfo->id, NULL);
	free(tinfo);
	return NULL;
}

Channel::Channel(const int _id) : id(_id), sound(NULL), panning(0)
{
	setVolume(20 + rand() % 80);
}
Channel::~Channel() {}

void Channel::setVolume(const int val)
{
	//	cout << "volume: " << val << " / " << (MIX_MAX_VOLUME * max(0, min(val, 100))) / 100 << endl;
	Mix_Volume(id, (MIX_MAX_VOLUME * max(0, min(val, 100))) / 100);
}

void Channel::setPanning(const int val)
{
	panning = min(max(-30, val), 30);
	Mix_SetPanning(id, -255 * min(0, max(-100, panning)) / 100, 255 * max(0, min(panning, 100)) / 100);
}

const int Channel::getPanning() const
{
	return panning;
}

void Channel::changePanning()
{
	setPanning(panning + rand() % 20 - 10);
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
	// cout << "Channel " << id << " finished" << endl;
	Mix_HaltChannel(id);
	sound->unload();
	sound = NULL;
}

int Channel::init(const int channels)
{
	Mix_Init(MIX_INIT_MP3);
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		cerr << "SDL_Mixer couldnt init. " << Mix_GetError() << endl;
		return 0;
	}
	Mix_ChannelFinished(Channel::finished);

	const int real = Mix_AllocateChannels(min(channels, MIX_CHANNELS));
	for (int i = 0; i < real; i++)
	{
		Channel::channels.push_back(new Channel(i));
	}
	//cout << "Created " << real << " channels" << endl;
	return real;
}

void Channel::cleanup()
{
	Mix_ChannelFinished(NULL);
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
	// cout << "Checking channels..." << endl;
	for (int i = 0; i < Channel::channels.size(); i++)
	{
		Channel *ch = Channel::channels[i];
		if (ch->sound == NULL)
		{
			ch->play(Sound::get(), rand() % 10);
			break;
		}
	}
}

void Channel::finished(int id)
{
	Channel *ch = Channel::channels[id];
	if (ch)
	{
		ch->finish();
	}
}