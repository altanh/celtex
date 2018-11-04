#include <cstdint>
#include <vector>

#include "Pattern.h"

Pattern::Pattern(uint32_t w, uint32_t h, const std::vector<Cell> &cells)
    : _width(w), _height(h), _cells(cells) {}


const std::vector<Cell> &Pattern::getCells() const {
  return _cells;
}

const Cell &Pattern::getCell(uint32_t u, uint32_t v) const {
  return _cells[v * _height + u];
}

uint32_t Pattern::getWidth() const {
  return _width;
}

uint32_t Pattern::getHeight() const {
  return _height;
}
