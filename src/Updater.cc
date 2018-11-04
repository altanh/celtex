#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

#include "Updater.h"

void Updater::update(Grid *grid) {
  // make a fresh grid, make changes there, then replace the original grid
  Grid dest(grid->getWidth(), grid->getHeight(), grid->isTorus());

  uint32_t chunk_height = grid->getHeight() / _threads;
  uint32_t chunk_remainder = grid->getHeight() % _threads;

  std::vector<std::thread> threads;

  for (size_t i = 0; i < _threads; ++i) {
    uint32_t y = i * chunk_height;
    uint32_t h = (i == _threads - 1) ?
        chunk_height + chunk_remainder : chunk_height;

    threads.emplace_back(updateChunk, this, grid, &dest, 0, y, dest.getWidth(), h);
  }

  for (auto &t : threads)
    t.join();

  *grid = std::move(dest);
}

uint8_t GOL::_aliveCount(std::vector<const Cell*> adj) {
  uint8_t count = 0;

  for(size_t i = 0; i < 8; ++i) {
    count += adj[i] ? (adj[i]->state ? 1 : 0) : 0;
  }

  return count;
}

void GOL::updateChunk(Grid *grid, Grid *dest, 
    uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  for (uint32_t v = y; v < y + h; ++v) {
    for (uint32_t u = x; u < x + w; ++u) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(u, v));

      bool alive = grid->getCell(u, v).state ?
          ((alive_count == 2) || (alive_count == 3)) :
          (alive_count == 3);

      dest->setCell(u, v, { alive });
    }
  }
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

void StochasticGOL::updateChunk(Grid *grid, Grid *dest, 
    uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  for (size_t v = y; v < y + h; ++v) {
    for (size_t u = x; u < x + w; ++u) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(u, v));
      std::uniform_real_distribution<double> unif_dist(0.0, 1.0);
      double p = unif_dist(_rand);

      bool alive = p <= _aliveProbability(grid->getCell(u, v), alive_count);

      dest->setCell(u, v, { alive });
    }
  }
}
