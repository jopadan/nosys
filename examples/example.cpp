#include "NoSYS/glfw.hpp"

int main(int argc, char** argv)
{
	sys::init();
	nma::geo::box<sca::f32> cube;
	while(sys::tick())
	{
		sys::sclr();

		glRotatef((f32)sys::time.now * 100.0f, 1.0f, 0.0f, 1.0f);
		cube.draw();
		sys::time.draw_fps();

		sys::swap();
	}
	sys::halt();
	exit(EXIT_SUCCESS);
}
