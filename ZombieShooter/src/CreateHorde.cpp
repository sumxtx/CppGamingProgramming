#include "ZombieArena.hpp"
#include "Zombie.hpp"
#include <iostream>

Zombie* createHorde(int numZombies, IntRect arena)
{
  Zombie* zombies = new Zombie[numZombies];
  int maxY = arena.height - 50;
  int minY = arena.top + 50;
  int maxX = arena.width - 50;
  int minX = arena.left + 50;

  for(int i = 0; i < numZombies; i++)
  {
    srand((int)time(0) * i);
    int side = (rand() % 4);
    std::cout << " Zombie " << i << " side " << side << std::endl;
    float x, y;
    switch(side)
    {
      case 0: // L
        x = minX;
        y = (rand() % maxY) + minY;
        break;
      case 1: // R
        x = maxX;
        y = (rand() % maxY) + minY;
        break;
      case 2: // T
        x = (rand() % maxX) + minX;
        y = minY;
        break;
      case 3: // B
        x = (rand() % maxX) + minX;
        y = maxY;
        break;
    }
    srand((int)time(0) * i * 2);
    int type = (rand() % 3);
    zombies[i].spawn(x, y, type, i);
    std::cout << "Zombie Location " << x << " " << y << " " << std::endl;
  }
  return zombies;
}
