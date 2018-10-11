
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
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {

      Keyboard(); 

    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{

  switch(m_event.type)
  {
    case SDL_QUIT:
      m_running = false;
      break;
    case SDL_KEYDOWN:
      switch(m_event.key.keysym.sym)
      {
        case SDLK_a:
          m_graphics->changeERotation();
          break;
        case SDLK_s:
          m_graphics->changeSRotation();
          break;
        case SDLK_d:
          m_graphics->changeMRotation();
          break;
        case SDLK_ESCAPE:
          m_running = false;
          break;
        case SDLK_f:
          m_graphics->changeMOrbit();
          break;
        case SDLK_RIGHT:
          m_graphics->changeMOrbit();
          m_graphics->changeEOrbit();
          break;
        case SDLK_LEFT:
          m_graphics->changeMRotation();
          m_graphics->changeERotation();
          m_graphics->changeSRotation();
          break;
        case SDLK_UP:
          m_graphics->changeMoonOrbit();
          break;
        case SDLK_DOWN:
          m_graphics->changeMoonRotation();
          break;
    }
    case SDL_MOUSEBUTTONDOWN:
      if (m_event.button.button == SDL_BUTTON_LEFT)
      {
        m_graphics->changeEOrbit();
      }
      break;
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