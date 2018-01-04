#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "defines.h"
#include "vecteur.h"
#include "particule.h"
#include "toolbox.h"

class world
{
  public:
    double dt;
    particule *tab_part;
    
    
    world();
    ~world();
    
    void calcul_force_particules();
    void integration_verlet();
    void conditions_limites_BVK(vecteur &r);
    void distribution_normale_vitesse();
    void thermalisation();
    
};

#endif // WORLD_H_INCLUDED
