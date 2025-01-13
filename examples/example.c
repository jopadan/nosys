#include <nosys.h>

int main(int argc, char** argv)
{
	sys_make(320,240,"sys_glfw");

	sys_init(sys);

	do
	{
	} while(sys_clr(sys) && sys_draw(sys) && sys_swap(sys));

	exit(sys_halt(sys) && sys_free(sys) ? EXIT_SUCCESS : EXIT_FAILURE);
}
