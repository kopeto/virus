#include "Sim.h"


void Sim::OnLoop() {
  //*****************************************
  // MAIN LOOP
  //****************************************
  new_infected_today = 0;
  for(int id = 0; id<POPULATION; id++)
  {
    if(infected[id])
    {
      if(++days_infected[id]>=15)
      {
        immune[id]=1;
        immunes++;
        infected[id]=0;
        infections--;
      }
    }
    if(!immune[id] && infected[id])
    {
      infect_around(id,gen);
      if(!dead[id])
      {
        if(kill(gen,die_P))
        {
          dead[id]=1;
          deads++;
          infected[id]=0;
          infections--;
        }
      }
    }
  }

  // UPDATE infected
  for(int id=0;id<POPULATION;id++)
  {
    if(infection_MASK[id])
    {
      infected[id]=infection_MASK[id];
      days_infected[id]=0;
      infections++;
      infection_MASK[id]=false;
      new_infected_today++;
    }
  }

  // Show intermediate results
  if(day%SAMPLE_RATE==0 && DisplayInConsole)
  {
    display_results();
  }

  //old_infections = infections;

  data_state[day][0]=infections;
  data_state[day][1]=immunes;
  data_state[day][2]=deads;

  std::cout<<data_state[day][0]<<":"
    <<data_state[day][1]<<":"
    <<data_state[day][2]<<'\n';
}
