#include <nosys.h>

int main(int argc, char** argv)
{
	sys_make(320,240,"sys_glfw");

	sys_init(sys);

	do
	{
	} while(sys_clr(sys) && sys_draw(sys) && sys_swap(sys));

	sys_halt(sys);
	sys_free(sys);

	exit(EXIT_SUCCESS);
}
