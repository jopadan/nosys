#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/personality.h>
#include <sys/time.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glc.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glu.h>

typedef  GLubyte pixel[4];

struct sys_t
{
	const char* error;
	const char* title;
	GLFWwindow* win;
	int w;
	int h;
	int aspect_ratio;
	int bpp;
	GLdouble cursor_x;
	GLdouble cursor_y;
	GLfloat  alpha;
	GLfloat  beta;
	GLfloat  zoom;
	struct
	{
		struct {
			clock_t clock;
			uint64_t freq;
			struct timespec time;
			timer_t id;
			sigset_t mask;
			struct itimerspec its;
			uint64_t frames;
			uint64_t fps;
		} timer;
		struct utsname arch;
		int pers;
		uint16_t bits;
		bool running;
		char* sys;
		char* date;
		char* fps;
		char* simd;
	} info;
	struct
	{
		int id;
		int w;
		int h;
	} font;
	struct
	{
		const char* dir;
		const char* name;
		size_t number;
	} screenshot;
};	

struct sys_t* sys = NULL;

extern inline bool sys_make(int width, int height, const char* title)
{
	sys = calloc(1, sizeof(struct sys_t));
	if(sys != NULL)
	{
		sys->w = width;
		sys->h = height;
		sys->aspect_ratio = sys->h > 0 ? (float)sys->w/(float)sys->h : 1.f;
		sys->title = title;
		sys->info.timer.clock  = CLOCK_REALTIME;
		sys->info.timer.freq   = 1 << 10;
		sys->info.timer.fps    = 0;
		sys->info.timer.frames = 0;
		sys->alpha             = 210.0f;
		sys->beta              = -70.0f;
		sys->zoom              =   2.0f;
		sys->font.id           = 1;
		sys->font.w            = 10;
		sys->font.h            = 10;
		sys->screenshot.name   = "screenshot.raw";
		sys->info.simd         = "sse4_2";
		return true;
	}
	return false;
}

extern inline bool sys_free(struct sys_t* s)
{
	if(s != NULL)
	{
		if(s->info.sys != NULL)
		{
			free(s->info.sys);
			s->info.sys = NULL;
		}
		if(s->info.fps != NULL)
		{
			free(s->info.fps);
			s->info.fps = NULL;
		}
		free(s);
		s = NULL;
		return true;
	}
	return false;
}

extern inline bool sys_date(struct sys_t* s)
{
	clock_gettime(s->info.timer.clock, &s->info.timer.time);
	s->info.date = ctime(&s->info.timer.time.tv_sec);
	s->info.date[strlen(s->info.date) - 1] = '\0';
	return true;
}

extern inline void sys_perspective()
{
	glViewport(0,0,sys->w,sys->h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.f, sys->aspect_ratio, 0.5f, 1024.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
}

extern inline void sys_ortho2d()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., sys->w, 0., sys->h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3.0);
	glColor3f(1.0f, 1.0f, 1.0f);
}

extern inline void sys_look_at()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, sys->zoom,
	          0.0f, 0.0f, 0.0f,
	          0.0f, 1.0f, 0.0f);
}

extern inline void sys_font_pre()
{
	sys_ortho2d();
	glPushMatrix();
}
extern inline void sys_font_post()
{
	glPopMatrix();
	sys_perspective();	
}

extern inline void sys_font(const char* msg, int x, int y)
{
	glRasterPos2f(x,y);
	glcRenderString(msg);
}

extern inline void sys_scroll(GLFWwindow* window, GLdouble _x, GLdouble _y)
{
	sys->zoom += (GLfloat) _y / 4.0f;
	if(sys->zoom < 1.25f) sys->zoom = 1.25f;
	printf("zoom: %f\n", sys->zoom);
}

extern inline void sys_mouse(GLFWwindow* window, int button, int action, int mods)
{
	if(button != GLFW_MOUSE_BUTTON_LEFT)
		return;
	if(action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &sys->cursor_x, &sys->cursor_y);
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

extern inline void sys_cursor(GLFWwindow* window, double x, double y)
{
	if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		sys->alpha += (GLfloat)(x-sys->cursor_x) / 10.0f;
		sys->beta  += (GLfloat)(y-sys->cursor_y) / 10.0f;
		sys->cursor_x = x;
		sys->cursor_y = y;
	}
}

extern inline char sys_tui_spinner()
{
	static size_t i = 0;
	static char rotor[4] = "\\|/-";
	return rotor[i++ % 4];
}

extern inline bool sys_screenshot(struct sys_t* s)
{
	size_t count = s->w * s->h;
	GLubyte pixels[count * sizeof(GLubyte) * 4];
	GLint pack_alignment;
	glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, s->w, s->h, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment);
	FILE* of = fopen(s->screenshot.name, "wb");

	size_t i = 0;
	size_t written = 0;
	size_t block_size = 32;
	while(written < count * 4)
	{
		printf("\rWriting %s: %c", s->screenshot.name, sys_tui_spinner());
		ssize_t out = fwrite(pixels, 1, block_size, of);
		if(out <= 0)
			break;
		written += out;
	}
	fclose(of);
	printf("\rWriting %s: %s", s->screenshot.name, written == count * 4 ? "Finished!\n" : "Failed!\n");

	return true;
}

extern inline void sys_keys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("key: %s action: %d mods: %X\n", glfwGetKeyName(key, scancode), action, mods);

	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch(key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_F12:
				sys_screenshot(sys);
				break;
			case GLFW_KEY_UP:
				sys->beta -= 5;
				printf("beta: %f\n", sys->beta);
				break;
			case GLFW_KEY_DOWN:
				sys->beta += 5;
				printf("beta: %f\n", sys->beta);
				break;
			case GLFW_KEY_LEFT:
				sys->alpha += 5;
				printf("alpha: %f\n", sys->alpha);
				break;
			case GLFW_KEY_RIGHT:
				sys->alpha -= 5;
				printf("alpha: %f\n", sys->alpha);
				break;
			case GLFW_KEY_PAGE_UP:
				sys->zoom -= 0.25f;
				if(sys->zoom <= 1.25f)
					sys->zoom = 1.25f;
				printf("zoom: %f\n", sys->zoom);
				break;
			case GLFW_KEY_PAGE_DOWN:
				sys->zoom += 0.25f;
				printf("zoom: %f\n", sys->zoom);
				break;
			default:
				break;
		}
	}
}
extern inline void sys_size(GLFWwindow* window, int width, int height)
{
	sys->w = width;
	sys->h = height;
	sys->aspect_ratio = sys->h > 0 ? (float)sys->w / (float)sys->h : 1.f;
	sys_perspective();
	printf("Resolution changed: %dx%d\n", sys->w, sys->h);
}

extern inline void sys_fps(int sig, siginfo_t* si, void* uc)
{
	sys->info.timer.fps = sys->info.timer.frames;
	sys->info.timer.frames = 0;
	sys_date(sys);
	asprintf(&sys->info.fps, "FPS: %zu", sys->info.timer.fps);
}

extern inline bool sys_info(struct sys_t* s)
{
	uname(&s->info.arch);
	s->info.pers = personality(-1);
	s->info.bits = sysconf(s->info.pers & PER_LINUX32 ? _SC_WORD_BIT : _SC_LONG_BIT);

	if(asprintf(&s->info.sys, "%s-%s %s %hu-bit %s\0", s->info.arch.sysname, s->info.arch.release, s->info.arch.machine, s->info.bits, s->info.simd) == -1)
		return false;

	s->info.timer.clock = CLOCK_REALTIME;
	sys_date(s);

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sys_fps;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGRTMIN, &sa, NULL) == -1 || sigemptyset(&s->info.timer.mask) == -1 || sigaddset(&s->info.timer.mask, SIGRTMIN) == -1 || sigprocmask(SIG_SETMASK, &s->info.timer.mask, NULL) == -1)
		return false;

	struct sigevent sev;
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIGRTMIN;
	sev.sigev_value.sival_ptr = &s->win;

	s->info.timer.its.it_interval.tv_sec = 1;
	s->info.timer.its.it_interval.tv_nsec = 0;
	s->info.timer.its.it_value.tv_sec  = 1;
	s->info.timer.its.it_value.tv_nsec = 0;

	if(timer_create(s->info.timer.clock,&sev,&s->info.timer.id) == -1 || timer_settime(s->info.timer.id, 0, &s->info.timer.its, NULL) == -1)
		return false;
	return true;
}

extern inline bool sys_init(struct sys_t* s)
{
	if(!glfwInit())
	{
		glfwGetError(&s->error);
		printf("Error: %s\n", s->error);
		exit(EXIT_FAILURE);
	}
	s->win = glfwCreateWindow(sys->w, sys->h, sys->title, NULL, NULL);
	if(!s->win)
	{
		glfwGetError(&s->error);
		printf("Error: %s\n", s->error);
		glfwTerminate();
	}

	glfwSetInputMode(s->win, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetFramebufferSizeCallback(s->win, sys_size);
	glfwGetFramebufferSize(s->win, &s->w, &s->h);
	glfwSetScrollCallback(s->win, sys_scroll);
	glfwSetMouseButtonCallback(s->win, sys_mouse);
	glfwSetCursorPosCallback(s->win, sys_cursor);
	glfwSetKeyCallback(s->win, sys_keys);
	glfwMakeContextCurrent(s->win);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(0);
	glcContext(glcGenContext());
	glcScale(s->font.w,s->font.h);
	glcNewFontFromFamily(s->font.id,"Helvetica");
	glcFont(s->font.id);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	sys_size(s->win, s->w, s->h);
	return sys_info(s);
}

extern inline bool sys_halt(struct sys_t* s)
{
	glfwTerminate();
	return true;
}

extern inline bool sys_swap(struct sys_t* s)
{
	glfwSwapBuffers(s->win);
	glfwPollEvents();
	if(sigprocmask(SIG_UNBLOCK, &s->info.timer.mask, NULL) != -1)
		s->info.timer.frames++;
	return !glfwWindowShouldClose(s->win);
}

extern inline bool sys_clr(struct sys_t* s)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}

extern inline bool sys_draw_pre(struct sys_t* s)
{
	sys_look_at();
	GLfloat pos[4] = { 0.0f, 0.0f, 3.0f, 1.0f };
	GLfloat col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, pos);
	glEnable(GL_LIGHT0);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);
	glFogfv(GL_FOG_COLOR, col);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
	glCullFace(GL_NONE);
	glDisable(GL_CULL_FACE);
	return true;
}

extern inline bool sys_draw_post(struct sys_t* s)
{
	
	sys_font_pre();
	sys_font(s->info.sys, 0, s->h - 10);
	sys_font(s->info.date, 0, s->h - 20);
	sys_font(s->info.fps, 0, s->h - 30);
	sys_font_post();
	return true;
}

extern inline bool sys_draw_test(struct sys_t* s)
{
	sys_draw_pre(s);
	glPushMatrix();
	glRotatef(s->beta , 1.0f, 0.0f, 0.0f);
	glRotatef((GLfloat)glfwGetTime() * s->alpha, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glColor4f(1.0f,0.0f,0.0f,1.0f);
	glVertex4f(0.5f,-0.5f,0.0f,1.0f);
	glColor4f(0.0f,0.5f,0.0f,1.0f);
	glVertex4f(0.5f,0.5f,0.0f,1.0f);
	glVertex4f(-0.5f, 0.5f,0.0f,1.0f);
	glColor4f(0.0f,0.0f,1.0f,1.0f);
	glVertex4f(-0.5f,-0.5f,0.0f,1.0f);
	glEnd();
	glPopMatrix();
	return sys_draw_post(s);
}

extern inline bool sys_draw(struct sys_t* s)
{
	sys_draw_pre(s);
	return sys_draw_post(s);
}

