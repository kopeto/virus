#include "Sim.h"

void Sim::OnCleanup() {
  delete[] infection_MASK;
  delete[] infected;
  delete[] dead;
  delete[] immune;

  if(MainRenderer)
  SDL_DestroyRenderer(MainRenderer);
  if(MainWindow)
  SDL_DestroyWindow(MainWindow);
  if(MainFont)
  TTF_CloseFont( MainFont );

  TTF_Quit();
  SDL_Quit();
}
