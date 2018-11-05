#include <iostream>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <vector>

#include <SDL.h>

#include "Grid.h"
#include "Automaton.h"
#include "Updater.h"
#include "Pattern.h"

const int WIDTH = 800;
const int HEIGHT = 800;

const int AUT_WIDTH = WIDTH / 4;
const int AUT_HEIGHT = AUT_WIDTH * ((float) HEIGHT / WIDTH);

const float AUT_DELTA_X = (float) WIDTH / AUT_WIDTH;
const float AUT_DELTA_Y = (float) HEIGHT / AUT_HEIGHT;

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

  SDL_RendererInfo ri;
  SDL_GetRendererInfo(ren, &ri);

  std::cout << "using renderer: " << ri.name << std::endl;

  std::vector<double> pB = {
    0.0, 0.01, 0.95, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0
  };

  std::vector<double> pS = {
    0.0, 0.88, 1.0, 0.8, 0.2, 0.0625, 0.0, 1.0, 0.0
  };

  Automaton aut(AUT_WIDTH, AUT_HEIGHT, true, std::shared_ptr<Updater>(new StochasticGOL(8, pB, pS)));

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

  Pattern rect(AUT_WIDTH / 4, AUT_HEIGHT / 4, std::vector<Cell>(AUT_WIDTH * AUT_HEIGHT / 16, { true }));

  //aut.putPattern(AUT_WIDTH / 2, AUT_HEIGHT / 2, pentomino);
  aut.putPattern(AUT_WIDTH / 2 - AUT_WIDTH / 8, AUT_HEIGHT / 2 - AUT_HEIGHT / 8, rect);

  SDL_Event e;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    const Grid &g = aut.getGrid();
    std::vector<SDL_Rect> cell_rects;

    for (size_t y = 0; y < g.getHeight(); ++y) {
      for (size_t x = 0; x < g.getWidth(); ++x) {
        bool alive = g.getCell(x, y).state;

        if (alive)
          cell_rects.push_back({(float) x * AUT_DELTA_X, (float) y * AUT_DELTA_Y, AUT_DELTA_X, AUT_DELTA_Y});
      }
    }

    SDL_SetRenderDrawColor(ren, 255, 100, 0, 255);
    SDL_RenderFillRects(ren, cell_rects.data(), cell_rects.size());
    SDL_RenderPresent(ren);

    aut.step();
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}