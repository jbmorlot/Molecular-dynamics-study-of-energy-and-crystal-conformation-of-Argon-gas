#include "vecteur.h"
#include "defines.h"

using namespace std;

vecteur::vecteur()
{
    x=0;
    y=0;
    z=0;
}

double vecteur::norme()
{
  return sqrt( x*x+y*y+z*z);
}

void vecteur::set(double x1, double y1, double z1)
{
    x=x1;
    y=y1;
    z=z1;
}

vecteur::~vecteur()
{}


//Surcharge d'opérateur
vecteur vecteur::operator+(const vecteur &a) const
{
  vecteur c;
  c.x = x + a.x;
  c.y = y + a.y;
  c.z = z + a.z;
  return c;
}

vecteur vecteur::operator*(const double & a) const
{
  vecteur b;
  b.x = x*a; 
  b.y = y*a;
  b.z = z*a;
  return b;
}

double vecteur::operator*(const vecteur & a) const
{
  return x*a.x+y*a.y+z*a.z;
}


vecteur vecteur::operator-(const vecteur & a) const
{
  vecteur c;
  c.x = x - a.x;
  c.y = y - a.y;
  c.z = z - a.z;
  return c;
}

vecteur& vecteur::operator=(const vecteur & a)
{
  x = a.x;
  y = a.y;
  z = a.z;
  return *this;
}


vecteur& vecteur::operator+=(const vecteur & s)
{
  x=x + s.x;
  y=y + s.y;
  z=z + s.z;
  return *this;
}

std::ostream& operator<<(std::ostream& flux, const vecteur & v)
{
  flux<<v.x<<"\t"<<v.y<<"\t"<<v.z<< endl;
  return flux;	
}
