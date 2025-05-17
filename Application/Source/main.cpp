#include "../Source/Windows/Application.h"

int main()
{
	using namespace Psycho;

	Application window;

	if (window.Initialize())
	{
		while (window.IsRunning())
		{
			window.Update();
		}
	}
}