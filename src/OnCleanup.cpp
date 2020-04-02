#include "Sim.h"

void Sim::OnCleanup() {

  std::cout << "CLEANING UP...\n";

  delete[] infection_MASK;
  delete[] infected;
  delete[] dead;
  delete[] immune;
  delete[] data_state;
  delete[] days_infected;

  for(auto p_Texture: Texture_Vector)
  {
    SDL_DestroyTexture(p_Texture);
  }

  if(MainRenderer)
  SDL_DestroyRenderer(MainRenderer);
  if(MainWindow)
  SDL_DestroyWindow(MainWindow);
  if(MainFont)
  TTF_CloseFont( MainFont );

  TTF_Quit();
  SDL_Quit();
}
