#pragma once

namespace video
{
	int w = 320;
	int h = 240;
	int bpp = 32;
	struct window
	{
	};
	std::vector<window> win;
	bool halt();
	namespace view
	{
		bool ortho2d();
		bool persp();
		bool lookat();
		namespace hud
		{
		};
		namespace menu
		{
		};
		namespace console
		{
			bool enable_stdout = true;
			bool draw()
			{
				if(enable_stdout)
					return info::draw();
				return false;
			}	
		};
		namespace sgc
		{
			struct scene
			{
				bool prel() { return true; }
				bool draw() { return true; }
				bool post() { return true; }
			};
			std::vector<struct scene> scenes;
			bool draw()
			{
				for(size_t i = 0; i < scenes.size(); i++)
				{
					scenes[i].prel();
					scenes[i].draw();
					scenes[i].post();
				}
				return true;
			}
		};
	};
	bool init(int width = 320, int height = 240, const char* title = "sys::glfw")
	{
		return true;
	}
	bool halt()
	{
		return true;
	}
	bool size();
	bool step() { return true; }

	bool prel();
	bool draw();
	bool post();
};
