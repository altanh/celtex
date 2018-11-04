#include <iostream>
#include <memory>

#include "Grid.h"
#include "Automaton.h"
#include "Updater.h"
#include "Pattern.h"

int main(int argc, char **argv) {
  Automaton aut(32, 32, true, std::shared_ptr<Updater>(new StochasticGOL()));

  Pattern glider(3, 3, {
    { false }, {  true }, { false },
    { false }, { false }, {  true },
    {  true }, {  true }, {  true }
  });

  Pattern pentomino(3, 3, {
    { false }, {  true }, {  true },
    {  true }, {  true }, { false },
    { false }, {  true }, { false }
  });

  aut.putPattern(14, 14, pentomino);

  while (true) {
    const Grid &g = aut.getGrid();

    for (size_t y = 0; y < g.getHeight(); ++y) {
      for (size_t x = 0; x < g.getWidth(); ++x) {
        std::cout << (g.getCell(x,y).state ? "*" : " ");
      }
      std::cout << std::endl;
    }

    aut.step();

    std::cin.get();
  }

  return 0;
}