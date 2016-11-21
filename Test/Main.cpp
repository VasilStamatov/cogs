#include <cogs\Window.h>

int main(int argc, char** argv)
{
		cogs::Window window;
		window.create("Test", 800, 600, cogs::WindowCreationFlags::RESIZABLE);
		int frames = 0;
		while (frames++ < 10000)
		{

		}
		window.close();
  return 0;
}