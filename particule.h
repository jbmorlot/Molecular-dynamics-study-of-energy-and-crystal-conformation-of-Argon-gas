#ifndef PARTICULE_H_INCLUDED
#define PARTICULE_H_INCLUDED

#include <iostream>
#include <cmath>

#include "defines.h"
#include "vecteur.h"



class particule
{
  public:
    vecteur r;
    vecteur v;
    vecteur force;
    double rayon;
    double masse;
    double dist_int6;
    
    particule();
    void set(vecteur r1);
    vecteur force_paire(vecteur r2);
    ~particule();
    
    particule& operator=(const particule& a);
    friend std::ostream& operator<<(std::ostream& flux, const particule & v);

    
};





#endif // PARTICULE_H_INCLUDED