#include <iostream>
#include <cstdlib>
#include <memory>
#include <SDL.h>

#include "Grid.h"
#include "Automaton.h"
#include "Updater.h"
#include "Pattern.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << SDL_GetError() << std::endl;

    return EXIT_FAILURE;
  }

  SDL_Window *win = SDL_CreateWindow("celtex | automata based texture generation", 
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!win) {
    std::cerr << SDL_GetError() << std::endl;
    SDL_Quit();

    return EXIT_FAILURE;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!ren) {
    SDL_DestroyWindow(win);
    std::cerr << SDL_GetError() << std::endl;
    SDL_Quit();

    return EXIT_FAILURE;
  }

  Automaton aut(WIDTH, HEIGHT, true, std::shared_ptr<Updater>(new GOL()));

  Pattern glider(3, 3, {
    { false }, {  true }, { false },
    { false }, { false }, {  true },
    {  true }, {  true }, {  true }
  });

  Pattern pentomino(3, 3, {
    { false }, {  true }, {  true },
    {  true }, {  true }, { false },
    { false }, {  true }, { false }
  });

  aut.putPattern(WIDTH / 2, HEIGHT / 2, pentomino);

  SDL_Event e;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;

        default:
          break;
      }
    }

    SDL_RenderClear(ren);

    const Grid &g = aut.getGrid();

    for (size_t y = 0; y < g.getHeight(); ++y) {
      for (size_t x = 0; x < g.getWidth(); ++x) {
        bool alive = g.getCell(x, y).state;

        SDL_SetRenderDrawColor(ren, alive ? 255 : 0, 0, 0, 255);
        SDL_RenderDrawPoint(ren, x, y);
      }
    }

    SDL_RenderPresent(ren);

    aut.step();
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}