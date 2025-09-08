#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.hpp"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena, int wave);


