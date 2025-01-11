#include <array>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <experimental/simd>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/personality.h>
#include <sys/time.h>
#include <signal.h>

void halt()
{
	printf("ERROR\n");
	exit(EXIT_FAILURE);
}

namespace frame_timer
{
	timer_t timerid;
	sigset_t mask;
	int64_t freq = 1 << 10;
	struct sigevent sev;
	struct sigaction sa;
	struct itimerspec its = { .it_interval = {1,0}, .it_value = {1,0} };
	uint64_t frames = 0;
	uint64_t fps    = 0;

	void callback(int sig, siginfo_t* si, void* uc)
	{
		fps    = frames;
		frames = 0;
		printf("%zu\n", frame_timer::fps);
	}
	bool start()
	{
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = callback;
		sigemptyset(&sa.sa_mask);

		if(sigaction(SIGRTMIN, &sa, nullptr) == -1)
			return false;

		sigemptyset(&mask);
		sigaddset(&mask, SIGRTMIN);

		if(sigprocmask(SIG_SETMASK, &mask, nullptr) == -1)
			return false;

		sev.sigev_notify = SIGEV_SIGNAL;
		sev.sigev_signo  = SIGRTMIN;
		sev.sigev_value.sival_ptr = &timerid;

		if(timer_create(CLOCK_REALTIME,&sev,&timerid) == -1 || timer_settime(timerid, 0, &its, nullptr) == -1)
			return false;
		return true;
	}
	bool update()
	{
		if(sigprocmask(SIG_UNBLOCK, &mask, nullptr) != -1)
		{
			frames++;
			return true;
		}
		return false;
	}
};

int main(int argc, char** argv)
{
	sys::time::start();
	while(sys::time::update())
	{
	}
	exit(EXIT_SUCCESS);
}
