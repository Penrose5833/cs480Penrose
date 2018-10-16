
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
	int mouseX = 0;
	int mouseY = 0;
	int* mouseXPtr;
	int* mouseYPtr;

	mouseXPtr = &mouseX;
	mouseYPtr = &mouseY;


	m_pause = false;
 	m_running = true;

 	SDL_SetRelativeMouseMode(SDL_TRUE);

	while(m_running)
	{
		// Update the DT
		m_DT = getDT();

		// Check the keyboard input
		while(SDL_PollEvent(&m_event) != 0)
		{
		  Keyboard(mouseXPtr, mouseYPtr, m_DT);
		}

		// Update and render the graphics
		if(!m_pause)
			m_graphics->Update(m_DT);
		else
			m_graphics->Update(0);

		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();
	}
}

void Engine::Keyboard(int* mouseXPtr, int* mouseYPtr, unsigned int dt)
{
	if(m_event.type == SDL_QUIT)
	{
		m_running = false;
	}
	else if (m_event.type == SDL_KEYDOWN)
	{
		// handle key down events here
		if (m_event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_running = false;
		}
		else if(m_event.key.keysym.sym == SDLK_w)
		{
			m_graphics-> translateCamera(SDLK_w, dt);
		}
		else if(m_event.key.keysym.sym == SDLK_a)
		{
			m_graphics-> translateCamera(SDLK_a, dt);
		}
		else if(m_event.key.keysym.sym == SDLK_s)
		{
			m_graphics-> translateCamera(SDLK_s, dt);
		}
		else if(m_event.key.keysym.sym == SDLK_d)
		{
			m_graphics-> translateCamera(SDLK_d, dt);
		}
		else if(m_event.key.keysym.sym == SDLK_UP)
		{
			m_graphics->changeSpeed(1);
		}
		else if(m_event.key.keysym.sym == SDLK_DOWN)
		{
			m_graphics->changeSpeed(-1);
		}

		else if(m_event.key.keysym.sym == SDLK_p)
		{
			m_graphics->renderPath(-1);
		}

	}
	// else if (m_event.type == SDL_MOUSEBUTTONDOWN)
	// {
	// 	//pause motion
	// 	m_pause = !m_pause;
	// }
	else if (m_event.type == SDL_MOUSEMOTION)
	{
		// cout << "lastX: " << *mouseXPtr << " lastY: " << *mouseYPtr << endl; 
		SDL_GetRelativeMouseState(mouseXPtr, mouseYPtr);
		// cout << "newX: " << *mouseXPtr << " newY: " << *mouseYPtr << endl;

		m_graphics -> rotateCamera(*mouseXPtr, *mouseYPtr, dt);
	}
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
