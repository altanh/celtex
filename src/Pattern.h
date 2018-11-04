#ifndef CELTEX_PATTERN_H
#define CELTEX_PATTERN_H

#include <cstdint>
#include <vector>

#include "Cell.h"

class Pattern {
 public:
  Pattern(uint32_t w, uint32_t h, const std::vector<Cell> &cells);

  const std::vector<Cell> &getCells() const;
  const Cell &getCell(uint32_t u, uint32_t v) const;

  uint32_t getWidth() const;
  uint32_t getHeight() const;

 private:
  uint32_t _width;
  uint32_t _height;
  std::vector<Cell> _cells;
};

#endif