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
		Channel::check();
		usleep(1 * 1000 * 1000);
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

	video.run();
	running = false;

	pthread_join(thread, NULL);

	return 0;
}