#pragma once

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

#include <MLR/scalar.hpp>

using namespace math;

namespace sys
{
	GLFWwindow*    win = nullptr;
	s32            w = 320u;
	s32            h = 240u;
	f32            aspect_ratio = (f32)w/(f32)h;
	s32            bpp = 32;

	f64 cursor_x = 0.0;
	f64 cursor_y = 0.0;
	f32 alpha    = 210.0f;
	f32 beta     = -70.0f;
	f32 zoom     =   2.0f;

	struct
	{
		void halt()
		{
			if(win)
				glfwDestroyWindow(win);
			glfwTerminate();
		}

		void perspective()
		{
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective( 65.0f, aspect_ratio, 0.5f, 1024.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_LIGHTING);
		}
		void ortho2d()
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
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0.0f,   0.0f, zoom,
		        	  0.0f,   0.0f, 0.0f,
				  0.0f,   1.0f, 0.0f);
		}

		void size(GLFWwindow* window, int width, int height)
		{
			w = width;
			h = height;
			aspect_ratio = h > 0 ? (f32)w / (f32)h : 1.f;
			perspective();
			printf("Resolution changed: %dx%d\n", w, h);
		}

		void clr()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void pre()
		{
		
			look_at();
			f32 pos[4] = { 0.0f, 0.0f, 3.0f, 1.0f };
			f32 col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
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
			glPushMatrix();
			glRotatef(beta , 1.0f, 0.0f, 0.0f);
			glRotatef((f32)info.t_now * alpha, 0.0f, 0.0f, 1.0f);
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
			//test_cube.draw();
			post();
		}
		void swap()
		{
			glfwSwapBuffers(win);
			glfwPollEvents();
		}
	} view;


	#include "glfw_keys.hpp"
	std::string arch()
	{
		const auto cpus = hwinfo::getAllCPUs();
		for(const auto& cpu : cpus)
		{
			const std::vector<std::string> flags = cpu.flags();
			fmt::print("{:<20} ", "flags:");
			size_t len = 0;
			for(const auto& flag : flags)
			{
				len += flag.size();
				if(len >= 40)
				{
					len = flag.size();
					fmt::print("\n{:<20} ", "");
				}
				fmt::print("{} ", flag);
			}
		}
		fmt::print("\n");
		return cpus[0].cpu.model();
	}
	bool init(int width = 320u, int height = 240u, int bits = 32, const char* title = "sys::glfw")
	{
		const char* description;
		arch();
		w = width;
		h = height;
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
		}
		aspect_ratio = (f32)w/(f32)h;
		bpp = bits;

		/* set GLFW callbacks */ 
		glfwSetInputMode(win, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetFramebufferSizeCallback(win, size);
		glfwSetScrollCallback(win, scroll);
		glfwSetMouseButtonCallback(win, mouse);
		glfwSetCursorPosCallback(win, cursor);
		glfwSetKeyCallback(win, keys);
		glfwGetFramebufferSize(win, &w, &h);
		glfwMakeContextCurrent(win);
		gladLoadGL(glfwGetProcAddress);
		glfwSwapInterval(0);
		/* disable vsync */
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		size(win, width, height);
		return true;
	}

	bool tick()
	{
		info.tick();

		if (glfwWindowShouldClose(win) || glfwGetKey(win, GLFW_KEY_ESCAPE))
			return false;
		return true;
	}
}
