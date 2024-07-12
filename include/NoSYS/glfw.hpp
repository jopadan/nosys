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
#include <MLR/mlr.hpp>
#include <sail-c++/sail-c++.h>
#include <sail-c++/image_output.h>

using namespace math;
using namespace math::sca;

namespace sys
{
	GLFWwindow*               win = nullptr; /* system window                  */
	s32                       w   =    320u; /* screen width                   */
	s32                       h   =    240u; /* screen height                  */
	f32		aspect_ratio  = (sca::f32)320/(sca::f32)240;
	f32                       z   =    1.0f; /* mouse wheel scroll zoom z-axis */

	struct
	{
		s32    idx = 1;
		std::array<s32, 2> sze = { 15, 15 };
	} font;
	struct
	{
		void set_perspective()
		{
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective( 65.0f, aspect_ratio, 0.0f, 100.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		void set_ortho2d()
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0., w, 0., h);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		void look_at()
		{
			gluLookAt(0.0f,   0.0f, 3.0f * z,
		        	  0.0f,   0.0f, 0.0f,
				  0.0f,   1.0f, 0.0f);
		}
	} cam;
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
			cam.set_ortho2d();
			glRasterPos2f(w - font.sze[0] * 4, h - font.sze[1]);
			glcRenderString(fps.data());
			cam.set_perspective();
		}
	} time;
	struct
	{
		geo::box<float> test_cube;
		void clr()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		void pre()
		{
			cam.look_at();
			vec::f32<4> pos = { 0.0f, 0.0f, 3.0f * z, 1.0f };
			vec::f32<4> col = { 1.0f, 1.0f, 1.0f, 1.0f };
			glLightfv(GL_LIGHT0, GL_POSITION, (sca::f32*)&pos);
			glLightfv(GL_LIGHT0, GL_AMBIENT, (sca::f32*)&pos);
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
		void post()
		{
		}
		void draw()
		{
			pre();
			post();
		}
		void draw_test()
		{
			pre();
			glRotatef((f32)time.now * 100.0f, 1.0f, 0.0f, 1.0f);
			test_cube.draw();
			post();
		}
	} view;

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
		w = width;
		h = height;
		aspect_ratio = (sca::f32)w / (sca::f32)h;
		cam.set_perspective();
		printf("Resolution changed: %dx%d\n", w, h);
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
		size(win, width, height);
		glcContext(glcGenContext());
		glcScale(font.sze[0], font.sze[1]);
		glcFont(font.idx);
		glcNewFontFromFamily(1, "Times");
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
		if (glfwGetKey(win, GLFW_KEY_UP))
			zoom(win, 0, 1.0/128);
		else if (glfwGetKey(win, GLFW_KEY_DOWN))
			zoom(win, 0, -1.0/128);
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
