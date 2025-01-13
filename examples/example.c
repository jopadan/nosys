#include <nosys.h>

bool sys_draw_test(struct sys_t* s)
{
	sys_draw_pre();
	glPushMatrix();
	glRotatef(s->beta , 1.0f, 0.0f, 0.0f);
	glRotatef((GLfloat)glfwGetTime() * s->alpha, 0.0f, 0.0f, 1.0f);
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
	sys_draw_post();
	printf("\r%s %s %s", s->info.date, s->info.sys, s->info.fps);
	return true;
}

int main(int argc, char** argv)
{
	sys_make(320,240,"sys_glfw");

	sys_init(sys);

	do
	{
	} while(sys_clr(sys) && sys_draw_test(sys) && sys_swap(sys));

	exit(sys_halt(sys) && sys_free(sys) ? EXIT_SUCCESS : EXIT_FAILURE);
}
