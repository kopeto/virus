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
  heal_P = 0.0f;
  recovery_days = 15;
  INITIAL_INFECTED = 100;
  DAYS = 100;
  IMMUNITY = 0.0f;

  DisplayInConsole = 0;
  CreatePNGSequence = 0;
  CreateMP4Video = 0;
  until_end = 1;

  infection_MASK = new unsigned char[POPULATION];
  infected = new unsigned char[POPULATION];
  dead = new unsigned char[POPULATION];
  immune = new unsigned char[POPULATION];
  days_infected = new int[POPULATION];

  for(int i=0;i<POPULATION;i++)
  {
    immune[i]=0;
    days_infected[i]=-1;
  }

  data_state = new int*[DAYS];
  for(int d=0; d<DAYS+1; d++)
  {
    data_state[d]=new int[3];
  }


  day = 0;
  infections=0;
  old_infections=0;
  new_infected_today = 0;
  deads=0;
  immunes = 0;

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
  heal_P = 0.0f;
  recovery_days = 15;
  die_P = 0.01f;
  INITIAL_INFECTED = 100;
  DAYS = 100;
  IMMUNITY = 0.0f;

  DisplayInConsole = 0;
  CreatePNGSequence = 0;
  CreateMP4Video = 0;
  until_end = 1;

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
      else if(option == "recovery_days")
      {
        std::string s; stream >> s;
        recovery_days = stoi(s);
      }
      else if(option == "heal_P")
      {
        std::string s; stream >> s;
        heal_P = stod(s);
      }
      else if(option == "until_end")
      {
        std::string s; stream >> s;
        until_end = stoi(s);
      }
      else
      {
        std::cout<<"Option \""<<option<<"\" not recognized.\n";
      }
    }
    data.close();
  }

  POPULATION = MAX_ROW * MAX_COL;

  infection_MASK = new unsigned char[POPULATION];
  infected = new unsigned char[POPULATION];
  dead = new unsigned char[POPULATION];
  immune = new unsigned char[POPULATION];
  days_infected = new int[POPULATION];



  for(int i=0; i<POPULATION; i++)
  {
    days_infected[i]=-1;
  }

  data_state = new int*[DAYS];
  for(int d=0; d<DAYS+1; d++)
  {
    data_state[d]=new int[3];
  }

  day = 0;
  infections=0;
  old_infections=0;
  new_infected_today = 0;
  deads=0;
  immunes = 0;

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
    int id = (row-1)*MAX_COL+col;
    if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
    {
      infection_MASK[id]=infect(gen,infection_P);
      //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
    }

    if(col!=0)
    {
      int id = (row-1)*MAX_COL+col-1;
      if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
      {
        infection_MASK[id]=infect(gen,infection_P);
        //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
      }
    }
    if(col<MAX_COL-1)
    {
      int id = (row-1)*MAX_COL+col+1;
      if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
      {
        infection_MASK[id]=infect(gen,infection_P);
        //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
      }
    }
  }

  if(row!=MAX_ROW-1)
  {
    int id = (row+1)*MAX_COL+col;
    if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
    {
      infection_MASK[id]=infect(gen,infection_P);
      //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
    }

    if(col!=0)
    {
      int id = (row+1)*MAX_COL+col-1;
      if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
      {
        infection_MASK[id]=infect(gen,infection_P);
        //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
      }
    }
    if(col<MAX_COL-1)
    {
      int id = (row+1)*MAX_COL+col+1;
      if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
      {
        infection_MASK[id]=infect(gen,infection_P);
        //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
      }
    }
  }

  if(col!=0)
  {
    int id = (row)*MAX_COL+col-1;
    if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
    {
      infection_MASK[id]=infect(gen,infection_P);
      //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
    }
  }

  if(col<MAX_COL-1)
  {
    int id = (row)*MAX_COL+col+1;
    if(!infected[id] && !infection_MASK[id] && !immune[id] && !dead[id])
    {
      infection_MASK[id]=infect(gen,infection_P);
      //infections+=(int)infection_MASK[(row)*MAX_COL+col+1];
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
  int ret = std::system(command);
}
