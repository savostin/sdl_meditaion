#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <glob.h>
#include <pthread.h>

#include "channel.h"
#include "video.h"

using namespace std;

static vector<string> globVector(const string &pattern)
{
	glob_t glob_result;
	glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
	vector<string> files;
	for (unsigned int i = 0; i < glob_result.gl_pathc; ++i)
	{
		files.push_back(string(glob_result.gl_pathv[i]));
	}
	globfree(&glob_result);
	return files;
}

bool running = false;

static void *work(void *)
{
	srand(time(NULL));
	while (running)
	{
		usleep(1000000);
		Channel::check();
	}
	Channel::cleanup();
	exit(EXIT_SUCCESS);
	return NULL;
}

static void on_close(int signal)
{
	puts("Cleaning up...");
	running = false;
}

int main()
{
	signal(SIGINT, on_close);
	pthread_t thread;

	Channel::init(4);
	Video video(VIS_LINES);

	vector<string> files = globVector("./mp3/*.mp3");
	for (int i = 0; i < files.size(); i++)
	{
		Sound::add(files[i]);
	}
	running = true;

	pthread_create(&thread, NULL, work, NULL);
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					video.resized();
				}
				break;
			}
		}
		video.tick();
		usleep(100);
	}

	pthread_join(thread, NULL);

	return 0;
}