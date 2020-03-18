#include "Sim.h"


void Sim::OnLoop() {
  //*****************************************
  // MAIN LOOP
  //****************************************


  //
  // for(const auto &id: infected_vector)
  // {
  //   infect_around(id,gen);
  //   if(!dead[id])
  //   {
  //     dead[id]=kill(gen,die_P);
  //     deads += dead[id];
  //   }
  // }

  // Randomiz positions
  // obtain a time-based seed:
  // unsigned seed = gen.get_device_random(0,0xFFFFFF);
  // std::shuffle (&infected[0], &infected[POPULATION-1], std::default_random_engine(seed));
  for(int id = 0; id<POPULATION; id++)
  {
    if(infected[id])
    {
      infect_around(id,gen);
      if(!dead[id])
      {
        dead[id]=kill(gen,die_P);
        deads += dead[id];
      }
    }
  }

  // UPDATE infected
  for(int id=0;id<POPULATION;id++)
  {
    if(infection_MASK[id])
    {
      infected[id]=infection_MASK[id];
      infection_MASK[id]=false;
      //infected_vector.push_back(id);
    }

  }

  // Show intermediate results
  if(day%SAMPLE_RATE==0 && DisplayInConsole)
  {
    display_results();
  }

  // Break if all infected
  if(infections == POPULATION)
  {
    std::cout<<"All infected in "<<day<<" days(P="<<infection_P<<")\n";
    //Running = false;
  }
}
