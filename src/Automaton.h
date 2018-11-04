#ifndef CELTEX_AUTOMATON_H
#define CELTEX_AUTOMATON_H

#include <cstdint>
#include <memory>

#include "Grid.h"
#include "Updater.h"
#include "Pattern.h"

class Automaton {
 public:
  Automaton(uint32_t w, uint32_t h, bool torus, std::shared_ptr<Updater> u);

  const Grid &getGrid() const;
  Grid &getGrid();

  void putPattern(int32_t x, int32_t y, const Pattern &p);

  void step();

 private:
  Grid _grid;
  std::shared_ptr<Updater> _updater;
};

#endif