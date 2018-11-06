#include <iostream>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <vector>
#include <random>

#include <SDL.h>

#include "Grid.h"
#include "Automaton.h"
#include "Updater.h"
#include "Pattern.h"

const int WIDTH = 800;
const int HEIGHT = 800;

const int AUT_WIDTH = WIDTH / 2;
const int AUT_HEIGHT = AUT_WIDTH * ((float) HEIGHT / WIDTH);

const float AUT_DELTA_X = (float) WIDTH / AUT_WIDTH;
const float AUT_DELTA_Y = (float) HEIGHT / AUT_HEIGHT;

std::vector<Cell> makeRect(uint32_t w, uint32_t h, double density) {
  std::uniform_real_distribution<double> unif_dist(0.0, 1.0);
  std::random_device rand;

  std::vector<Cell> res;
  for (size_t i = 0; i < w * h; ++i) {
    res.push_back({ unif_dist(rand) <= density });
  }

  return res;
}

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
    0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0
  };

  std::vector<double> pS = {
    0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0
  };

  Automaton aut(AUT_WIDTH, AUT_HEIGHT, true, std::shared_ptr<Updater>(new StochasticGOL(8, pB, pS)));//StochasticGOL(8, pB, pS)));

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

  uint32_t rect_w = AUT_WIDTH / 4;
  uint32_t rect_h = AUT_HEIGHT / 4;
  Pattern rect(rect_w, rect_h, makeRect(rect_w, rect_h, 1.0));

  //aut.putPattern(AUT_WIDTH / 2, AUT_HEIGHT / 2, pentomino);
  aut.putPattern(AUT_WIDTH / 2 - rect.getWidth() / 2, AUT_HEIGHT / 2 - rect.getHeight() / 2, rect);

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
          cell_rects.push_back({static_cast<int>((float) x * AUT_DELTA_X), 
            static_cast<int>((float) y * AUT_DELTA_Y), 
            static_cast<int>(AUT_DELTA_X), 
            static_cast<int>(AUT_DELTA_Y)});
      }
    }

    SDL_SetRenderDrawColor(ren, 205, 100, 100, 255);
    SDL_RenderFillRects(ren, cell_rects.data(), cell_rects.size());
    SDL_RenderPresent(ren);

    aut.step();
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}