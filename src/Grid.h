#ifndef CELTEX_GRID_H
#define CELTEX_GRID_H

#include <cstdint>
#include <vector>

#include "Cell.h"

class Grid {
 public:
  Grid();
  Grid(uint32_t width, uint32_t height, bool torus);
  Grid(Grid &&other);  // move constructor
  Grid(const Grid &other);
  ~Grid();

  Grid &operator=(const Grid &other);  // copy assignment
  Grid &operator=(Grid &&other);  // move assignment

  void create(uint32_t width, uint32_t height, bool torus);

  const Cell &getCell(int32_t x, int32_t y) const;
  std::vector<const Cell*> getAdjacent(int32_t x, int32_t y) const;
  void setCell(int32_t x, int32_t y, const Cell &c);

  uint32_t getCanonical(int32_t x, int32_t y) const;
  bool inBounds(int32_t x, int32_t y) const;

  uint32_t getWidth() const;
  uint32_t getHeight() const;
  bool isTorus() const;

 private:
  Cell *_cells;
  uint32_t _width;
  uint32_t _height;
  bool _torus;

  uint32_t _edgeWrap(int32_t x, int32_t y) const;
};

#endif