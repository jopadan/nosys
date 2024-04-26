# NoSYS

Platform independent framework based on:

- [GLFW] platform abstraction framework
- [QuesoGLC] opengl font library
- [SAIL] image load/save library
- [OpenNMA] linear algebra math library

![screenshot](assets/images/screenshot.png "screenshot")

# Usage

```c++
#include <NoSYS/glfw.hpp>

col::u16<5,6,5> bfcolor = { 12, 23, 26 };
col::u8<4>      vccolor = (col::u8<4>)bfcolor;

int main(int argc, char** argv)
{
    sys::init();
    while(sys::tick())
    {
      sys::sclr();

      /* cast RGB565 bitfield color to RGBA8888 vector color */
      glColor4ubv(&vccolor[0]);
      /* your main loop scene graph draw code here */

      sys::swap();
    }
    sys::halt();
    exit(SUCCESS);
}
```

# Links
- [OpenNMA]
- [GLFW]
- [QuesoGLC]
- [SAIL]

[OpenNMA]: https://codeberg.org/forcemaster/OpenNMA
[GLFW]: https://github.com/glfw/glfw
[QuesoGLC]: https://sourceforge.net/projects/quesoglc/
[SAIL]: https://github.com/HappySeaFox/sail
