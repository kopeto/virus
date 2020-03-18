#include "Sim.h"

bool Sim::OnInit() {
    int WINDOW_FLAGS = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    else
    {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"SDL Initiated.\n");
    }


    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    MainFont = TTF_OpenFont("fonts/DejaVuSans.ttf", 24); //this opens a font style and sets a size
    if(MainFont == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF ERROR: %s\n",TTF_GetError());

    }

    WINDOW_FLAGS |= SDL_WINDOW_ALLOW_HIGHDPI;

    if (SDL_CreateWindowAndRenderer(MAX_COL+GRAPHW, MAX_ROW, WINDOW_FLAGS, &MainWindow, &MainRenderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return false;
    }
    else
    {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Window and renderer created.\n");
    }
    SDL_SetWindowTitle(MainWindow, "VIRUS SIM");

    // INITIAL INFECTATION
    for(int i=0; infections<INITIAL_INFECTED; i++){
      int target = gen.get_device_random(0,POPULATION-1);
      if(!infected[target]){
        infected[target]=true;
        infections++;
        //infected_vector.push_back(target);
      }
    }
    old_infections=infections;

    char x10[1024];
    sprintf(x10,"(x10)");
    SDL_Surface* srf_msgX10 = TTF_RenderText_Solid(MainFont, x10, BLUE);
    SDL_Texture* msgX10 = SDL_CreateTextureFromSurface(MainRenderer, srf_msgX10);
    int w,h;
    TTF_SizeText(MainFont, x10,&w,&h);
    SDL_Rect rect{MAX_COL,MAX_ROW-GRAPHH,w,h};
    SDL_RenderCopy(MainRenderer, msgX10, NULL, &rect);
    SDL_DestroyTexture(msgX10);
    SDL_FreeSurface(srf_msgX10);
    return true;
}
