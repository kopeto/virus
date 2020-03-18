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

#include "random_generator.h"
#include "CEvent.h"
#include "Defines.h"

const SDL_Color WHITE = {0xFF, 0xFF, 0xFF};
const SDL_Color BLACK = {0x00, 0x00, 0x00};
const SDL_Color RED = {0xFF, 0x00, 0x00};
const SDL_Color BLUE = {0x00, 0x00, 0xFF};

const int GRAPHW = 600;
const int GRAPHH = 450;

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
  int INITIAL_INFECTED;
  int DAYS;
  double IMMUNITY;


  char* infection_MASK;
  char* infected;
  char* dead;
  char* immune;

  int infections;
  int old_infections;
  int deads;
  int day;
  RandomGen gen;

  SDL_Window* MainWindow;
  SDL_Renderer* MainRenderer;
  TTF_Font* MainFont;

  bool Running;
  bool Pause;
  bool DisplayInConsole;
  bool CreatePNGSequence;
  bool CreateMP4Video;

  bool infect(RandomGen& gen, double prob);
  void infect_around(int id, RandomGen& gen);
  bool kill(RandomGen& gen, double prob);

  void save_texture(SDL_Texture *tex, const char *filename);
  void RenderMP4Video();

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
