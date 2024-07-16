#include "NoSYS/glfw.hpp"

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
