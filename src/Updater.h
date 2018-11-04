#ifndef CELTEX_UPDATOR_H
#define CELTEX_UPDATOR_H

#include <cstdlib>
#include <vector>
#include <random>

#include "Grid.h"

class Updater {
  size_t _threads;
 public:
  Updater() : _threads(1) {}
  Updater(size_t threads) : _threads(threads) {}

  virtual void update(Grid *grid);
  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
};

class GOL : public Updater {
 public:
  GOL() : Updater() {}
  GOL(size_t threads) : Updater(threads) {}

  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h);

 protected:
  static uint8_t _aliveCount(std::vector<const Cell*> adj);
};

class StochasticGOL : public GOL {
 public:
  StochasticGOL() : GOL() {}
  StochasticGOL(size_t threads) : GOL(threads) {}

  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h);

 private:
  std::default_random_engine _rand;
  static double _aliveProbability(const Cell &c, uint8_t alive_count);
};

#endif