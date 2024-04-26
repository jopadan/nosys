#pragma once

#include <array>
#include <vector>
#include <string>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glu.h>
#include <GL/glc.h>
#include <NMA/nma.hpp>
#include <iostream>
#include <sail-c++/sail-c++.h>
#include <sail-c++/image_output.h>

using namespace nma;
using namespace nma::sca;

namespace sys
{
	GLFWwindow*               win = nullptr; /* system window                  */
	s32                       w   =    320u; /* screen width                   */
	s32                       h   =    240u; /* screen height                  */
	f32                       z   =    1.0f; /* mouse wheel scroll zoom z-axis */

	struct
	{
		s32    idx = 1;
		std::array<s32, 2> sze = { 15, 15 };
	} font;
	struct
	{
		f64 now;
		f64 last;
		f64 delta;
		u64 frames;
		std::array<char, 8> fps;
		inline f64 get() { return glfwGetTime(); }

		bool tick()
		{
			now = get();
			frames++;
			delta = now - last;
			if(delta >= 1.0)
			{
				fps = { '\0' };
				snprintf(fps.data(), 8, "%3hufps", (u16)frames);
				frames = 0;
				last = now;
			}
			return true;
		}
		void draw_fps()
		{
			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0., sys::w, 0., sys::h);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(sys::w - 5 * sys::font.sze[0], sys::h - sys::font.sze[1]);
			glcFont(sys::font.idx);
			glcRenderString(fps.data());
			glPopMatrix();
		}
	} time;

	bool grab()
	{
		std::vector<col::u8<4>> pixels(w * h);
		GLint pack_alignment;
		glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glReadBuffer(GL_FRONT);
		glFlush();
		glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, &pixels.front());
		glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment);

		/* flip vertically */
		for(sca::s32 row = 0; row < h >> 2; row++)
		{
			for(sca::s32 col = 0; col < w; col++)
			{ 
				col::u8<4> tmp = pixels[row * w + col];
				pixels[row * w + col] = pixels[(h - row - 1) * w + col];
				pixels[(h - row - 1) * w + col] = tmp;
			}
		}
		sail::image image(pixels.data(), SAIL_PIXEL_FORMAT_BPP32_RGBA, w, h, w * sizeof(pixels[0]));
		sail::image_output image_output("screenshot.png");
		SAIL_TRY(image_output.next_frame(image));
		SAIL_TRY(image_output.finish());
		return true;
	}


	void zoom(GLFWwindow* window, f64 x, f64 y)
	{
		z += (f32)y / 8.0f;
		if(z < 0) z = 0;
	}
	void keys(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		if(key == GLFW_KEY_F12 && action == GLFW_PRESS)
			grab();
	}
	void size(GLFWwindow* window, int w, int h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( 65.0f, (f32)w/(f32)h, 0.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0f,   0.0f, 3.0f * z,
		          0.0f,   0.0f, 0.0f,
		          0.0f,   1.0f, 0.0f);
		vec::f32<4> pos = { 0.0f, 0.0f, 3.0f * z, 1.0f };
		vec::f32<4> col = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, (sca::f32*)&pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, (sca::f32*)&col);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_DENSITY, 0.1f);
		glFogfv(GL_FOG_COLOR, (sca::f32*)&col);
	}

	void sclr()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool init(int width = w, int height = h, const char* title = "sys::glfw")
	{
		const char* description;

		if (!glfwInit())
		{
			glfwGetError(&description);
			printf("Error: %s\n", description);
			return false;
		}

		win = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!win)
		{
			glfwGetError(&description);
			printf("Error: %s\n", description);
			glfwTerminate();
			return false;
		}
		glfwSetInputMode(win, GLFW_STICKY_KEYS, GLFW_TRUE);
		/* set GLFW callbacks */ 
		glfwSetFramebufferSizeCallback(win, size);
		glfwSetScrollCallback(win, zoom);
		glfwSetKeyCallback(win, keys);
		glfwMakeContextCurrent(win);
		gladLoadGL(glfwGetProcAddress);
		glfwGetFramebufferSize(win, &w, &h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		size(win, width, height);
		glcContext(glcGenContext());
		glcScale(font.sze[0], font.sze[1]);
		glcNewFontFromFamily(1, "Helvetica");
		glcNewFontFromFamily(2, "Courier");
		glcNewFontFromFamily(3, "Times");
		glcFont(font.idx);
		return true;
	}

	void halt()
	{
		if(win)
			glfwDestroyWindow(win);
		glfwTerminate();
	}

	bool tick()
	{
		time.tick();
		glfwMakeContextCurrent(win);
		glfwGetWindowSize(win, &w, &h);
		size(win, w,h);

		if (glfwWindowShouldClose(win) || glfwGetKey(win, GLFW_KEY_ESCAPE))
			return false;
		return true;
	}

	void swap()
	{
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}
