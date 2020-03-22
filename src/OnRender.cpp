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
  char info_line1[256];
  char info_line2[256];
  char info_line3[256];
  char info_line4[256];
  char info_line5[256];
  char info_line6[256];
  char info_line7[256];

  sprintf(info_line1,
    "DAY %4d POPULATION:%8d",
    day,POPULATION);
  sprintf(info_line2,
    "Infected %7d (%%%.1f)",
    infections,(double)infections*100/POPULATION);
  sprintf(info_line3,
    "Healthy  %7d (%%%.1f)",
      POPULATION-infections-deads,
      (double)(POPULATION-infections-deads)*100/POPULATION);
  sprintf(info_line4,
    "Dead     %7d (%%%.1f)",
    deads,(double)deads*100/POPULATION);
  sprintf(info_line5,
    "Total cases %7d (%%%.1f)",
    total_cases,(double)total_cases*100/POPULATION);
  sprintf(info_line6,
    "Death rate %%%2.2f",
    (double)deads/total_cases*100);
  sprintf(info_line7,
    "All healthy in day %d",
    zero_infected_day);

  SDL_Surface* srf_info_line1 = TTF_RenderText_Solid(MainFont, info_line1, WHITE);
  SDL_Texture* texture_msg_line1 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line1);
  SDL_Surface* srf_info_line2 = TTF_RenderText_Solid(MainFont, info_line2, WHITE);
  SDL_Texture* texture_info_line2 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line2);
  SDL_Surface* srf_info_line3 = TTF_RenderText_Solid(MainFont, info_line3, WHITE);
  SDL_Texture* texture_info_line3 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line3);
  SDL_Surface* srf_info_line4 = TTF_RenderText_Solid(MainFont, info_line4, WHITE);
  SDL_Texture* texture_info_line4 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line4);
  SDL_Surface* srf_info_line5 = TTF_RenderText_Solid(MainFont, info_line5, WHITE);
  SDL_Texture* texture_info_line5 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line5);
  SDL_Surface* srf_info_line6 = TTF_RenderText_Solid(MainFont, info_line6, WHITE);
  SDL_Texture* texture_info_line6 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line6);
  SDL_Surface* srf_info_line7 = TTF_RenderText_Solid(MainFont, info_line7, WHITE);
  SDL_Texture* texture_info_line7 = SDL_CreateTextureFromSurface(MainRenderer, srf_info_line7);


  int w1,w2,w3,w4,w5,w6,w7,h;
  TTF_SizeText(MainFont, info_line1,&w1,&h);
  TTF_SizeText(MainFont, info_line2,&w2,&h);
  TTF_SizeText(MainFont, info_line3,&w3,&h);
  TTF_SizeText(MainFont, info_line4,&w4,&h);
  TTF_SizeText(MainFont, info_line5,&w5,&h);
  TTF_SizeText(MainFont, info_line6,&w6,&h);
  TTF_SizeText(MainFont, info_line7,&w7,&h);

  SDL_SetRenderDrawColor(MainRenderer,0,0,0,0);
  SDL_Rect Info_RECT{MAX_COL,0,GRAPHW,MAX_ROW-GRAPHH};
  SDL_RenderFillRect(MainRenderer,&Info_RECT);
  SDL_Rect Message1_rect{MAX_COL,0*h,w1,h};
  SDL_Rect Message2_rect{MAX_COL,1*h,w2,h};
  SDL_Rect Message3_rect{MAX_COL,2*h,w3,h};
  SDL_Rect Message4_rect{MAX_COL,3*h,w4,h};
  SDL_Rect Message5_rect{MAX_COL,4*h,w5,h};
  SDL_Rect Message6_rect{MAX_COL,5*h,w6,h};
  SDL_Rect Message7_rect{MAX_COL,6*h,w7,h};
  // SDL_RenderFillRect(MainRenderer,&Message1_rect);
  // SDL_RenderFillRect(MainRenderer,&Message2_rect);
  // SDL_RenderFillRect(MainRenderer,&Message3_rect);
  // SDL_RenderFillRect(MainRenderer,&Message4_rect);
  // SDL_RenderFillRect(MainRenderer,&Message5_rect);
  SDL_RenderCopy(MainRenderer, texture_msg_line1, NULL, &Message1_rect);
  SDL_RenderCopy(MainRenderer, texture_info_line2, NULL, &Message2_rect);
  SDL_RenderCopy(MainRenderer, texture_info_line3, NULL, &Message3_rect);
  SDL_RenderCopy(MainRenderer, texture_info_line4, NULL, &Message4_rect);
  SDL_RenderCopy(MainRenderer, texture_info_line5, NULL, &Message5_rect);
  SDL_RenderCopy(MainRenderer, texture_info_line6, NULL, &Message6_rect);
  if(all_healthy)
  SDL_RenderCopy(MainRenderer, texture_info_line7, NULL, &Message7_rect);

  SDL_DestroyTexture(texture_msg_line1);
  SDL_DestroyTexture(texture_info_line2);
  SDL_DestroyTexture(texture_info_line3);
  SDL_DestroyTexture(texture_info_line4);
  SDL_DestroyTexture(texture_info_line5);
  SDL_DestroyTexture(texture_info_line6);
  SDL_DestroyTexture(texture_info_line7);
  SDL_FreeSurface(srf_info_line1);
  SDL_FreeSurface(srf_info_line2);
  SDL_FreeSurface(srf_info_line3);
  SDL_FreeSurface(srf_info_line4);
  SDL_FreeSurface(srf_info_line5);
  SDL_FreeSurface(srf_info_line6);
  SDL_FreeSurface(srf_info_line7);

}

void Sim::RenderGraph()
{
  // DEADS
  //
  SDL_Rect rect_dead{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)deads/POPULATION*GRAPHH),//y
      (int)(GRAPHW/DAYS)+1,//w
      (int)((double)deads/POPULATION*GRAPHH) -1//h
    };


  // INFECTIONS
  //
  SDL_Rect rect_infected{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)infections/POPULATION*GRAPHH),//y
      (int)(GRAPHW/DAYS)+1,//w
      (int)((double)infections/POPULATION*GRAPHH)-1//h
    };
  rect_infected.y -= rect_dead.h;

  // IMMUNES
  //
  // SDL_Rect rect_immune{
  //     MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
  //     MAX_ROW-(int)((double)immunes/POPULATION*GRAPHH),//y
  //     (int)(GRAPHW/DAYS)+1,//w
  //     (int)((double)immunes/POPULATION*GRAPHH) -1//h
  //   };
  SDL_Rect rect_new_infected{
      MAX_COL+(int)((double)day/DAYS*GRAPHW),//x
      MAX_ROW-(int)((double)new_infected_today/POPULATION*GRAPHH*10),//y
      3,//w
      3//h
    };

  SDL_SetRenderDrawColor(MainRenderer,0xFF, 0xA7, 0x00, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_infected);
  SDL_SetRenderDrawColor(MainRenderer,0xFF, 0x00, 0x00, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_dead);
  SDL_SetRenderDrawColor(MainRenderer,0x00, 0x00, 0xFF, 0x00);
  SDL_RenderFillRect(MainRenderer,&rect_new_infected);
  //SDL_RenderDrawPoint(MainRenderer,point_new_infected.x,point_new_infected.y);

  // GRAPH DELIMITER
  SDL_SetRenderDrawColor(MainRenderer,0xFF,0,0,0);
  SDL_Rect GRAPH_DELIMITER{MAX_COL,MAX_ROW-GRAPHH,GRAPHW,GRAPHH};
  SDL_RenderDrawRect(MainRenderer,&GRAPH_DELIMITER);

}
