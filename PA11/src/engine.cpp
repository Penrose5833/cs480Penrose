
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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
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
  
	//MENU
	ImGui::CreateContext();
	ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
	ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getGLContext());
	ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
	ImGui::StyleColorsDark();

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
	float strikeSpeedPercent = 0.5;

	mouseXPtr = &mouseX;
	mouseYPtr = &mouseY;


	m_pause = false;
 	m_running = true;

 	SDL_SetRelativeMouseMode(SDL_TRUE);
 	SDL_SetWindowGrab(m_window -> getSDLWindow(), SDL_TRUE);
 
 	// SDL_CaptureMouse(SDL_TRUE);

	while(m_running)
	{

		// Update the DT
		m_DT = getDT();

		// Start Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_window->getSDLWindow());
		ImGui::NewFrame();

		// Check the keyboard input
		while(SDL_PollEvent(&m_event) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&m_event); //ImGui input
		  	Keyboard(shiftPtr, mouseXPtr, mouseYPtr, m_DT);
		}



		bool menu = true; // currently does nothing but still need for "file options"
		//The Menu
		
		ImGui::Begin("Pool is for fools", &menu, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
		    if (ImGui::BeginMenu("File"))
		    {
		    	if (ImGui::MenuItem("New Game")) {/* the things for a new game */ }
		        if (ImGui::MenuItem("Exit Game", "ESC"))   { m_running = false;}
		        ImGui::EndMenu();
		    }
		    ImGui::EndMenuBar();
		}

		if (ImGui::Button("Toggle Lighting"))
		{
		    m_graphics -> toggleShader();
		}

		ImGui::SliderFloat("Strike Power", &strikeSpeedPercent, 0.1f, 1.0f);
		m_graphics -> setStrikeSpeed(strikeSpeedPercent);

		state = m_graphics -> getGameState();
		if (state != oldState)
		{
			if (state == 3 || state == 6 || state == 5)
			{
				if (player == 1)
					player = 2;
				else player = 1;
			}

			oldState = state;

		}

		ImGui::TextColored(ImVec4(.3,1,.3,1), "\nPlayer %i's Turn.", player);

		//if not time to shoot 8 ball
		ImGui::TextColored(ImVec4(1,0,0,1), "\nSelect A Pocket");
		//if time to shoot 8 ball
		//ImGui::TextColored(ImVec4(0,1,0,1), "\nSelect A Pocket");
		ImGui::BeginChild("Scrolling", ImVec2(0,0), true);//, true);
		if (ImGui::Button("Top Left"))
		{
		    //things
		}

		if (ImGui::Button("Top Right"))
		{
			
		}
		if (ImGui::Button("Middle Left"))
		{
		    //things
		}
		if (ImGui::Button("Middle Right"))
		{
		    //things
		}
		if (ImGui::Button("Bottom Left"))
		{
		    //things
		}

		if (ImGui::Button("Bottom Right"))
		{
			
		}


		
		ImGui::EndChild();

		ImGui::End();


		// Update and render the graphics

		m_graphics->Update(m_DT);
		m_graphics->Render();

		// Dear ImGui rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		

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
		if (m_event.key.keysym.sym == SDLK_q)
		{
			SDL_bool mode = SDL_GetRelativeMouseMode();
			if(mode == SDL_TRUE)
				mode = SDL_FALSE;
			else
				mode = SDL_TRUE;
			// cout << mode << endl;
			SDL_SetRelativeMouseMode(mode);
			// SDL_CaptureMouse(mode);
			SDL_SetWindowGrab(m_window -> getSDLWindow(), mode);
		}
		else if(m_event.key.keysym.sym == SDLK_i)
		{
			// m_graphics-> translateCamera(SDLK_i, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_j)
		{
			// m_graphics-> translateCamera(SDLK_j, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_k)
		{
			// m_graphics-> translateCamera(SDLK_k, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_l)
		{
			// m_graphics-> translateCamera(SDLK_l, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_u)
		{
			// m_graphics-> translateCamera(SDLK_u, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_o)
		{
			// m_graphics-> translateCamera(SDLK_o, dt, shiftPtr);
		}
		else if(m_event.key.keysym.sym == SDLK_UP)
		{
			// m_graphics->translateCamera(glm::vec3(0.0, -0.5, 0.0));
			// m_graphics->adjustGravity('Z', -2.0);
			// m_graphics->applyImpulse(btVector3(0,-.02,-0.5));
		}
		else if(m_event.key.keysym.sym == SDLK_DOWN)
		{
			// m_graphics -> increaseStrikeSpeed();
			// m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('Z', 2.0);
			// m_graphics->applyImpulse(btVector3(0,-.02,0.5));
		}
		else if(m_event.key.keysym.sym == SDLK_LEFT)
		{
			// m_graphics->setLeftFlip(true);
			// m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('X', -2.0);
			// m_graphics->applyImpulse(btVector3(-0.5,-.02,0));
		}
		else if(m_event.key.keysym.sym == SDLK_RIGHT)
		{
			// m_graphics->setRightFlip(true);
			// m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('X', 2.0);
			// m_graphics->applyImpulse(btVector3(0.5,-.02,0));
		}
		else if(m_event.key.keysym.sym == SDLK_w)
		{
			// reverse orbit on down key
			// m_graphics->reverseObjectOrbit();
			// m_graphics-> translateCamera(glm::vec3(0.0, 0.0, -0.5));
		}
		else if(m_event.key.keysym.sym == SDLK_a)
		{
			// reverse orbit on down key
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
			m_graphics->incrementSelectedBall(-1);
			// m_graphics -> adjustAmbientLight();
		}
		else if(m_event.key.keysym.sym == SDLK_n)
        {
        	m_graphics->incrementSelectedBall(1);
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
			if(m_graphics->getGameState() == 5 or m_graphics->getGameState() == 6)
			{
				m_graphics->moveBallUp();
			}
			else
			{
				// m_graphics->applyImpulse(btVector3(0,-.02,-0.5));
			}
		}
		else if(m_event.key.keysym.sym == SDLK_DOWN)
		{
			//m_graphics->translateCamera(glm::vec3(0.0, 0.5, 0.0));
			// m_graphics->adjustGravity('Z', -2.0);
			// m_graphics->applyImpulse();
			if(m_graphics->getGameState() == 5 or m_graphics->getGameState() == 6)
			{
				m_graphics->moveBallDown();
			}
			else
			{
				// m_graphics->applyImpulse(btVector3(0,-.02,.5));
			}
		}
		else if(m_event.key.keysym.sym == SDLK_RIGHT)
		{
			// m_graphics -> setRightFlip(false);
			//m_graphics->applyPaddleImpulseRight();
			if(m_graphics->getGameState() == 5 or m_graphics->getGameState() == 6)
			{
				m_graphics->moveBallRight();
			}
			else
			{
				// m_graphics->applyImpulse(btVector3(.5,-.02,0));
			}
		}
		else if(m_event.key.keysym.sym == SDLK_LEFT)
		{
			// m_graphics -> setLeftFlip(false);
			//m_graphics->applyPaddleImpulseLeft();
			if(m_graphics->getGameState() == 5 or m_graphics->getGameState() == 6)
			{
				m_graphics->moveBallLeft();
			}
			else
			{
				// m_graphics->applyImpulse(btVector3(-.5,-.02,0));
			}
		}
		else if(m_event.key.keysym.sym == SDLK_b)
		{
			m_graphics->incrementSelectedBall(-1);
			// m_graphics -> adjustAmbientLight();
		}
		else if(m_event.key.keysym.sym == SDLK_n)
        {
        	m_graphics->incrementSelectedBall(1);
            // m_graphics -> newGame();
        }
        else if(m_event.key.keysym.sym == SDLK_v)
        {
        	if(m_graphics->getGameState() == 5 or m_graphics->getGameState() == 6)
			{
				m_graphics->dropBall();
			}
        }
        else if(m_event.key.keysym.sym == SDLK_s)
        {
        	m_graphics -> setCueStrike();
        }
	}
	else if (m_event.type == SDL_MOUSEBUTTONDOWN)
	{
		// m_graphics -> increaseStrikeSpeed();
		if(SDL_GetRelativeMouseMode() == SDL_TRUE)
		{
			m_graphics -> setCueStrike();
		}
	}
	else if (m_event.type == SDL_MOUSEBUTTONUP)
	{
		// m_graphics -> setCueStrike();
	}
	else if (m_event.type == SDL_MOUSEMOTION)
	{
		// cout << "lastX: " << *mouseXPtr << " lastY: " << *mouseYPtr << endl; 
		SDL_GetRelativeMouseState(mouseXPtr, mouseYPtr);
		// cout << "newX: " << *mouseXPtr << " newY: " << *mouseYPtr << endl;
		if(SDL_GetRelativeMouseMode() == SDL_TRUE)
		{
			m_graphics -> translateCamera(*mouseXPtr, *mouseYPtr, 0, dt, shiftPtr);
		}
	}
	else if (m_event.type == SDL_MOUSEWHEEL)
	{
		// cout << "lastX: " << *mouseXPtr << " lastY: " << *mouseYPtr << endl; 
		SDL_GetRelativeMouseState(mouseXPtr, mouseYPtr);
		// cout << m_event.wheel.y << endl;
		if(SDL_GetRelativeMouseMode() == SDL_TRUE)
		{
			m_graphics -> translateCamera(0, 0, m_event.wheel.y, dt, shiftPtr);
		}
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
