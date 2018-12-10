
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

bool Engine::Initialize(char* configFileName)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, configFileName))
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
	bool shift = false;
	bool* shiftPtr = &shift;

	mouseXPtr = &mouseX;
	mouseYPtr = &mouseY;


	m_pause = false;
 	m_running = true;

 	// SDL_SetRelativeMouseMode(SDL_TRUE);

	while(m_running)
	{

		// Update the DT
		m_DT = getDT();

		// Check the keyboard input
		while(SDL_PollEvent(&m_event) != 0)
		{
		  Keyboard(shiftPtr, mouseXPtr, mouseYPtr, m_DT);
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

void Engine::Keyboard(bool* shiftPtr, int* mouseXPtr, int* mouseYPtr, unsigned int dt)
{
	if(m_event.type == SDL_QUIT)
	{
		m_running = false;
	}
	else if (m_event.type == SDL_KEYDOWN)
	{
		// handle key down events here
		if (m_event.key.keysym.sym == SDLK_LSHIFT)
		{
			*shiftPtr = true;
		}
		if (m_event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_running = false;
		}
		else if(m_event.key.keysym.sym == SDLK_i)
		{
			m_graphics-> translateCamera(SDLK_i, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_j)
		{
			m_graphics-> translateCamera(SDLK_j, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_k)
		{
			m_graphics-> translateCamera(SDLK_k, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_l)
		{
			m_graphics-> translateCamera(SDLK_l, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_u, shiftPtr)
		{
			m_graphics-> translateCamera(SDLK_u, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_o)
		{
			m_graphics-> translateCamera(SDLK_o, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_UP)
		{
			// m_graphics->translateCamera(glm::vec3(0.0, -0.5, 0.0));
			// m_graphics->adjustGravity('Z', -2.0);
		}
		else if(m_event.key.keysym.sym == SDLK_DOWN)
		{
			//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('Z', 2.0);
		}
		else if(m_event.key.keysym.sym == SDLK_LEFT)
		{
			// m_graphics->setLeftFlip(true);
			//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('X', -2.0);
		}
		else if(m_event.key.keysym.sym == SDLK_RIGHT)
		{
			// m_graphics->setRightFlip(true);
			//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('X', 2.0);
		}
		else if(m_event.key.keysym.sym == SDLK_w)
		{
			// reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(0.0, 0.0, -0.5));
		}
		else if(m_event.key.keysym.sym == SDLK_a)
		{
			//reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(-0.5, 0.0, 0.0));
			// m_graphics -> cycleObjectSelection();
		}
		else if(m_event.key.keysym.sym == SDLK_s)
		{
			//reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(0.0, 0.0, 0.5));
			// m_graphics -> adjustObjectSpecularBrightness();
		}
		else if(m_event.key.keysym.sym == SDLK_d)
		{
			//reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(0.5, 0.0, 0.0));
			// m_graphics -> adjustObjectDiffuseBrightness();
		}
		else if(m_event.key.keysym.sym == SDLK_f)
		{
			//reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(0.5, 0.0, 0.0));
			// m_graphics -> adjustObjectShininess();
		}
		else if(m_event.key.keysym.sym == SDLK_t)
		{
			m_graphics -> toggleShader();
		}
		else if(m_event.key.keysym.sym == SDLK_b)
		{
			// m_graphics -> adjustAmbientLight();
		}
		else if(m_event.key.keysym.sym == SDLK_n)
        {
            // m_graphics -> newGame();
        }
	}
	else if (m_event.type == SDL_KEYUP)
	{
		// handle key down events here
		if (m_event.key.keysym.sym == SDLK_LSHIFT)
		{
			*shiftPtr = false;
		}
		if (m_event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_running = false;
		}
		else if(m_event.key.keysym.sym == SDLK_UP)
		{
			//m_graphics->translateCamera(glm::vec3(0.0, -0.5, 0.0));
			// m_graphics->adjustGravity('Z', 2.0);
		}
		else if(m_event.key.keysym.sym == SDLK_DOWN)
		{
			//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('Z', -2.0);
			// m_graphics->applyImpulse();
		}
		// else if(m_event.key.keysym.sym == SDLK_LEFT)
		// {

		// 	//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
		// 	m_graphics->adjustGravity('X', 2.0);
		// }
		// else if(m_event.key.keysym.sym == SDLK_RIGHT)
		// {
		// 	//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
		// 	m_graphics->adjustGravity('X', -2.0);
		// }
		else if(m_event.key.keysym.sym == SDLK_RIGHT)
		{
			// m_graphics -> setRightFlip(false);
			//m_graphics->applyPaddleImpulseRight();
		}
		else if(m_event.key.keysym.sym == SDLK_LEFT)
		{
			// m_graphics -> setLeftFlip(false);
			//m_graphics->applyPaddleImpulseLeft();
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

		m_graphics -> rotateCamera(*mouseXPtr, *mouseYPtr);
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
