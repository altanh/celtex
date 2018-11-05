#include "Automaton.h"

Automaton::Automaton(uint32_t w, uint32_t h, bool torus, std::shared_ptr<Updater> u)
  : _grid(w, h, torus), _dest(w, h, torus), _updater(u), _texture(w, h) {}

const Grid &Automaton::getGrid() const {
  return _grid;
}

Grid &Automaton::getGrid() {
  return _grid;
}

const Texture &Automaton::getTexture() const {
  return _texture;
}

void Automaton::putPattern(int32_t x, int32_t y, const Pattern &p) {
  for (uint32_t v = 0; v < p.getHeight(); ++v) {
    for (uint32_t u = 0; u < p.getWidth(); ++u) {
      _grid.setCell(x + (int64_t) u, y + (int64_t) v, p.getCell(u, v));
    }
  }
}

void Automaton::step() {
  _updater->update(&_grid, &_dest);
  _grid = _dest;
}
