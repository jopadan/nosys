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

using namespace nma;
using namespace nma::sca;

namespace sys
{
        GLFWwindow*               win = nullptr; /* system window                  */
        s32                       w   =    640u; /* screen width                   */
        s32                       h   =    480u; /* screen height                  */
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
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(sys::w - 5 * sys::font.sze[0], sys::h - sys::font.sze[1]);
			glcFont(sys::font.idx);
			glcRenderString(fps.data());
			glPopMatrix();
		}
        } time;

        void zoom(GLFWwindow* window, f64 x, f64 y)
        {
                z += (f32)y / 8.0f;
                if(z < 0) z = 0;
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
        }

        void sclr()
        {
                glClear(GL_COLOR_BUFFER_BIT);
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
