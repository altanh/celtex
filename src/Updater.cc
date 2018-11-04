#include <vector>

#include "Updater.h"

uint8_t GOL::_aliveCount(std::vector<const Cell*> adj) {
  uint8_t count = 0;

  for(size_t i = 0; i < 8; ++i) {
    count += adj[i] ? (adj[i]->state ? 1 : 0) : 0;
  }

  return count;
}

void GOL::update(Grid *grid) {
  // make a copy of grid, make changes there, then replace the origin grid
  Grid copy = *grid;

  for (size_t y = 0; y < grid->getHeight(); ++y) {
    for (size_t x = 0; x < grid->getWidth(); ++x) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(x, y));

      bool alive = grid->getCell(x, y).state ?
          ((alive_count == 2) || (alive_count == 3)) :
          (alive_count == 3);

      copy.setCell(x, y, { alive });
    }
  }

  *grid = std::move(copy);
}

double StochasticGOL::_aliveProbability(const Cell &c, uint8_t alive_count) {
  if (c.state) {
    if (alive_count < 2) {
      return 0.1;
    } else if(alive_count == 2 || alive_count == 3) {
      return 0.95;
    } else {
      return 0.2;
    }
  } else {
    if (alive_count < 3) {
      return 0.0;
    } else if (alive_count > 3) {
      return 0.01 * alive_count;
    } else {
      return 0.98;
    }
  }
}

void StochasticGOL::update(Grid *grid) {
  Grid copy = *grid;

  for (size_t y = 0; y < grid->getHeight(); ++y) {
    for (size_t x = 0; x < grid->getWidth(); ++x) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(x, y));
      std::uniform_real_distribution<double> unif_dist(0.0, 1.0);
      double p = unif_dist(_rand);

      bool alive = p <= _aliveProbability(grid->getCell(x, y), alive_count);

      copy.setCell(x, y, { alive });
    }
  }

  *grid = std::move(copy);
}
