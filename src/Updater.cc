#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <bitset>

#include "Updater.h"

void Updater::update(Grid *grid, Grid *dest) {
  uint32_t chunk_height = grid->getHeight() / _threads;
  uint32_t chunk_remainder = grid->getHeight() % _threads;

  std::vector<std::thread> threads;

  for (size_t i = 0; i < _threads; ++i) {
    uint32_t y = i * chunk_height;
    uint32_t h = (i == _threads - 1) ?
        chunk_height + chunk_remainder : chunk_height;

    threads.emplace_back(&Updater::updateChunk, this, 
        grid, dest, 0, y, dest->getWidth(), h);
  }

  for (auto &t : threads)
    t.join();
}

static uint8_t _aliveCount(std::vector<const Cell*> adj) {
  uint8_t count = 0;

  for(size_t i = 0; i < 8; ++i) {
    count += adj[i] ? (adj[i]->state ? 1 : 0) : 0;
  }

  return count;
}

void GenGOL::updateChunk(Grid *grid, Grid *dest, 
    uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  for (uint32_t v = y; v < y + h; ++v) {
    for (uint32_t u = x; u < x + w; ++u) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(u, v));
      const Cell &c = grid->getCell(u, v);
      
      dest->setCell(u, v, { ((1 << alive_count) & (c.state ? _S : _B)) != 0 });
    }
  }
}

void StochasticGOL::updateChunk(Grid *grid, Grid *dest, 
    uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  for (size_t v = y; v < y + h; ++v) {
    for (size_t u = x; u < x + w; ++u) {
      uint8_t alive_count = _aliveCount(grid->getAdjacent(u, v));
      const Cell &c = grid->getCell(u, v);

      static std::uniform_real_distribution<double> unif_dist(0.0, 1.0);
      double p = unif_dist(_rand);

      bool alive = p <= (c.state ? _pS[alive_count] : _pB[alive_count]);

      dest->setCell(u, v, { alive });
    }
  }
}

static bool _notAdj(std::vector<const Cell*> adj) {
  std::vector<size_t> locs(2, 0);

  // should only have 2
  for (size_t i = 0; i < adj.size(); ++i) {
    if (adj[i]->state)
      locs.push_back(i);
  }

  size_t x = locs[0], y = locs[1];

  return !(x == 0 && y == 1) && !(x == 0 && y == 3)
      && !(x == 1 && y == 2)
      && !(x == 2 && y == 4)
      && !(x == 3 && y == 5)
      && !(x == 4 && y == 7)
      && !(x == 5 && y == 6)
      && !(x == 6 && y == 7);
}

void JustFriends::updateChunk(Grid *grid, Grid *dest, 
    uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  for (size_t v = y; v < y + h; ++v) {
    for (size_t u = x; u < x + w; ++u) {
      std::vector<const Cell*> neighbors = grid->getAdjacent(u, v);
      uint8_t alive_count = _aliveCount(neighbors);
      const Cell &c = grid->getCell(u, v);

      if (c.state) {
        dest->setCell(u, v, { alive_count == 1 || alive_count == 2 });
      } else if (alive_count != 2) {
        dest->setCell(u, v, { false });
      } else {
        dest->setCell(u, v, { _notAdj(neighbors) });
      }
    }
  }
}
