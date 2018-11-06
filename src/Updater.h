#ifndef CELTEX_UPDATOR_H
#define CELTEX_UPDATOR_H

#include <cstdint>
#include <vector>
#include <random>

#include "Grid.h"

class Updater {
 public:
  Updater() : _threads(1) {}
  Updater(size_t threads) : _threads(threads) {}

  virtual void update(Grid *grid, Grid *dest);
  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

 private:
  size_t _threads;
};

class GenGOL : public Updater {
 public:
  enum RuleFlag {
    kZero  =   1,
    kOne   =   2,
    kTwo   =   4,
    kThree =   8,
    kFour  =  16,
    kFive  =  32,
    kSix   =  64,
    kSeven = 128,
    kEight = 256
  };

  GenGOL()
      : Updater(), _B(RuleFlag::kThree ), _S(RuleFlag::kTwo | RuleFlag::kThree) {}

  GenGOL(size_t thr, uint16_t b, uint16_t s)
      : Updater(thr), _B(b), _S(s) {}

  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h);

 private:
  uint16_t _B;
  uint16_t _S;
};

class StochasticGOL : public Updater {
 public:
  // default is just standard GOL
  StochasticGOL() : Updater(), _pB({0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}),
      _pS({0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}) {}

  StochasticGOL(size_t threads, std::vector<double> pB, std::vector<double> pS)
      : Updater(threads), _pB(pB), _pS(pS) {}

  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h);

 private:
  std::default_random_engine _rand;
  std::vector<double> _pB;
  std::vector<double> _pS;
};

class JustFriends : public Updater {
 public:
  JustFriends() : Updater() {}
  JustFriends(size_t thr) : Updater(thr) {}

  virtual void updateChunk(Grid *grid, Grid *dest,
      uint32_t x, uint32_t y, uint32_t w, uint32_t h);
};

#endif