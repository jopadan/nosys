#include "nosys.hpp"

int main(int argc, char** argv)
{
	sys::init();

	while(sys::info::running)
	{
		sys::step();
		sys::video::view::console::draw();
	}

	sys::halt();

	exit(EXIT_SUCCESS);
}
