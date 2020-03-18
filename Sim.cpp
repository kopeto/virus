#include "Sim.h"

Sim::Sim()
{
  //
  // DEFAULT INITIAL VLUES
  //
  MAX_ROW = 600;
  MAX_COL = 800;
  POPULATION = MAX_ROW * MAX_COL;
  infection_P = 0.1f;
  SAMPLE_RATE = 1;

  die_P = 0.01f;
  INITIAL_INFECTED = 100;
  DAYS = 100;
  IMMUNITY = 0.0f;

  DisplayInConsole = 0;
  CreatePNGSequence = 0;
  CreateMP4Video = 0;

  infection_MASK = new char[POPULATION];
  infected = new char[POPULATION];
  dead = new char[POPULATION];
  immune = new char[POPULATION];

  for(int i=0;i<POPULATION;i++)
  {
    immune[i]=0;
  }

  day = 0;
  infections=0;
  old_infections=0;
  deads=0;

  //RandomGen gen;
  MainWindow = NULL;
  MainRenderer = NULL;
  MainFont = NULL;
  Running = true;
  Pause = false;
}

Sim::Sim(const char * filename)
{
  MAX_ROW = 600;
  MAX_COL = 800;

  infection_P = 0.1f;
  SAMPLE_RATE = 1;

  die_P = 0.01f;
  INITIAL_INFECTED = 100;
  DAYS = 100;
  IMMUNITY = 0.0f;

  DisplayInConsole = 0;
  CreatePNGSequence = 0;
  CreateMP4Video = 0;

  std::string line;
  std::ifstream data(filename);
  if(data.is_open())
  {

    while (getline(data,line))
    {
      if(!line.length()) continue;
      //std::cout<< line << '\n';
      std::istringstream stream(line);
      std::string option;
      stream >> option;
      if(option[0]=='#') continue;
      if(option == "MAX_ROW")
      {
        std::string s; stream >> s;
        MAX_ROW = stoi(s);
      }
      else if(option == "MAX_COL")
      {
        std::string s; stream >> s;
        MAX_COL = stoi(s);
      }
      else if(option == "infection_P")
      {
        std::string s; stream >> s;
        infection_P = stod(s);
      }
      else if(option == "SAMPLE_RATE")
      {
        std::string s; stream >> s;
        SAMPLE_RATE = stoi(s);
      }
      else if(option == "die_P")
      {
        std::string s; stream >> s;
        die_P = stod(s);
      }
      else if(option == "INITIAL_INFECTED")
      {
        std::string s; stream >> s;
        INITIAL_INFECTED = stoi(s);
      }
      else if(option == "DAYS")
      {
        std::string s; stream >> s;
        DAYS = stoi(s);
      }
      else if(option == "IMMUNITY")
      {
        std::string s; stream >> s;
        IMMUNITY = stod(s);
      }
      else if(option == "DisplayInConsole")
      {
        std::string s; stream >> s;
        DisplayInConsole = stoi(s);
      }
      else if(option == "CreatePNGSequence")
      {
        std::string s; stream >> s;
        CreatePNGSequence = stoi(s);
      }
      else if(option == "CreateMP4Video")
      {
        std::string s; stream >> s;
        CreateMP4Video = stoi(s);
      }
      else
      {
        std::cout<<"Option \""<<option<<"\" not recognized.\n";
      }
    }
    data.close();
  }

  POPULATION = MAX_ROW * MAX_COL;

  infection_MASK = new char[POPULATION];
  infected = new char[POPULATION];
  dead = new char[POPULATION];
  immune = new char[POPULATION];

  for(int i=0;i<POPULATION*IMMUNITY;i++)
  {
    immune[i]=1;
  }
  std::shuffle (&immune[0], &immune[POPULATION-1], std::default_random_engine(gen.get_device_random(0,0xFFFFFF)));

  infections=0;
  old_infections=0;
  deads = 0;
  day = 0;

  //RandomGen gen;
  MainWindow = NULL;
  MainRenderer = NULL;
  Running = true;
}

bool Sim::infect(RandomGen& gen,double prob)
{
  return gen.get_device_random(0,10000)<(int)(prob*10000);
}

bool Sim::kill(RandomGen& gen,double prob)
{
  return gen.get_device_random(0,10000)<(int)(prob*10000);
}

void Sim::infect_around(int id,RandomGen& gen)
{
  int row = id/MAX_COL;
  int col = id%MAX_COL;

  if(row!=0)
  {
    if(!infected[(row-1)*MAX_COL+col] && !infection_MASK[(row-1)*MAX_COL+col] && !immune[(row-1)*MAX_COL+col])
    {
      infection_MASK[(row-1)*MAX_COL+col]=infect(gen,infection_P);
      infections += (int)infection_MASK[(row-1)*MAX_COL+col];
    }

    if(col!=0)
    {
      if(!infected[(row-1)*MAX_COL+col-1] && !infection_MASK[(row-1)*MAX_COL+col-1] && !immune[(row-1)*MAX_COL+col-1])
      {
        infection_MASK[(row-1)*MAX_COL+col-1]=infect(gen,infection_P);
        infections += (int)infection_MASK[(row-1)*MAX_COL+col-1];
      }
    }
    if(col<MAX_COL-1)
    {
      if(!infected[(row-1)*MAX_COL+col+1] && !infection_MASK[(row-1)*MAX_COL+col+1] && ! immune[(row-1)*MAX_COL+col+1])
      {
        infection_MASK[(row-1)*MAX_COL+col+1]=infect(gen,infection_P);
        infections+=infection_MASK[(row-1)*MAX_COL+col+1];
      }
    }
  }

  if(row!=MAX_ROW-1)
  {
    if(!infected[(row+1)*MAX_COL+col] && !infection_MASK[(row+1)*MAX_COL+col] && !immune[(row+1)*MAX_COL+col])
    {
      infection_MASK[(row+1)*MAX_COL+col]=infect(gen,infection_P);
      infections+=(int)infection_MASK[(row+1)*MAX_COL+col];
    }

    if(col!=0)
    {
      if(!infected[(row+1)*MAX_COL+col-1] && !infection_MASK[(row+1)*MAX_COL+col-1] && !immune[(row+1)*MAX_COL+col-1])
      {
        infection_MASK[(row+1)*MAX_COL+col-1]=infect(gen,infection_P);
        infections+=(int)infection_MASK[(row+1)*MAX_COL+col-1];
      }
    }
    if(col<MAX_COL-1)
    {
      if(!infected[(row+1)*MAX_COL+col+1] && !infection_MASK[(row+1)*MAX_COL+col+1] && !immune[(row+1)*MAX_COL+col+1])
      {
        infection_MASK[(row+1)*MAX_COL+col+1]=infect(gen,infection_P);
        infections+=(int)infection_MASK[(row+1)*MAX_COL+col+1];
      }
    }
  }

  if(col!=0)
  {
    if(!infected[(row)*MAX_COL+col-1] && !infection_MASK[(row)*MAX_COL+col-1] && !immune[(row)*MAX_COL+col-1])
    {
      infection_MASK[(row)*MAX_COL+col-1]=infect(gen,infection_P);
      infections+=(int)infection_MASK[(row)*MAX_COL+col-1];
    }
  }

  if(col<MAX_COL-1)
  {
    if(!infected[(row)*MAX_COL+col+1] && !infection_MASK[(row)*MAX_COL+col+1] && !immune[(row)*MAX_COL+col+1])
    {
      infection_MASK[(row)*MAX_COL+col+1]=infect(gen,infection_P);
      infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
    }
  }
}

void Sim::display_results()
{
  std::cout<<"DAY "<<day<<": "<<infections;
  std::cout<<"(+"<<infections - old_infections<<") ";
  std::cout<<"Dead: "<<deads<<'\n';
}

void Sim::save_texture(SDL_Texture *texture, const char *filename) {
    SDL_Texture* target = SDL_GetRenderTarget(MainRenderer);
    SDL_SetRenderTarget(MainRenderer, texture);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(MainRenderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, filename);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(MainRenderer, target);
}

void Sim::RenderMP4Video()
{
  // example command
  // ffmpeg -loop 3 -r 20 -i img/sequences/%03d.png -c:v libx264 -vf "fps=25,format=yuv420p,scale=800:-1" out_loop3.mp4
  char command[1024];
  sprintf(command,"ffmpeg -r 30 -i img/sequences/%%03d.png -c:v libx264 -vf \"fps=25,format=yuv420p\" -y video/out%ddays.mp4",DAYS);
  std::system(command);
}
