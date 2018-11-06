#include "Automaton.h"

Automaton::Automaton(uint32_t w, uint32_t h, bool torus, std::shared_ptr<Updater> u)
  : _front(new Grid(w, h, torus)), _back(new Grid(w, h, torus)), _updater(u), _texture(w, h) {}

Automaton::~Automaton() {
  delete _front;
  delete _back;
}

const Grid &Automaton::getGrid() const {
  return *_front;
}

Grid &Automaton::getGrid() {
  return *_front;
}

const Texture &Automaton::getTexture() const {
  return _texture;
}

void Automaton::putPattern(int32_t x, int32_t y, const Pattern &p) {
  for (uint32_t v = 0; v < p.getHeight(); ++v) {
    for (uint32_t u = 0; u < p.getWidth(); ++u) {
      _front->setCell(x + (int64_t) u, y + (int64_t) v, p.getCell(u, v));
    }
  }
}

void Automaton::step() {
  _updater->update(_front, _back);

  // swap front and back
  Grid *temp = _front;
  _front = _back;
  _back = temp;
}
