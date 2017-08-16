#pragma once

#include <assert.h>
class Window;
class WindowLocator
{
public:
	static void provide(Window& window)
	{
		m_window = &window;
	}

	static Window& getWindow()
	{
		assert(m_window);
		return *m_window;
	}

private:
	static Window* m_window;
};