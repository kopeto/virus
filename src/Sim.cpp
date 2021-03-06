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
  until_end = 0;

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

  data_state = new int*[DAYS+1];
  for(int d=0; d<DAYS+1; d++)
  {
    data_state[d]=new int[3];
  }

}

Sim::Sim(const char * filename)
{
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
  until_end = 0;

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

  data_state = new int*[DAYS+1];
  for(int d=0; d<DAYS+1; d++)
  {
    data_state[d]=new int[3];
  }
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

void Sim::Surface_to_PNG(SDL_Surface* surface, const char *filename) {
    IMG_SavePNG(surface, filename);
}

void Sim::CleanPNGs()
{
  int ret = std::system("rm img/sequences/*.png");
}

void Sim::RenderMP4Video()
{
  char create_mp4_command[256];
  char filename[64];
  int count = 0;
  do
  {
    sprintf(filename,"video/%d_days-POP_%d-InfP_%.2f-Immunes_%.2f_%03d.mp4",
    DAYS,POPULATION,infection_P,IMMUNITY,count );
    count++;
  }while(boost::filesystem::exists(filename));

  sprintf(create_mp4_command,"ffmpeg -r 30 -i img/sequences/%%03d.png -c:v libx264 -vf \"fps=25,format=yuv420p\" -y %s",filename);
  int ret = std::system(create_mp4_command);
}

void Sim::RenderGif()
{
  char create_gif_command[256];
  char filename[64];
  int count = 0;
  do
  {
    sprintf(filename,"video/%d_days-POP_%d-InfP_%.2f-Immunes_%.2f_%03d.gif",
    DAYS,POPULATION,infection_P,IMMUNITY,count );
    count++;
  }while(boost::filesystem::exists(filename));
  sprintf(create_gif_command,"ffmpeg -r 30 -i img/sequences/%%03d.png -vf \"fps=25,format=yuv420p\" -y %s",filename);
  int ret = std::system(create_gif_command);
}

void Sim::PrintReport()
{
  std::ofstream log_outfile;
  log_outfile.open ("last_simulation.log");
  log_outfile <<  "================================\n";
  log_outfile <<  "   VIRUS SIMULATION LOG FILE    \n";
  log_outfile <<  "================================\n\n";

  char totalcases[256];
  sprintf(totalcases,
    "Total cases: %7d\n",
    total_cases);
  log_outfile << totalcases;

  char maxInfected[256];
  sprintf(maxInfected,
    "Max Infected at once: %7d Day %d\n",
    max_infected, max_infected_day);
  log_outfile << maxInfected;

  char deathRate[256];
  sprintf(deathRate,
    "Death rate: %%%2.2f\n",
    (double)deads/total_cases*100);
  log_outfile << deathRate;

  if(zero_infected_day!=-1)
  {
    char zeroInfected[256];
    sprintf(zeroInfected,
      "All healthy in day: %d\n\n",
      zero_infected_day);
    log_outfile << zeroInfected;
  }
  else
  {
    log_outfile << '\n';
  }
  char header[1024];
  sprintf(header, " Day  Healthy  Infected    Dead\n");
  log_outfile << header;
  for(int day=0; day<=DAYS; day++)
  {
    char line[1024];
    sprintf(line, " %3d  %7d  %8d %7d\n",
    day,POPULATION-data_state[day][0]-data_state[day][2],
    data_state[day][0],data_state[day][2]);
    log_outfile << line;
  }
  log_outfile.close();
}
