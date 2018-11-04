#ifndef CELTEX_UPDATOR_H
#define CELTEX_UPDATOR_H

#include <cstdlib>
#include <vector>
#include <random>

#include "Grid.h"

class Updater {
 public:
  virtual void update(Grid *grid) = 0;
};

class GOL : public Updater {
 public:
  virtual void update(Grid *grid);

 protected:
  uint8_t _aliveCount(std::vector<const Cell*> adj);
};

class StochasticGOL : public GOL {
 public:
  virtual void update(Grid *grid);

 private:
  std::default_random_engine _rand;
  virtual double _aliveProbability(const Cell &c, uint8_t alive_count);
};

#endif