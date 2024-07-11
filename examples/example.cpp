#include "NoSYS/glfw.hpp"

col::u16<5,6,5> bfcolor = { 12, 23, 26 };
col::u8<4>      vccolor = (col::u8<4>)bfcolor;

int main(int argc, char** argv)
{
	sys::init();
	while(sys::tick())
	{
		sys::sclr();

		glRotatef((f32)sys::time.now * 100.0f, 1.0f, 0.0f, 1.0f);
//		cube.draw();
		sys::time.draw_fps();
		sys::swap();
	}
	sys::halt();
	exit(EXIT_SUCCESS);
}
