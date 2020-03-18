#include "Sim.h"

int Sim::OnExecute() {
    if(OnInit() == false) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in Program initialization.\n");
      return -1;
    }
    else
    {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Program Initiated.\n");
    }

    SDL_Event Event;

    for(day=0; day<=DAYS && Running; day++)
    {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        OnLoop();
        OnRender();
        while(Pause && Running){
          while(SDL_PollEvent(&Event)) {
              OnEvent(&Event);
          }
        }
        old_infections = infections;
    }

    if(CreatePNGSequence && CreateMP4Video && Running)
    {
      RenderMP4Video();
      Running=0;
    }

    while(Running)
    {
      while(SDL_PollEvent(&Event)) {
          OnEvent(&Event);
      }
    }

    OnCleanup();

    return 0;
}
