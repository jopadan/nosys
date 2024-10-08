# nosys

Platform independent framework based on:

- [GLFW] platform abstraction framework
- [QuesoGLC] opengl font library
- [SAIL] image load/save library
- [MLR] linear algebra math library

![screenshot](assets/images/screenshot.png "screenshot")

# Usage

```c++

#include "NoSYS/glfw.hpp"

col::u16<col::rgb, 5,6,5> bfcolor = { 12, 23, 26 };
col::u8<4, col::rgba>     vccolor = (col::u8<4, col::rgba>)bfcolor;


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
```

# Links
- [MLR]
- [GLFW]
- [QuesoGLC]
- [SAIL]

[MLR]: https://github.com/jopadan/mlr
[GLFW]: https://github.com/glfw/glfw
[QuesoGLC]: https://sourceforge.net/projects/quesoglc/
[SAIL]: https://github.com/HappySeaFox/sail
