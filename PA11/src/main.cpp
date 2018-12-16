#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
	char filepath[] = "objects.txt";
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Pinball", 1600, 1200);
  // if(!engine->Initialize(argv[1]))
  if(!engine->Initialize(filepath))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
