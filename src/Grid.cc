#include <cstdlib>
#include <vector>

#include "Grid.h"

// from a comment by Christ Nolet on stackoverflow; assume b > 0
static inline _mod(int64_t a, int64_t b) {
  return (a < 0) ? (a % b + b) : (a % b);
}

Grid::Grid() : _cells(nullptr), _width(0), _height(0), _torus(false) {}

Grid::Grid(uint32_t width, uint32_t height, bool torus)
    : _cells(new Cell[width * height]), _width(width), _height(height), _torus(torus) {}

Grid::Grid(Grid &&other) 
    : _cells(other._cells), _width(other._width), _height(other._height), _torus(other._torus) {
  other._cells = nullptr;
  other._width = 0;
  other._height = 0;
}

// slow
Grid::Grid(const Grid &other)
    : _width(other._width), _height(other._height), _torus(other._torus) {
  _cells = new Cell[_width * _height];

  for (size_t i = 0; i < _width * _height; ++i)
    _cells[i] = other._cells[i];
}

Grid::~Grid() {
  if (_cells)
    delete[] _cells;
}

Grid &Grid::operator=(const Grid &other) {
  if (this == &other)
    return *this;

  create(other._width, other._height, other._torus);

  for (size_t y = 0; y < _height; ++y) {
    for (size_t x = 0; x < _width; ++x) {
      _cells[y * _width + x] = other._cells[y * _width + x];
    }
  }

  return *this;
}

Grid &Grid::operator=(Grid &&other) {
  if (this == &other)
    return *this;

  if (_cells)
    delete[] _cells;

  _cells = other._cells;
  _width = other._width;
  _height = other._height;
  _torus = other._torus;

  other._cells = nullptr;
  other._width = 0;
  other._height = 0;

  return *this;
}

void Grid::create(uint32_t width, uint32_t height, bool torus) {
  // delete if we had something before
  if (_cells) {
    delete[] _cells;

    _cells = nullptr;
  }

  _width = width;
  _height = height;
  _torus = torus;
  _cells = new Cell[_width * _height];
}

const Cell &Grid::getCell(int32_t x, int32_t y) const {
  // assuming (x,y) is not out of bounds
  return _cells[y * _width + x];
}

// 0 1 2
// 3   4
// 5 6 7
std::vector<const Cell*> Grid::getAdjacent(int32_t x, int32_t y) const {
  std::vector<const Cell*> adj(8);

  // most definitely can be optimized by expanding, but too lazy right now
  adj[0] = inBounds(x - 1, y - 1) ? &_cells[_edgeWrap(x - 1, y - 1)] : nullptr;
  adj[1] = inBounds(    x, y - 1) ? &_cells[_edgeWrap(    x, y - 1)] : nullptr;
  adj[2] = inBounds(x + 1, y - 1) ? &_cells[_edgeWrap(x + 1, y - 1)] : nullptr;
  adj[3] = inBounds(x - 1,     y) ? &_cells[_edgeWrap(x - 1,     y)] : nullptr;
  adj[4] = inBounds(x + 1,     y) ? &_cells[_edgeWrap(x + 1,     y)] : nullptr;
  adj[5] = inBounds(x - 1, y + 1) ? &_cells[_edgeWrap(x - 1, y + 1)] : nullptr;
  adj[6] = inBounds(    x, y + 1) ? &_cells[_edgeWrap(    x, y + 1)] : nullptr;
  adj[7] = inBounds(x + 1, y + 1) ? &_cells[_edgeWrap(x + 1, y + 1)] : nullptr;

  return adj;
}

inline uint32_t Grid::_edgeWrap(int32_t x, int32_t y) const {
  if (x == -1)
    x = _width - 1;
  else if (x == (int64_t) _width)
    x = 0;

  if (y == -1)
    y = _height - 1;
  else if (y == (int64_t) _height)
    y = 0;

  return y * _width + x;
}

void Grid::setCell(int32_t x, int32_t y, const Cell &c) {
  _cells[getCanonical(x, y)] = c;
}

inline uint32_t Grid::getCanonical(int32_t x, int32_t y) const {
  if (!_torus)
    return y * _width + x;

  // x' = x mod _width
  // y' = y mod _height
  return _mod(y, _height) * _width + _mod(x, _width);
}

bool Grid::inBounds(int32_t x, int32_t y) const {
  return _torus || ((x >= 0) && (x < (int64_t) _width) && (y >= 0) && (y < (int64_t) _height));
}

uint32_t Grid::getWidth() const {
  return _width;
}

uint32_t Grid::getHeight() const {
  return _height;
}

bool Grid::isTorus() const {
  return _torus;
}
