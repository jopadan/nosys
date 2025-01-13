#pragma once

namespace info
{
	/* arch/bits */
	struct utsname    arch;
	int               pers     = -1;
	uint64_t          bits     = 0;
	/* output string */
	char*             str      = nullptr;
	char*             date_str = nullptr;
	/* frame timer */
	time_t            t        = time(NULL);
	uint64_t          fps      = 0;
	uint64_t          frames   = 0;
	timer_t           timerid;
	/* timer signal */
	sigset_t          mask;
	struct itimerspec its      = { .it_interval = {1,0}, .it_value = {1,0} };
	bool              running  = true;

	inline constexpr const char* date()
	{
		t = time(NULL);
		date_str = ctime(&t);
		date_str[strlen(date_str) - 1] = '\0';
		return date_str;
	}

	void callback(int sig, siginfo_t* si, void* uc)
	{
		fps          = frames;
		frames       = 0;
		date();
	}

	bool init()
	{
		uname(&arch);
		pers = personality(-1);
		bits = sysconf(pers & PER_LINUX32 ? _SC_WORD_BIT : _SC_LONG_BIT);

		if(asprintf(&str, "%s-%s %s %zu-bit\0", arch.sysname, arch.release, arch.machine, bits) == -1)
			return false;

		date_str = ctime(&t);
		date_str[strlen(date_str) - 1] = '\0';

		struct sigaction  sa;
		sa.sa_flags     = SA_SIGINFO;
		sa.sa_sigaction = callback;
		sigemptyset(&sa.sa_mask);

		if(sigaction(SIGRTMIN, &sa, nullptr) == -1)
			return false;

		sigemptyset(&mask);
		sigaddset(&mask, SIGRTMIN);

		if(sigprocmask(SIG_SETMASK, &mask, nullptr) == -1)
			return false;

		struct sigevent   sev;
		sev.sigev_notify = SIGEV_SIGNAL;
		sev.sigev_signo  = SIGRTMIN;
		sev.sigev_value.sival_ptr = &sev;

		if(timer_create(CLOCK_REALTIME,&sev,&timerid) == -1 || timer_settime(timerid, 0, &its, nullptr) == -1)
			return false;
		return true;
	}
	bool step()
	{
		if(sigprocmask(SIG_UNBLOCK, &mask, nullptr) != -1)
		{
			frames++;
			return true;
		}
		return false;
	}
	bool draw()
	{
		printf("\r%s %s FPS: %-4.zu", date_str, str, fps);
		return true;
	}
	bool halt()
	{
		if(str)
			free(str);
		return true;
	}
};

