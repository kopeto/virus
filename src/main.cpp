#include "Sim.h"
#include <iostream>

int main(int argc, char* argv[]) {
  Sim *virus;
  if(argc > 1)
  {
    virus = new Sim(argv[1]);
  }
  else
  {
    virus = new Sim();
  }
  std::cout<<"START\n";
  if(virus->OnExecute())
    std::cout<<"END with errors.\n";
  else
    std::cout<<"END\n";
  return 0;
}
