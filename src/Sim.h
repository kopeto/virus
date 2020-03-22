#ifndef _SIM_H
#define _SIM_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <cstdlib>
#include <boost/filesystem.hpp>

#include "random_generator.h"
#include "CEvent.h"
#include "Defines.h"

const SDL_Color WHITE = {0xFF, 0xFF, 0xFF};
const SDL_Color BLACK = {0x00, 0x00, 0x00};
const SDL_Color RED = {0xFF, 0x00, 0x00};
const SDL_Color BLUE = {0x00, 0x00, 0xFF};

const int GRAPHW = 600;
const int GRAPHH = 400;

class Sim : public CEvent
{
public:
  //Default Constructor
  Sim();
  Sim(const char *);
  int MAX_ROW;
  int MAX_COL;
  int POPULATION;
  double infection_P;
  int SAMPLE_RATE;
  double die_P;
  double heal_P;
  int recovery_days;
  int INITIAL_INFECTED;
  int DAYS;
  double IMMUNITY;
  int until_end;

  unsigned char* infection_MASK = NULL;
  unsigned char* infected = NULL;
  unsigned char* dead = NULL ;
  unsigned char* immune = NULL;
  int* days_infected = NULL;
  int** data_state = NULL;

  int infections = 0;
  int old_infections = 0;
  int new_infected_today = 0;
  int deads = 0;
  int immunes = 0;
  int day = 0;
  int max_infected = 0;
  int max_infected_day = 0;
  int total_cases = 0;
  int zero_infected_day =-1;
  int all_healthy = 0;
  RandomGen gen;

  SDL_Window* MainWindow = NULL;
  SDL_Renderer* MainRenderer = NULL;
  TTF_Font* MainFont = NULL;

  //std::vector<SDL_Surface*> Surface_Vector;
  std::vector<SDL_Texture*> Texture_Vector;
  bool SimulationEnd = false;
  int Simulation_Current_Frame=0;

  bool Running = true;
  bool Pause = false;
  bool DisplayInConsole = 0;
  bool CreatePNGSequence = 1;
  bool CreateMP4Video = 1;

  bool infect(RandomGen& gen, double prob);
  void infect_around(int id, RandomGen& gen);
  bool kill(RandomGen& gen, double prob);

  void Surface_to_PNG(SDL_Surface* surface, const char *filename);
  void CleanPNGs();
  void RenderMP4Video();
  void RenderGif();
  void RenderDay();
  void PrintReport();

public:
  bool OnInit();
  void OnEvent(SDL_Event* Event);
  void OnLButtonDown(int mX, int mY);
  void OnRButtonDown(int mX, int mY);
  void OnKeyDown(SDL_Keycode sym, Uint16 mod, Uint16 unicode);
  void OnKeyUp(SDL_Keycode sym, Uint16 mod, Uint16 unicode);
  void OnMouseWheel(bool Up, bool Down);
  void OnExit();
  void OnPause();
  void OnLoop();
  void OnRender();
  void RenderInfo();
  void RenderGraph();
  void display_results();
  void OnCleanup();
  int OnExecute();
};

#endif //_SIM_H
