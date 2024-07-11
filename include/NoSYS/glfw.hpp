#pragma once

#include <array>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glu.h>
#include <GL/glc.h>
#include <NMA/nma.hpp>
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
			gluOrtho2D(0., w, 0., h);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(w - font.sze[0] * 4, h - font.sze[1]);
			glcRenderString(fps.data());
			glPopMatrix();
		}
	} time;

	bool grab(std::filesystem::path filepath = "screenshot.png")
	{
		std::vector<col::u8<3>> pixels(w * h);
		GLint pack_alignment;
		glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixels.front());
		glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment);

		sail::image image(pixels.data(), SAIL_PIXEL_FORMAT_BPP24_RGB, w, h, w * sizeof(pixels[0]));
		/* flip vertically */
		image.mirror(SAIL_ORIENTATION_MIRRORED_VERTICALLY);
		/* write image to filepath */
		sail::image_output image_output(filepath);
		SAIL_TRY(image_output.next_frame(image));
		SAIL_TRY(image_output.finish());
		return true;
	}

	#include "glfw_keys.hpp"


	void zoom(GLFWwindow* window, f64 x, f64 y)
	{
		z += (f32)y / 8.0f;
		if(z < 0) z = 0;
	}

	void size(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		w = width;
		h = height;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( 65.0f, (sca::f32)w/(sca::f32)h, 0.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0f,   0.0f, 3.0f * z,
		          0.0f,   0.0f, 0.0f,
		          0.0f,   1.0f, 0.0f);
		vec::f32<4> pos = { 0.0f, 0.0f, 3.0f * z, 1.0f };
		vec::f32<4> col = { 1.0f, 1.0f, 1.0f, 1.0f };
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		printf("Resolution changed: %dx%d\n", w, h);
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
		/* disable vsync */
		glfwSwapInterval(0);
		gladLoadGL(glfwGetProcAddress);
		glfwGetFramebufferSize(win, &w, &h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glcContext(glcGenContext());
		glcScale(font.sze[0], font.sze[1]);
		glcFont(font.idx);
		glcNewFontFromFamily(1, "Times");
		size(win, width, height);
		return true;
	}

	void halt()
	{
		glcDeleteFont(font.idx);
		glcDeleteContext(glcGetCurrentContext());
		if(win)
			glfwDestroyWindow(win);
		glfwTerminate();
	}

	bool tick()
	{
		time.tick();
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
