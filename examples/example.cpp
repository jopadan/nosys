#include "NoSYS/glfw.hpp"

col::u16<5,6,5> bfcolor = { 12, 23, 26 };
col::u8<4>      vccolor = (col::u8<4>)bfcolor;


int main(int argc, char** argv)
{
	sys::init();

	std::cout << sys::view.test_cube;

	while(sys::tick())
	{
		sys::view.clr();
		sys::view.draw_test();

		sys::time.draw_fps();
		sys::view.swap();
	}
	sys::halt();
	exit(EXIT_SUCCESS);
}
