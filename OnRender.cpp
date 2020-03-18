#include "Sim.h"

void Sim::OnRender() {
  SDL_Rect rect{0,0,MAX_COL,MAX_ROW};
  SDL_SetRenderDrawColor(MainRenderer, RENDER_COLOR_BLACK);
  SDL_RenderFillRect(MainRenderer,&rect);

  SDL_SetRenderDrawColor(MainRenderer, RENDER_COLOR_INFECTED);
  for(int i=0; i<POPULATION; i++ )
  {
    if(infected[i] && !dead[i]) SDL_RenderDrawPoint(MainRenderer, i%MAX_COL,i/MAX_COL);
  }
  SDL_SetRenderDrawColor(MainRenderer, RENDER_COLOR_DEAD);
  for(int i=0; i<POPULATION; i++ )
  {
    if(dead[i]) SDL_RenderDrawPoint(MainRenderer, i%MAX_COL,i/MAX_COL);
  }

  SDL_SetRenderDrawColor(MainRenderer, RENDER_COLOR_IMMUNE);
  for(int i=0; i<POPULATION; i++ )
  {
    if(immune[i]) SDL_RenderDrawPoint(MainRenderer, i%MAX_COL,i/MAX_COL);
  }

  RenderInfo();
  RenderGraph();
  SDL_RenderPresent(MainRenderer);

  if(CreatePNGSequence)
  {
    SDL_Texture*  texture = SDL_CreateTexture(MainRenderer,
                               SDL_PIXELFORMAT_RGBA32,
                               0,
                               MAX_COL+GRAPHW,
                               MAX_ROW);
    char filename[1024];
    sprintf(filename,"img/sequences/%03d.png",day);
    save_texture(texture,filename);
  }
  if(day==DAYS)
  {
    SDL_Texture*  texture = SDL_CreateTexture(MainRenderer,
                               SDL_PIXELFORMAT_RGBA32,
                               0,
                               MAX_COL+GRAPHW,
                               MAX_ROW);
    char filename[1024];
    sprintf(filename,"img/virus_%ddays_P%.3f_Immune%.2f_init%d.png",DAYS,infection_P,IMMUNITY,INITIAL_INFECTED);
    save_texture(texture,filename);
  }
}

void Sim::RenderInfo()
{
  char info_day[1024];
  char info_infected[1024];
  char info_dead[1024];

  sprintf(info_day,"DAY %d",day);
  sprintf(info_infected,
    "Infected %6d (%%%.1f)",
    infections,(double)infections*100/POPULATION);
  sprintf(info_dead,
    "Dead     %6d (%%%.1f)",
    deads,(double)deads*100/POPULATION);

  SDL_Surface* srf_msgDAY = TTF_RenderText_Solid(MainFont, info_day, WHITE);
  SDL_Texture* msgDAY = SDL_CreateTextureFromSurface(MainRenderer, srf_msgDAY);
  SDL_Surface* srf_msgINFECTED = TTF_RenderText_Solid(MainFont, info_infected, WHITE);
  SDL_Texture* msgINFECTED = SDL_CreateTextureFromSurface(MainRenderer, srf_msgINFECTED);
  SDL_Surface* srf_msgDEAD = TTF_RenderText_Solid(MainFont, info_dead, WHITE);
  SDL_Texture* msgDEAD = SDL_CreateTextureFromSurface(MainRenderer, srf_msgDEAD);

  int w1,w2,w3,h;
  TTF_SizeText(MainFont, info_day,&w1,&h);
  TTF_SizeText(MainFont, info_infected,&w2,&h);
  TTF_SizeText(MainFont, info_dead,&w3,&h);
  SDL_Rect Message1_rect{MAX_COL,0*h,w1,h};
  SDL_Rect Message2_rect{MAX_COL,1*h,w2,h};
  SDL_Rect Message3_rect{MAX_COL,2*h,w3,h};
  SDL_SetRenderDrawColor(MainRenderer,0,0,0,0);
  SDL_RenderFillRect(MainRenderer,&Message1_rect);
  SDL_RenderFillRect(MainRenderer,&Message2_rect);
  SDL_RenderFillRect(MainRenderer,&Message3_rect);
  SDL_RenderCopy(MainRenderer, msgDAY, NULL, &Message1_rect);
  SDL_RenderCopy(MainRenderer, msgINFECTED, NULL, &Message2_rect);
  SDL_RenderCopy(MainRenderer, msgDEAD, NULL, &Message3_rect);
  SDL_DestroyTexture(msgDAY);
  SDL_DestroyTexture(msgINFECTED);
  SDL_DestroyTexture(msgDEAD);
  SDL_FreeSurface(srf_msgDAY);
  SDL_FreeSurface(srf_msgINFECTED);
  SDL_FreeSurface(srf_msgDEAD);
}

void Sim::RenderGraph()
{
  SDL_Rect rect_infected{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)infections/POPULATION*GRAPHH),//y
      (int)(GRAPHW/DAYS)+1,//w
      (int)((double)infections/POPULATION*GRAPHH) -1//h
    };
  SDL_Rect rect_dead{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)deads/POPULATION*GRAPHH),//y
      (int)(GRAPHW/DAYS)+1,//w
      (int)((double)deads/POPULATION*GRAPHH) -1//h
    };
  // SDL_Rect rect_new_infected{
  //     MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
  //     MAX_ROW-(int)((double)(infections - old_infections)/POPULATION*GRAPHH*10),//y
  //     (int)(GRAPHW/DAYS)+1,//w
  //     (int)((double)(infections - old_infections)/POPULATION*GRAPHH*10)-1//h
  //   };
  SDL_Rect rect_new_infected{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)(infections - old_infections)/POPULATION*GRAPHH*10),//y
      3,//w
      3//h
    };
  // SDL_Point point_new_infected{
  //   MAX_COL+(int)((double)day/DAYS*GRAPHW),
  //   MAX_ROW-(int)((double)(infections - old_infections)/POPULATION*GRAPHH*10)
  // };

  SDL_SetRenderDrawColor(MainRenderer,0xFF, 0xA7, 0x00, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_infected);
  SDL_SetRenderDrawColor(MainRenderer,0xFF, 0x00, 0x00, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_dead);
  SDL_SetRenderDrawColor(MainRenderer,0x00, 0x00, 0xFF, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_new_infected);
  //SDL_RenderDrawPoint(MainRenderer,point_new_infected.x,point_new_infected.y);

  SDL_SetRenderDrawColor(MainRenderer,0xFF,0,0,0);
  SDL_Rect GRAPH_DELIMITER{MAX_COL,MAX_ROW-GRAPHH,GRAPHW,GRAPHH};
  SDL_RenderDrawRect(MainRenderer,&GRAPH_DELIMITER);

}
