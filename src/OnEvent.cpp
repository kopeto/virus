#include "Sim.h"


void Sim::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}

void Sim::OnExit() {
  Running = false;
}

void Sim::OnPause() {
  Pause = !Pause;
}

void Sim::OnLButtonDown(int mX, int mY) {
  return ;
}

void Sim::OnRButtonDown(int mX, int mY) {
  return ;
}

void Sim::OnKeyDown(SDL_Keycode sym, Uint16 mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_LEFT: {
            break;
        }

        case SDLK_RIGHT: {
            break;
        }

        case SDLK_UP: {
            break;
        }

        case SDLK_DOWN: {
            break;
        }

        case SDLK_KP_PLUS: {
          break;
        }

        case SDLK_KP_MINUS: {
          break;
        }

        case SDLK_SPACE: {
          OnPause();
          break;
        }
        case SDLK_p: {
          OnPause();
          break;
        }

        case SDLK_q: {
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "Exit.\n"
          );
          OnExit();
          break;
        }
        case SDLK_ESCAPE: {
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "Exit.\n"
          );
          OnExit();
          break;
        }

        default: {
          break;
        }
    }
}

void Sim::OnKeyUp(SDL_Keycode sym, Uint16 mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_LEFT: {
            break;
        }

        case SDLK_RIGHT: {
            break;
        }

        case SDLK_w: {
            break;
        }
        default: {
        }
    }
}

void Sim::OnMouseWheel(bool Up, bool Down) {
}
